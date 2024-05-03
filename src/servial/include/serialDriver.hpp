// C++ system
#include <future>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "rclcpp/rclcpp.hpp"

#include "serial/serial.h"

#include "CRC.h"
#include "CRC8_CRC16.h"

#include "my_msgss/msg/gamestate.hpp"
#include "my_msgss/msg/supplyaction.hpp"
#include "my_msgss/msg/refereewarning.hpp"
#include "my_msgss/msg/points.hpp"

#include "opencv2/opencv.hpp"

using namespace serial;

struct serialRobot
{
    uint16_t id;
    bool is_continue;
    float confidence;
    float x;
    float y;
};

//消息头
struct frame_header
{
    uint8_t SOF = 0xA5;//固定值
    uint16_t data_length = 10;//data的长度
    uint8_t seq; //包序号
    uint8_t crc; //帧头crc8
} __attribute__((packed));

//小地图消息数据 10hz 发送
struct point_data
{
    uint16_t target_robot_id;
    float target_position_x;
    float target_position_y;
} __attribute__((packed));
struct point_msg {
    frame_header head;
    uint16_t cmd_id = 0x0305;
    point_data data;
    uint16_t crc;
} __attribute__((packed));

//车间通讯
struct robot_interactive_data//最大10HZ 发送和接收
{
    uint16_t cmd_id;
    uint16_t sender_id;
    uint16_t receiver_id;
    uint8_t content[13];
} __attribute__((packed));

struct graphic_data_struct_t
{
    uint8_t graphic_name[3];
    uint32_t operate_tpye:3;
    uint32_t graphic_tpye:3;
    uint32_t layer:4;
    uint32_t color:4;
    uint32_t start_angle:9;
    uint32_t end_angle:9;
    uint32_t width:10;
    uint32_t start_x:11;
    uint32_t start_y:11;
    uint32_t radius:10;
    uint32_t end_x:11;
    uint32_t end_y:11;
} __attribute__((packed));
struct client_ui_data//最大10HZ 发送和接收
{
    uint16_t cmd_id;
    uint16_t sender_id;
    uint16_t receiver_id;
    graphic_data_struct_t data;
} __attribute__((packed));
struct client_ui_msgs {
    frame_header head;
    uint16_t cmd_id = 0x0301;
    client_ui_data data;
    uint16_t crc;
} __attribute__((packed));

struct _graphic_delete_t
{
    uint8_t operate_tpye;
    uint8_t layer;
} __attribute__((packed));
struct client_ui_delete_data//最大10HZ 发送和接收
{
    uint16_t cmd_id;
    uint16_t sender_id;
    uint16_t receiver_id;
    _graphic_delete_t data;
} __attribute__((packed));
struct client_ui_delete_msgs {
    frame_header head;
    uint16_t cmd_id = 0x0301;
    client_ui_delete_data data;
    uint16_t crc;
} __attribute__((packed));
struct robot_interactive_msgs {
    frame_header head;
    uint16_t cmd_id = 0x0301;
    robot_interactive_data data;
    uint16_t crc;
} __attribute__((packed));

//自定义控制消息
struct robot_interactive_control_data //30HZ 发送和接收
{
    uint8_t content[30];
} __attribute__((packed));
struct robot_interactive_control_msgs {
    frame_header head;
    uint16_t cmd_id = 0x0302;
    robot_interactive_control_data data;
    uint16_t crc;
} __attribute__((packed));

//车辆血量消息
struct robot_health_data //1hz 接收
{
    uint16_t red_1_robot_HP = 0;
    uint16_t red_2_robot_HP = 0;
    uint16_t red_3_robot_HP = 0;
    uint16_t red_4_robot_HP = 0;
    uint16_t red_5_robot_HP = 0;
    uint16_t red_7_robot_HP = 0;
    uint16_t red_outpose_HP = 0;//前哨站
    uint16_t red_base_HP = 0;//基地

    uint16_t blue_1_robot_HP = 0;
    uint16_t blue_2_robot_HP = 0;
    uint16_t blue_3_robot_HP = 0;
    uint16_t blue_4_robot_HP = 0;
    uint16_t blue_5_robot_HP = 0;
    uint16_t blue_7_robot_HP = 0;
    uint16_t blue_outpose_HP = 0;//前哨站
    uint16_t blue_base_HP = 0;//基地
} __attribute__((packed));
struct robot_health_msgs //1HZ
{
    frame_header head;
    uint16_t cmd_id = 0x0003;
    robot_health_data data;
    uint16_t crc;
} __attribute__((packed));

//比赛状态信息
struct game_status_data //1hz 接收
{
    uint8_t game_type: 4; //1：机甲大师赛 2：单项赛 3：人工智能挑战赛 4：联盟赛3v3 5：联盟赛1v1
    uint8_t game_progress: 4; //0：未开始比赛 1：准备阶段 2：自检阶段 3：5s倒计时 4：对战中 5：比赛结算中
    uint16_t stage_remain_time = 0; //当前阶段剩余时间，单位s
    uint64_t SyncTimeStamp = 0; //机器人接收到该指令的精确Unix时间,当机载端收到有效的NTP服务器授时后生效

} __attribute__((packed));
struct game_status_msgs {
    frame_header head;
    uint16_t cmd_id = 0x0001;
    game_status_data data;
    uint16_t crc;
} __attribute__((packed));

//比赛结果
struct game_result_data//比赛结束发送 接收
{
    uint8_t winner; //0平局 1红方胜利 2蓝方胜利
} __attribute__((packed));
struct game_result_msg {
    frame_header head;
    uint16_t cmd_id = 0x0002;
    game_result_data data;
    uint16_t crc;
} __attribute__((packed));

//飞镖交互消息
struct dart_interactivate_msg_data {
    uint8_t dart_launch_opening_status; //fei biao fa she kou zhuang tai 1 guan bi 2 zheng zai huo dong 0 yi jing kai qi
    uint8_t dart_attack_target; //飞镖的打击目标,默认为前哨站;0:前哨站; 1:基地。
    uint16_t target_change_time; //切换打击目标时的比赛剩余时间,单位秒,从未切换默认为 0。
    uint16_t operate_launch_cmd_time; //最近一次操作手确定发射指令时的比赛剩余时间,单位秒, 初始值为 0。
} __attribute__((packed));

//场地机关占领消息
struct site_event_data //1hz 接收
{
    uint32_t event_type;
    //bit 0:己方补给站 1 号补血点占领状态 1 为已占领;
    //bit 1:己方补给站 2 号补血点占领状态 1 为已占领;
    //bit 2:己方补给站 3 号补血点占领状态 1 为已占领;
    //bit 3-5:己方能量机关状态:
    // bit 3 为打击点占领状态,1 为占领;
    // bit 4 为小能量机关激活状态,1 为已激活;
    // bit 5 为大能量机关激活状态,1 为已激活;
    //bit 6:己方侧 R2/B2 环形高地占领状态 1 为已占领;
    //bit 7:己方侧 R3/B3 梯形高地占领状态 1 为已占领;
    //bit 8:己方侧 R4/B4 梯形高地占领状态 1 为已占领;
    //bit 9:己方基地护盾状态: 1 为基地有虚拟护盾血量; 0 为基地无虚拟护盾血量;
    //bit 10:己方前哨战状态: 1 为前哨战存活; 0 为前哨战被击毁;
    //bit 10 -31: 保留
} __attribute__((packed));
struct site_event_msgs {
    frame_header head;
    uint16_t cmd_id = 0x0101;
    site_event_data data;
    uint16_t crc;
} __attribute__((packed));

//补给站消息
struct supply_projectile_action_data //触发时发送 接收
{
    uint8_t supply_projectile_id; //补给站口ID 1一号补给口 2二号补给口
    uint8_t supply_robot_id; //补弹机器人ID
    uint8_t supply_projectile_step; //出弹口开闭状态 0关闭 1子弹准备中 2子弹下落
    uint8_t supply_projectile_num; //补单数量 50:50颗子弹 100:100颗子弹 150...... 200......
} __attribute__((packed));
struct supply_projectile_action_msg {
    frame_header head;
    uint16_t cmd_id = 0x0102;
    supply_projectile_action_data data;
    uint16_t crc;
} __attribute__((packed));

//裁判判罚消息
struct referee_warning_data //触发时发送 接收
{
    uint8_t level; //1黄牌 2红牌 3判负
    uint8_t foul_robot_id; //犯规机器人ID 判负时为0
} __attribute__((packed));
struct referee_warning_msg {
    frame_header head;
    uint16_t cmd_id = 0x0104;
    referee_warning_data data;
    uint16_t crc;
} __attribute__((packed));

//飞镖闸门关闭倒计时
struct dart_remaining_time_data //1hz 接收
{
    uint8_t dart_remaining_time; //15s倒计时
} __attribute__((packed));
struct dart_remaining_time_msg {
    frame_header head;
    uint16_t cmd_id = 0x0105;
    dart_remaining_time_data data;
    uint16_t crc;
} __attribute__((packed));

uint8_t warn_state;


class SerialDriver : public rclcpp::Node
{
public:
  SerialDriver();

  void serial_init();

  bool receiveAllData();

  void robots_init();

  void allrobots_adjust();

  void worldPointsCallback(const my_msgss::msg::Points msg);

  void serialCommunication();

  bool sendPointsData();

  bool our_color; // 0,red 1,blue
  
  serial::Serial serial_port;
  uint8_t receiveData[1024];
  point_msg pointMsg;
  robot_interactive_msgs robotInteractiveMsgs;
  robot_interactive_msgs HeroMsgs;
  robot_interactive_control_msgs robotInteractiveControlMsgs;
  robot_health_msgs robotHealthMsgs;
  game_result_msg gameResultMsg;
  site_event_msgs siteEventMsgs;
  supply_projectile_action_msg supplyProjectileActionMsg;
  referee_warning_msg refereeWarningMsg;
  dart_remaining_time_msg dartRemainingTimeMsg;
  game_status_msgs gameStatusMsgs;

  my_msgss::msg::Gamestate gameStateRosMsg;
  my_msgss::msg::Supplyaction supplyProjectileActionRosMsg;
  my_msgss::msg::Refereewarning refereeWarningRosMsg;

  std::vector<serialRobot> serialRobots;

  rclcpp::Subscription<my_msgss::msg::Points>::SharedPtr worldPointsSub;

  rclcpp::Publisher<my_msgss::msg::Gamestate>::SharedPtr gameStatePub;

  rclcpp::TimerBase::SharedPtr timer;
};