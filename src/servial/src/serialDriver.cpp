#include "serialDriver.hpp"

SerialDriver::SerialDriver() : Node("serial")
{
    serial_init();

    robots_init();

    our_color = 0;

    worldPointsSub = this->create_subscription<my_msgss::msg::Points>("/serial/world_points", 10, std::bind(&SerialDriver::worldPointsCallback, this, std::placeholders::_1));

    gameStatePub = this->create_publisher<my_msgss::msg::Gamestate>("/game_state", 10);

    timer = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&SerialDriver::serialCommunication, this));
}

void SerialDriver::serial_init()
{
    serial_port.setPort("/dev/ttyUSB0");
    serial_port.setBaudrate(115200);
    serial::Timeout to = serial::Timeout::simpleTimeout(1000);
    serial_port.setTimeout(to);
    serial_port.open();
}

bool SerialDriver::receiveAllData()
{
    if(serial_port.available())
    {
        bool if_pub = false;
        serial_port.read(receiveData, serial_port.available());
        //使用所有消息类型进行匹配，若CRC校验通过，则匹配成功
            gameStatusMsgs = (*(game_status_msgs *) receiveData);
            dartRemainingTimeMsg = (*(dart_remaining_time_msg *) receiveData);
            robotHealthMsgs = (*(robot_health_msgs *) receiveData);
            gameResultMsg = (*(game_result_msg *) receiveData);
            siteEventMsgs = (*(site_event_msgs *) receiveData);
            supplyProjectileActionMsg = (*(supply_projectile_action_msg *) receiveData);
            refereeWarningMsg = (*(referee_warning_msg *) receiveData);
            dartRemainingTimeMsg = (*(dart_remaining_time_msg *) receiveData);

            gameStateRosMsg.dart_remaining_time = 16;
            gameStateRosMsg.winner = 3;
            if ((gameStatusMsgs.head.crc == get_CRC8_check_sum((uint8_t *) &gameStatusMsgs,
                                                               (sizeof(gameStatusMsgs.head) -
                                                                sizeof(gameStatusMsgs.head.crc)), 0xff)) &&
                (gameStatusMsgs.crc ==
                 get_CRC16_check_sum((uint8_t *) &gameStatusMsgs, (sizeof(gameStatusMsgs) - sizeof(gameStatusMsgs.crc)),
                                     0xffff))) {

                gameStateRosMsg.game_progress = gameStatusMsgs.data.game_progress;
                gameStateRosMsg.game_type = gameStatusMsgs.data.game_type;
                gameStateRosMsg.stage_remain_time = gameStatusMsgs.data.stage_remain_time;
                if_pub = true;
            }
            if ((dartRemainingTimeMsg.head.crc == get_CRC8_check_sum((uint8_t *) &dartRemainingTimeMsg,
                                                                     (sizeof(dartRemainingTimeMsg.head) -
                                                                      sizeof(dartRemainingTimeMsg.head.crc)), 0xff)) &&
                (dartRemainingTimeMsg.crc == get_CRC16_check_sum((uint8_t *) &dartRemainingTimeMsg,
                                                                 (sizeof(dartRemainingTimeMsg) -
                                                                  sizeof(dartRemainingTimeMsg.crc)), 0xffff))) {
                gameStateRosMsg.dart_remaining_time = dartRemainingTimeMsg.data.dart_remaining_time;
                if_pub = true;
            }
            if ((robotHealthMsgs.head.crc == get_CRC8_check_sum((uint8_t *) &robotHealthMsgs,
                                                                (sizeof(robotHealthMsgs.head) -
                                                                 sizeof(robotHealthMsgs.head.crc)), 0xff)) &&
                (robotHealthMsgs.crc == get_CRC16_check_sum((uint8_t *) &robotHealthMsgs,
                                                            (sizeof(robotHealthMsgs) - sizeof(robotHealthMsgs.crc)),
                                                            0xffff))) {
                gameStateRosMsg.blue_1_hp = robotHealthMsgs.data.blue_1_robot_HP;
                gameStateRosMsg.blue_2_hp = robotHealthMsgs.data.blue_2_robot_HP;
                gameStateRosMsg.blue_3_hp = robotHealthMsgs.data.blue_3_robot_HP;
                gameStateRosMsg.blue_4_hp = robotHealthMsgs.data.blue_4_robot_HP;
                gameStateRosMsg.blue_5_hp = robotHealthMsgs.data.blue_5_robot_HP;
                gameStateRosMsg.blue_7_hp = robotHealthMsgs.data.blue_7_robot_HP;
                gameStateRosMsg.blue_base_hp = robotHealthMsgs.data.blue_base_HP;
                gameStateRosMsg.blue_outpose_hp = robotHealthMsgs.data.blue_outpose_HP;
                gameStateRosMsg.red_1_hp = robotHealthMsgs.data.red_1_robot_HP;
                gameStateRosMsg.red_2_hp = robotHealthMsgs.data.red_2_robot_HP;
                gameStateRosMsg.red_3_hp = robotHealthMsgs.data.red_3_robot_HP;
                gameStateRosMsg.red_4_hp = robotHealthMsgs.data.red_4_robot_HP;
                gameStateRosMsg.red_5_hp = robotHealthMsgs.data.red_5_robot_HP;
                gameStateRosMsg.red_7_hp = robotHealthMsgs.data.red_7_robot_HP;
                gameStateRosMsg.red_base_hp = robotHealthMsgs.data.red_base_HP;
                gameStateRosMsg.red_outpose_hp = robotHealthMsgs.data.red_outpose_HP;
                if_pub = true;
            }
            if ((gameResultMsg.head.crc == get_CRC8_check_sum((uint8_t *) &gameResultMsg, (sizeof(gameResultMsg.head) -
                                                                                           sizeof(gameResultMsg.head.crc)),
                                                              0xff)) && (gameResultMsg.crc ==
                                                                         get_CRC16_check_sum((uint8_t *) &gameResultMsg,
                                                                                             (sizeof(gameResultMsg) -
                                                                                              sizeof(gameResultMsg.crc)),
                                                                                             0xffff))) {
                gameStateRosMsg.winner = gameResultMsg.data.winner;
                if_pub = true;
            }
            if ((siteEventMsgs.head.crc == get_CRC8_check_sum((uint8_t *) &siteEventMsgs, (sizeof(siteEventMsgs.head) -
                                                                                           sizeof(siteEventMsgs.head.crc)),
                                                              0xff)) && (siteEventMsgs.crc ==
                                                                         get_CRC16_check_sum((uint8_t *) &siteEventMsgs,
                                                                                             (sizeof(siteEventMsgs) -
                                                                                              sizeof(siteEventMsgs.crc)),
                                                                                             0xffff))) {
                gameStateRosMsg.if_supply_projectile_one_occupied = (siteEventMsgs.data.event_type | 0x80000000);
                gameStateRosMsg.if_supply_projectile_two_occupied = (siteEventMsgs.data.event_type | 0x40000000);
                gameStateRosMsg.if_supply_projectile_three_occupied = (siteEventMsgs.data.event_type | 0x20000000);
                gameStateRosMsg.if_wind_mill_hit_place_occupied = (siteEventMsgs.data.event_type | 0x10000000);
                gameStateRosMsg.if_wind_mill_big_lighted = (siteEventMsgs.data.event_type | 0x80000000);
                gameStateRosMsg.if_wind_mill_small_lighted = (siteEventMsgs.data.event_type | 0x08000000);
                gameStateRosMsg.if_rb2_occupied = (siteEventMsgs.data.event_type | 0x04000000);
                gameStateRosMsg.if_rb3_occupied = (siteEventMsgs.data.event_type | 0x02000000);
                gameStateRosMsg.if_rb4_occupied = (siteEventMsgs.data.event_type | 0x01000000);
                gameStateRosMsg.if_base_protected = (siteEventMsgs.data.event_type | 0x00800000);
                gameStateRosMsg.if_outpose_alive = (siteEventMsgs.data.event_type | 0x00400000);
                if_pub = true;
            }
            if ((supplyProjectileActionMsg.head.crc == get_CRC8_check_sum((uint8_t *) &supplyProjectileActionMsg,
                                                                          (sizeof(supplyProjectileActionMsg.head) -
                                                                           sizeof(supplyProjectileActionMsg.head.crc)),
                                                                          0xff)) && (supplyProjectileActionMsg.crc ==
                                                                                     get_CRC16_check_sum(
                                                                                             (uint8_t *) &supplyProjectileActionMsg,
                                                                                             (sizeof(supplyProjectileActionMsg) -
                                                                                              sizeof(supplyProjectileActionMsg.crc)),
                                                                                             0xffff))) {
                supplyProjectileActionRosMsg.supply_projectile_id = supplyProjectileActionMsg.data.supply_projectile_id;
                supplyProjectileActionRosMsg.supply_robot_id = supplyProjectileActionMsg.data.supply_robot_id;
                supplyProjectileActionRosMsg.supply_projectile_step = supplyProjectileActionMsg.data.supply_projectile_step;
                supplyProjectileActionRosMsg.supply_projectile_num = supplyProjectileActionMsg.data.supply_projectile_num;
                if_pub = true;
            }
            if ((refereeWarningMsg.head.crc == get_CRC8_check_sum((uint8_t *) &refereeWarningMsg,
                                                                  (sizeof(refereeWarningMsg.head) -
                                                                   sizeof(refereeWarningMsg.head.crc)), 0xff)) &&
                (refereeWarningMsg.crc == get_CRC16_check_sum((uint8_t *) &refereeWarningMsg,
                                                              (sizeof(refereeWarningMsg) -
                                                               sizeof(refereeWarningMsg.crc)), 0xffff))) {
                refereeWarningRosMsg.level = refereeWarningMsg.data.level;
                refereeWarningRosMsg.foul_robot_id = refereeWarningMsg.data.foul_robot_id;
                if_pub = true;
            }
            if ((dartRemainingTimeMsg.head.crc == get_CRC8_check_sum((uint8_t *) &dartRemainingTimeMsg,
                                                                     (sizeof(dartRemainingTimeMsg.head) -
                                                                      sizeof(dartRemainingTimeMsg.head.crc)), 0xff)) &&
                (dartRemainingTimeMsg.crc == get_CRC16_check_sum((uint8_t *) &dartRemainingTimeMsg,
                                                                 (sizeof(dartRemainingTimeMsg) -
                                                                  sizeof(dartRemainingTimeMsg.crc)), 0xffff))) {
                gameStateRosMsg.dart_remaining_time = dartRemainingTimeMsg.data.dart_remaining_time;
                if_pub = true;
            }
            if (if_pub) {
                gameStatePub->publish(gameStateRosMsg);
                return true;
            }
            return false;
        }
        return false;
}

void SerialDriver::worldPointsCallback(const my_msgss::msg::Points msg)
{
    int armor_number = -1;
    float confidence = 0.0;
    float x = 0.0;
    float y = 0.0;
    for(int i =0;i<msg.data.size();i++)
    {
        armor_number = msg.data[i].id;
        x = msg.data[i].x;
        y = msg.data[i].y;
        confidence = msg.data[i].confidence;
        serialRobots[armor_number].confidence = confidence;
        serialRobots[armor_number].x = x;
        serialRobots[armor_number].y = y;
        serialRobots[armor_number].is_continue = true;
    }
    allrobots_adjust();
}

void SerialDriver::robots_init()
{
    serialRobot robot;
    robot.id = -1;
    robot.x = 0.0;
    robot.y = 0.0;
    robot.confidence = 0.0;
    robot.is_continue = false;
    for(int i =0;i<13;i++)
    {
        robot.id++;
        serialRobots.push_back(robot);
    }
}

void SerialDriver::allrobots_adjust()
{
    for(int i = 1;i<serialRobots.size();i++)
    {
        if(serialRobots[i].is_continue == false)
        {
            serialRobots[i].confidence = 0.0;
            serialRobots[i].x = 0.0;
            serialRobots[i].y = 0.0;
        }
        serialRobots[i].is_continue = false;
    }
}

void SerialDriver::serialCommunication()
{
    sendPointsData();

    receiveAllData();
}

//发送信息的函数
//-----------------------------------------------------------------------------------------
bool SerialDriver::sendPointsData()
{
    bool if_send = false;
    serialRobot best_robot;
    best_robot.confidence = 0.0;
    if(our_color == 0)
    {
        for(int i =7;i<serialRobots.size();i++)
        {
            if(serialRobots[i].x !=0.0 && serialRobots[i].y != 0.0)
            {
                if(serialRobots[i].confidence > best_robot.confidence)
                {
                    best_robot = serialRobots[i];
                    if_send = true;
                }
            }
        }
    }
    else if(our_color == 1)
    {
        for(int i =1;i<7;i++)
        {
            if(serialRobots[i].x !=0.0 && serialRobots[i].y != 0.0)
            {
                if(serialRobots[i].confidence > best_robot.confidence)
                {
                    best_robot = serialRobots[i];
                    if_send = true;
                }
            }
        }
    }
    if(if_send == false)
    {
        return false;
    }
    else
    {
        pointMsg.head.SOF = 0xA5;
        pointMsg.head.data_length = 10;
        pointMsg.head.seq = 1;
        pointMsg.head.crc = get_CRC8_check_sum((uint8_t *) &pointMsg, (sizeof(pointMsg.head) - sizeof(pointMsg.head.crc)),
                                             0xff);
        pointMsg.cmd_id = 0x0305;
        pointMsg.data.target_position_x = best_robot.x;
        pointMsg.data.target_position_y = best_robot.y;
        pointMsg.data.target_robot_id = best_robot.id;
        pointMsg.crc = get_CRC16_check_sum((uint8_t *) &pointMsg, (sizeof(pointMsg) - sizeof(pointMsg.crc)), 0xffff);
        serial_port.write((uint8_t *) &pointMsg, sizeof(point_msg));
        std::cout << "Send one point msg target_id = " << pointMsg.data.target_robot_id << " x = "
             << pointMsg.data.target_position_x << " y = " << pointMsg.data.target_position_y << std::endl;
        return true;
    }
}

//-----------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SerialDriver>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}