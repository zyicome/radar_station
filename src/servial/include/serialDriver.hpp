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
#include "my_msgss/msg/radarmark.hpp"
#include "my_msgss/msg/hp.hpp"
#include "my_msgss/msg/radarinfo.hpp"
#include "my_msgss/msg/dart.hpp"


#include "std_msgs/msg/int8.hpp"

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

//我们发送的消息结构体
//-------------------------------------------------
//小地图消息数据 10hz 发送
struct point_data
{
    uint16_t hero_position_x;
    uint16_t hero_position_y;
    uint16_t engineer_position_x;
    uint16_t engineer_position_y;
    uint16_t infantry_3_position_x;
    uint16_t infantry_3_position_y;
    uint16_t infantry_4_position_x;
    uint16_t infantry_4_position_y;
    uint16_t infantry_5_position_x;
    uint16_t infantry_5_position_y;
    uint16_t sentry_position_x;
    uint16_t sentry_position_y;
} __attribute__((packed));
struct point_msg {
    frame_header head;
    uint16_t cmd_id = 0x0305;
    point_data data;
    uint16_t crc;
} __attribute__((packed));

//车间通讯
struct robot_interaction_data//最大10HZ 发送和接收
{
    uint16_t data_cmd_id; //子内容 ID需为开放的子内容 ID 
                          //0x0200~0x02FF: x≤112 机器人之间通信
                          //0x0100: 2 选手端删除图层
                          //0x0101: 15 选手端绘制一个图形
                          //0x0102: 30 选手端绘制两个图形
                          //0x0103: 75 选手端绘制五个图形
                          //0x0104: 105 选手端绘制七个图形
                          //0x0110: 45 选手端绘制字符图形
                          //0x0120: 4 哨兵自主决策指令
                          //0x0121: 1 雷达自主决策指令
                          //由于存在多个内容 ID，但整个 cmd_id 上行频率最大为 10Hz，请合理安排带宽。
    uint16_t sender_id; //发送者 ID需与自身 ID 匹配，ID 编号详见附录
    uint16_t receiver_id; //接收者 ID
    uint8_t user_data[112]; //内容数据段 x 最大为 112
} __attribute__((packed));
struct robot_interaction_msgs {
    frame_header head;
    uint16_t cmd_id = 0x0301;
    robot_interaction_data data;
    uint16_t crc;
} __attribute__((packed));

// UI消息
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

struct radar_cmd_data
{
    uint8_t radar_cmd;
    //开局为 0，修改此值即可请求触发双倍易伤，若此时雷达拥有触发双倍易伤的机会，则可触发。此值的变化需要单调递增且每次仅能增加 1，否则视为不合法.
} __attribute__((packed));
struct radar_cmd_msgs {
    frame_header head;
    uint16_t cmd_id = 0x0121;
    radar_cmd_data data;
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

// 接受裁判系统发来的消息
//-------------------------------------------------
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
    //bit 6-7:己方侧 R2/B2 环形高地占领状态 1 为已占领;
    //bit 8-9:己方侧 R3/B3 梯形高地占领状态 1 为已占领;
    //bit 10-11:己方侧 R4/B4 梯形高地占领状态 1 为已占领;
    //bit 12-18:己方基地虚拟护盾的剩余值百分比（四舍五入，保留整数）
    //bit 19-27:飞镖最后一次击中己方前哨站或基地的时间（0-420，开局默认为0）
    //bit 28-29:飞镖最后一次击中己方前哨站或基地的具体目标，开局默认为 0，1 为击中前哨站，2 为击中基地固定目标，3 为击中基地随机目标
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
    //uint8_t supply_projectile_id; //补给站口ID 1一号补给口 2二号补给口
    uint8_t reserved;
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
    uint8_t level; //1双方黄牌 2黄牌 3红牌 4判负
    uint8_t foul_robot_id; //犯规机器人ID（如红 1 机器人 ID 为 1，蓝1 机器人 ID 为 101） 判负和双方黄牌时，该值为 0
    uint8_t count; //己方最后一次受到判罚的违规机器人对应判罚等级的违规次数。（开局默认为 0。）
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
    uint8_t dart_remaining_time; //己方飞镖发射剩余时间，单位：秒
    uint8_t dart_hit_target_info : 2;
    uint8_t dart_hit_target_total_info : 3;
    uint8_t dart_selected_target : 2; 
    //bit 0-1:最近一次己方飞镖击中的目标，开局默认为 0，1 为击中前哨站，2 为击中基地固定目标，3 为击中基地随机目标
    //bit 2-4:对方最近被击中的目标累计被击中计数，开局默认为 0，至多为 4
    //bit 5-6:飞镖此时选定的击打目标，开局默认或未选定/选定前哨站时为 0，选中基地固定目标为 1，选中基地随机目标为 2
    //bit 7-15:保留
} __attribute__((packed));
struct dart_remaining_time_msg {
    frame_header head;
    uint16_t cmd_id = 0x0105;
    dart_remaining_time_data data;
    uint16_t crc;
} __attribute__((packed));

struct air_support_data
{
    uint8_t airforce_status; //空中机器人状态（0 为正在冷却，1 为冷却完毕，2 为正在空中支援）
    uint8_t time_remain; //此状态的剩余时间（单位为：秒，向下取整，即冷却时间剩余 1.9 秒时，此值为 1）若冷却时间为 0，但未呼叫空中支援，则该值为 0
} __attribute__((packed));
struct air_support_msg {
    frame_header head;
    uint16_t cmd_id = 0x0205;
    air_support_data data;
    uint16_t crc;
} __attribute__((packed));

//飞镖交互消息
struct dart_interactivate_msg_data {
    uint8_t dart_launch_opening_status; //当前飞镖发射站的状态：1：关闭 2：正在开启或者关闭中 0：已经开启
    uint8_t reserved; //保留
    uint16_t target_change_time; //切换打击目标时的比赛剩余时间,单位秒,从未切换默认为 0。
    uint16_t operate_launch_cmd_time; //最近一次操作手确定发射指令时的比赛剩余时间,单位秒, 初始值为 0。
} __attribute__((packed));
struct dart_interactivate_msg {
    frame_header head;
    uint16_t cmd_id = 0x020A;
    dart_interactivate_msg_data data;
    uint16_t crc;
} __attribute__((packed));

//标记进度消息
struct radar_mark_data
{
    uint8_t mark_hero_progress; //对方英雄机器人被标记进度：0-120
    uint8_t mark_engineer_progress; //对方工程机器人被标记进度：0-120
    uint8_t mark_standard_3_progress; //对方3号步兵机器人被标记进度：0-120
    uint8_t mark_standard_4_progress; //对方4号步兵机器人被标记进度：0-120 
    uint8_t mark_standard_5_progress; //对方5号步兵机器人被标记进度：0-120
    uint8_t mark_sentry_progress; //对方哨兵机器人被标记进度：0-120
} __attribute__((packed));
struct radar_mark_msg {
    frame_header head;
    uint16_t cmd_id = 0x020C;
    radar_mark_data data;
    uint16_t crc;
} __attribute__((packed));

struct radar_info_data
{
    uint8_t radar_info : 2;
    uint8_t is_double_damage : 1;
    // bit 0-1:雷达是否拥有触发双倍易伤的机会，开局为 0，数值为雷达拥有触发双倍易伤的机会，至多为 2
    // bit 2:对方是否正在被触发双倍易伤 0：对方未被触发双倍易伤 1：对方正在被触发双倍易伤
    // bit 3-7:保留
} __attribute__((packed));
struct radar_info_msg {
    frame_header head;
    uint16_t cmd_id = 0x020E;
    radar_info_data data;
    uint16_t crc;
} __attribute__((packed));

class SerialDriver : public rclcpp::Node
{
public:
  SerialDriver();

  ~SerialDriver();

  void serial_init();

  void robots_init();

  void allrobots_adjust();

  void worldPointsCallback(const my_msgss::msg::Points msg);

  void colorCallback(const std_msgs::msg::Int8::SharedPtr msg);

  void radarInfoCallback(const my_msgss::msg::Radarinfo msg);

  void modeCallback(const std_msgs::msg::Int8 msg);

  void serialCommunication();

  bool sendPointsData();

  void receiveAllData_three();

  bool our_color; // 0,red 1,blue
  bool test;
  
  serial::Serial serial_port;
  uint8_t receiveData[1024];
  point_msg pointMsg;
  robot_interaction_msgs robotInteractiveMsgs;
  robot_interaction_msgs HeroMsgs;
  robot_interactive_control_msgs robotInteractiveControlMsgs;
  robot_health_msgs robotHealthMsgs;
  game_result_msg gameResultMsg;
  site_event_msgs siteEventMsgs;
  supply_projectile_action_msg supplyProjectileActionMsg;
  referee_warning_msg refereeWarningMsg;
  dart_remaining_time_msg dartRemainingTimeMsg;
  game_status_msgs gameStatusMsgs;
  radar_mark_msg radarMarkMsg;
  radar_info_msg radarInfoMsg;

  radar_cmd_msgs radarCmdMsg;


  frame_header frameHeader;

  my_msgss::msg::Gamestate gameStateRosMsg;
  my_msgss::msg::Supplyaction supplyProjectileActionRosMsg;
  my_msgss::msg::Refereewarning refereeWarningRosMsg;
  my_msgss::msg::Radarmark radarMarkRosMsg;
  my_msgss::msg::Hp hpRosMsg;
  my_msgss::msg::Radarinfo radarInfoRosMsg;
  my_msgss::msg::Dart dartRosMsg;

  std::vector<serialRobot> serialRobots;

  rclcpp::Subscription<my_msgss::msg::Points>::SharedPtr worldPointsSub;
  rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr color_sub;
  rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr mode_sub;
  rclcpp::Subscription<my_msgss::msg::Radarinfo>::SharedPtr radarInfoSub;

  rclcpp::Publisher<my_msgss::msg::Gamestate>::SharedPtr gameStatePub;
  rclcpp::Publisher<my_msgss::msg::Points>::SharedPtr worldPointsPub;
  rclcpp::Publisher<my_msgss::msg::Hp>::SharedPtr hpPub;
  rclcpp::Publisher<my_msgss::msg::Radarmark>::SharedPtr radarMarkPub;
  rclcpp::Publisher<my_msgss::msg::Radarinfo>::SharedPtr radarInfoPub;
  rclcpp::Publisher<my_msgss::msg::Dart>::SharedPtr dartPub;

  rclcpp::TimerBase::SharedPtr send_timer;

  std::thread receive_thread;
};