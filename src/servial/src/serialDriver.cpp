#include "serialDriver.hpp"

SerialDriver::SerialDriver() : Node("serial")
{
    serial_init();

    robots_init();

    our_color = 0;

    test = false;

    seq = 0;

    worldPointsSub = this->create_subscription<my_msgss::msg::Points>("/serial/world_points", 10, std::bind(&SerialDriver::worldPointsCallback, this, std::placeholders::_1));

    color_sub = this->create_subscription<std_msgs::msg::Int8>("/our_color", 10, std::bind(&SerialDriver::colorCallback, this, std::placeholders::_1));

    radarInfoSub = this->create_subscription<my_msgss::msg::Radarinfo>("/serial/radar_info", 10, std::bind(&SerialDriver::radarInfoCallback, this, std::placeholders::_1));

    mode_sub = this->create_subscription<std_msgs::msg::Int8>("/mode", 10, std::bind(&SerialDriver::modeCallback, this, std::placeholders::_1));

    gameStatePub = this->create_publisher<my_msgss::msg::Gamestate>("/game_state", 10);

    worldPointsPub = this->create_publisher<my_msgss::msg::Points>("/test_world_points", 10);

    radarMarkPub = this->create_publisher<my_msgss::msg::Radarmark>("/radar_mark", 10);

    hpPub = this->create_publisher<my_msgss::msg::Hp>("/hp", 10);

    radarInfoPub = this->create_publisher<my_msgss::msg::Radarinfo>("/radar_info", 10);

    dartPub = this->create_publisher<my_msgss::msg::Dart>("/dart", 10);

    siteEventPub = this->create_publisher<my_msgss::msg::Siteevent>("/site_event", 10);

    send_timer = this->create_wall_timer(std::chrono::milliseconds(200), std::bind(&SerialDriver::serialCommunication, this));

    receive_thread = std::thread(&SerialDriver::receiveAllData_three, this);
}

SerialDriver::~SerialDriver()
{
    serial_port.close();

    if(receive_thread.joinable())
    { 
        receive_thread.join();
    }
}

void SerialDriver::serial_init()
{
    serial_port.setPort("/dev/ttyUSB0");
    serial_port.setBaudrate(115200);
    serial::Timeout to = serial::Timeout::simpleTimeout(1000);
    serial_port.setTimeout(to);
    serial_port.open();
}

void SerialDriver::receiveAllData_three()
{
    bool is_head = false;
    uint16_t CMD_ID = 0;
    uint16_t data_length = 0;
    uint8_t receive_data[1024];
    int pos = 0;
    bool if_pub = false;

    while(rclcpp::ok())
    {
        if(serial_port.available())
        {
            serial_port.read(receive_data, 1024);
            pos = 0;
            while(pos<1024)
            {
                if(receive_data[pos] == 0xA5)
                {
                    //pos:0xA5  pos+1 and pos+2:data_length pos+3:seq pos+4:CRC8 pos+5 and pos+6:cmd_id
                    //pos+7 to pos+data_length+7:data pos+data_length+8 and pos+data_length+9:CRC16
                    if(receive_data[pos + 4] != get_CRC8_check_sum((uint8_t *) &gameStatusMsgs,(sizeof(gameStatusMsgs.head) -sizeof(gameStatusMsgs.head.crc)), 0xff))
                    {
                        data_length = (int)receive_data[pos + 1] + (int)receive_data[pos + 2]*256;
                        CMD_ID = (receive_data[pos + 5]&0xff)|((receive_data[pos + 6]<<8)&0xff00);
                        switch (CMD_ID)
                        {
                            case 0x0001:
                            memcpy(&gameStatusMsgs, receive_data + pos, data_length + 9);
                            if(gameStatusMsgs.crc == get_CRC16_check_sum((uint8_t *) &gameStatusMsgs,(sizeof(gameStatusMsgs) - sizeof(gameStatusMsgs.crc)), 0xffff))
                            {
                                gameStateRosMsg.game_progress = gameStatusMsgs.data.game_progress;
                                gameStateRosMsg.game_type = gameStatusMsgs.data.game_type;
                                gameStateRosMsg.stage_remain_time = gameStatusMsgs.data.stage_remain_time;
                                gameStatePub->publish(gameStateRosMsg);
                            }
                            break;
                        case 0x0002:
                            memcpy(&gameResultMsg, receive_data + pos, data_length + 9);
                            if(gameResultMsg.crc == get_CRC16_check_sum((uint8_t *) &gameResultMsg,(sizeof(gameResultMsg) - sizeof(gameResultMsg.crc)), 0xffff))
                            {

                            }
                            break;
                        case 0x0003:
                            memcpy(&robotHealthMsgs, receive_data + pos, data_length + 9);
                            if(robotHealthMsgs.crc == get_CRC16_check_sum((uint8_t *) &robotHealthMsgs,(sizeof(robotHealthMsgs) - sizeof(robotHealthMsgs.crc)), 0xffff))
                            {
                                hpRosMsg.red_1_hp = robotHealthMsgs.data.red_1_robot_HP;
                                hpRosMsg.red_2_hp = robotHealthMsgs.data.red_2_robot_HP;
                                hpRosMsg.red_3_hp = robotHealthMsgs.data.red_3_robot_HP;
                                hpRosMsg.red_4_hp = robotHealthMsgs.data.red_4_robot_HP;
                                hpRosMsg.red_5_hp = robotHealthMsgs.data.red_5_robot_HP;
                                hpRosMsg.red_7_hp = robotHealthMsgs.data.red_7_robot_HP;
                                hpRosMsg.red_outpose_hp = robotHealthMsgs.data.red_outpose_HP;
                                hpRosMsg.red_base_hp = robotHealthMsgs.data.red_base_HP;
                                hpRosMsg.blue_1_hp = robotHealthMsgs.data.blue_1_robot_HP;
                                hpRosMsg.blue_2_hp = robotHealthMsgs.data.blue_2_robot_HP;
                                hpRosMsg.blue_3_hp = robotHealthMsgs.data.blue_3_robot_HP;
                                hpRosMsg.blue_4_hp = robotHealthMsgs.data.blue_4_robot_HP;
                                hpRosMsg.blue_5_hp = robotHealthMsgs.data.blue_5_robot_HP;
                                hpRosMsg.blue_7_hp = robotHealthMsgs.data.blue_7_robot_HP;
                                hpRosMsg.blue_base_hp = robotHealthMsgs.data.blue_outpose_HP;
                                hpRosMsg.red_base_hp = robotHealthMsgs.data.blue_base_HP;
                                hpPub->publish(hpRosMsg);
                                //std::cout << "Send one hp msg" << std::endl;
                            }
                            break;
                        case 0x020C:
                            memcpy(&radarMarkMsg, receive_data + pos, data_length + 9);
                            if(radarMarkMsg.crc == get_CRC16_check_sum((uint8_t *) &radarMarkMsg,(sizeof(radarMarkMsg) - sizeof(radarMarkMsg.crc)), 0xffff))
                            {
                                radarMarkRosMsg.mark_hero_progress = radarMarkMsg.data.mark_hero_progress;
                                radarMarkRosMsg.mark_engineer_progress = radarMarkMsg.data.mark_engineer_progress;
                                radarMarkRosMsg.mark_standard_3_progress = radarMarkMsg.data.mark_standard_3_progress;
                                radarMarkRosMsg.mark_standard_4_progress = radarMarkMsg.data.mark_standard_4_progress;
                                radarMarkRosMsg.mark_standard_5_progress = radarMarkMsg.data.mark_standard_5_progress;
                                radarMarkRosMsg.mark_sentry_progress = radarMarkMsg.data.mark_sentry_progress;
                                radarMarkPub->publish(radarMarkRosMsg);
                                //std::cout << "Send one radar mark msg" << std::endl;
                            }
                            break;
                        case 0x020E:
                            memcpy(&radarInfoMsg, receive_data + pos, data_length + 9);
                            if(radarInfoMsg.crc == get_CRC16_check_sum((uint8_t *) &radarInfoMsg,(sizeof(radarInfoMsg) - sizeof(radarInfoMsg.crc)), 0xffff))
                            {
                                radarInfoRosMsg.radar_info = radarInfoMsg.data.radar_info;
                                radarInfoRosMsg.is_double_damage = radarInfoMsg.data.is_double_damage;
                                radarInfoPub->publish(radarInfoRosMsg);
                                //std::cout << "Send one radar info msg" << std::endl;
                            }
                            break;
                        case 0x0105:
                            memcpy(&dartRemainingTimeMsg, receive_data + pos, data_length + 9);
                            if(dartRemainingTimeMsg.crc == get_CRC16_check_sum((uint8_t *) &dartRemainingTimeMsg,(sizeof(dartRemainingTimeMsg) - sizeof(dartRemainingTimeMsg.crc)), 0xffff))
                            {
                                dartRosMsg.dart_remaining_time = dartRemainingTimeMsg.data.dart_remaining_time;
                                dartRosMsg.dart_hit_target_info = dartRemainingTimeMsg.data.dart_hit_target_info;
                                dartRosMsg.dart_hit_target_total_info = dartRemainingTimeMsg.data.dart_hit_target_total_info;
                                dartRosMsg.dart_selected_target = dartRemainingTimeMsg.data.dart_selected_target;
                                dartPub->publish(dartRosMsg);
                                //std::cout << "Send one dart remaining time msg" << std::endl;
                            }
                        case 0x0101:
                            memcpy(&siteEventMsgs, receive_data + pos, data_length + 9);
                            if(siteEventMsgs.crc == get_CRC16_check_sum((uint8_t *) &siteEventMsgs,(sizeof(siteEventMsgs) - sizeof(siteEventMsgs.crc)), 0xffff))
                            {
                                siteEventRosMsg.supply_rune_small_status = siteEventMsgs.data.supply_rune_small_status;
                                siteEventRosMsg.supply_rune_big_status = siteEventMsgs.data.supply_rune_big_status;
                                siteEventPub->publish(siteEventRosMsg);
                                //std::cout << "Send one site event msg" << std::endl;
                            }
                        default:
                            break;
                        }
                    }
                }
                    pos++;
                }
        }
    }
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

void SerialDriver::colorCallback(const std_msgs::msg::Int8::SharedPtr msg)
{
    our_color = msg->data;
}

void SerialDriver::radarInfoCallback(const my_msgss::msg::Radarinfo msg)
{
    radarCmdMsg.head.SOF = 0xA5;
    radarCmdMsg.head.data_length = 1 + 6;
    radarCmdMsg.head.seq = seq;
    seq++;
    radarCmdMsg.head.crc = get_CRC8_check_sum((uint8_t *) &radarCmdMsg, (sizeof(radarCmdMsg.head) - sizeof(radarCmdMsg.head.crc)),
                                             0xff);
    radarCmdMsg.id = 0x0301;
    radarCmdMsg.cmd_id = 0x0121;
    if(our_color == 0) //我们是红方
    {
        radarCmdMsg.sender_ID = 9;
    }
    else if(our_color == 1) //我们是蓝方
    {
        radarCmdMsg.sender_ID = 109;
    }
    radarCmdMsg.receiver_ID = 0x8080;
    radarCmdMsg.data.radar_cmd = msg.radar_cmd;
    radarCmdMsg.crc = get_CRC16_check_sum((uint8_t *) &radarCmdMsg, (sizeof(radarCmdMsg) - sizeof(radarCmdMsg.crc)), 0xffff);
    serial_port.write((uint8_t *) &radarCmdMsg, sizeof(radarCmdMsg));
    std::cout << "Send one radar cmd msg radar_cmd = " << radarCmdMsg.data.radar_cmd << std::endl;
}

void SerialDriver::modeCallback(const std_msgs::msg::Int8 msg)
{
    if(msg.data == 0)
    {
        test = false;
    }
    else if(msg.data == 1)
    {
        test = true;
    }
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
}

//发送信息的函数
//-----------------------------------------------------------------------------------------
bool SerialDriver::sendPointsData()
{
    if(test)
    {
        pointMsg.head.SOF = 0xA5;
        pointMsg.head.data_length = 24;
        pointMsg.head.seq = seq;
        seq++;
        pointMsg.head.crc = get_CRC8_check_sum((uint8_t *) &pointMsg, (sizeof(pointMsg.head) - sizeof(pointMsg.head.crc)),
                                             0xff);
        pointMsg.cmd_id = 0x0305;
        pointMsg.data.hero_position_x = (int)(serialRobots[1].x * 100);
        pointMsg.data.hero_position_y = (int)(serialRobots[1].y * 100);
        pointMsg.data.engineer_position_x = 150;
        pointMsg.data.engineer_position_y = 240;
        pointMsg.data.infantry_3_position_x = 0;
        pointMsg.data.infantry_3_position_y = 0;
        pointMsg.data.infantry_4_position_x = (int)(serialRobots[4].x * 100);
        pointMsg.data.infantry_4_position_y = (int)(serialRobots[4].y * 100);
        pointMsg.data.infantry_5_position_x = (int)(serialRobots[5].x * 100);
        pointMsg.data.infantry_5_position_y = (int)(serialRobots[5].y * 100);
        pointMsg.data.sentry_position_x = (int)(serialRobots[6].x * 100);
        pointMsg.data.sentry_position_y = (int)(serialRobots[6].y * 100);
        pointMsg.crc = get_CRC16_check_sum((uint8_t *) &pointMsg, (sizeof(pointMsg) - sizeof(pointMsg.crc)), 0xffff);
        serial_port.write((uint8_t *) &pointMsg, sizeof(pointMsg));
        std::cout << "seq: " << seq << std::endl;
        std::cout <<  "hero_position x = " << pointMsg.data.hero_position_x << " y = " << pointMsg.data.hero_position_x << std::endl;
        std::cout <<  "engineer_position x = " << pointMsg.data.engineer_position_x << " y = " << pointMsg.data.engineer_position_y << std::endl;
        std::cout <<  "infantry_3_position x = " << pointMsg.data.infantry_3_position_x << " y = " << pointMsg.data.infantry_3_position_y << std::endl;
        std::cout <<  "infantry_4_position x = " << pointMsg.data.infantry_4_position_x << " y = " << pointMsg.data.infantry_4_position_y << std::endl;
        std::cout <<  "infantry_5_position x = " << pointMsg.data.infantry_5_position_x << " y = " << pointMsg.data.infantry_5_position_y << std::endl;
        std::cout <<  "sentry_position x = " << pointMsg.data.sentry_position_x << " y = " << pointMsg.data.sentry_position_y << std::endl;
        return false;
    }
    else
    {
        if(our_color == 0) //我们是红方
        {
        pointMsg.head.SOF = 0xA5;
        pointMsg.head.data_length = 24;
        pointMsg.head.seq = seq;
        seq++;
        pointMsg.head.crc = get_CRC8_check_sum((uint8_t *) &pointMsg, (sizeof(pointMsg.head) - sizeof(pointMsg.head.crc)),
                                             0xff);
        pointMsg.cmd_id = 0x0305;
        pointMsg.data.hero_position_x = (int)(serialRobots[1].x * 100);
        pointMsg.data.hero_position_y = (int)(serialRobots[1].y * 100);
        pointMsg.data.engineer_position_x = (int)(serialRobots[2].x * 100);
        pointMsg.data.engineer_position_y = (int)(serialRobots[2].y * 100);
        pointMsg.data.infantry_3_position_x = (int)(serialRobots[3].x * 100);
        pointMsg.data.infantry_3_position_y = (int)(serialRobots[3].y * 100);
        pointMsg.data.infantry_4_position_x = (int)(serialRobots[4].x * 100);
        pointMsg.data.infantry_4_position_y = (int)(serialRobots[4].y * 100);
        pointMsg.data.infantry_5_position_x = (int)(serialRobots[5].x * 100);
        pointMsg.data.infantry_5_position_y = (int)(serialRobots[5].y * 100);
        pointMsg.data.sentry_position_x = (int)(serialRobots[6].x * 100);
        pointMsg.data.sentry_position_y = (int)(serialRobots[6].y * 100);
        /*pointMsg.data.hero_position_x = 1;
        pointMsg.data.hero_position_y = 1;
        pointMsg.data.engineer_position_x = 1;
        pointMsg.data.engineer_position_y = 1;
        pointMsg.data.infantry_3_position_x = 1;
        pointMsg.data.infantry_3_position_y = 1;
        pointMsg.data.infantry_4_position_x = 1;
        pointMsg.data.infantry_4_position_y = 1;
        pointMsg.data.infantry_5_position_x = 1;
        pointMsg.data.infantry_5_position_y = 1;
        pointMsg.data.sentry_position_x = 1;
        pointMsg.data.sentry_position_y = 1;*/
        pointMsg.crc = get_CRC16_check_sum((uint8_t *) &pointMsg, (sizeof(pointMsg) - sizeof(pointMsg.crc)), 0xffff);
        serial_port.write((uint8_t *) &pointMsg, sizeof(pointMsg));
        std::cout << "seq: " << seq << std::endl;
        std::cout <<  "hero_position x = " << pointMsg.data.hero_position_x << " y = " << pointMsg.data.hero_position_x << std::endl;
        std::cout <<  "engineer_position x = " << pointMsg.data.engineer_position_x << " y = " << pointMsg.data.engineer_position_y << std::endl;
        std::cout <<  "infantry_3_position x = " << pointMsg.data.infantry_3_position_x << " y = " << pointMsg.data.infantry_3_position_y << std::endl;
        std::cout <<  "infantry_4_position x = " << pointMsg.data.infantry_4_position_x << " y = " << pointMsg.data.infantry_4_position_y << std::endl;
        std::cout <<  "infantry_5_position x = " << pointMsg.data.infantry_5_position_x << " y = " << pointMsg.data.infantry_5_position_y << std::endl;
        std::cout <<  "sentry_position x = " << pointMsg.data.sentry_position_x << " y = " << pointMsg.data.sentry_position_y << std::endl;
        }
        else if(our_color == 1) //我们是蓝方
        {
        pointMsg.head.SOF = 0xA5;
        pointMsg.head.data_length = 24;
        pointMsg.head.seq = seq;
        seq++;
        pointMsg.head.crc = get_CRC8_check_sum((uint8_t *) &pointMsg, (sizeof(pointMsg.head) - sizeof(pointMsg.head.crc)),
                                             0xff);
        pointMsg.cmd_id = 0x0305;
        pointMsg.data.hero_position_x =  (int)(serialRobots[7].x * 100);
        pointMsg.data.hero_position_y =  (int)(serialRobots[7].y * 100);
        pointMsg.data.engineer_position_x = (int)(serialRobots[8].x * 100);
        pointMsg.data.engineer_position_y = (int)(serialRobots[8].y * 100);
        pointMsg.data.infantry_3_position_x =  (int)(serialRobots[9].x * 100);
        pointMsg.data.infantry_3_position_y =  (int)(serialRobots[9].y * 100);
        pointMsg.data.infantry_4_position_x = (int)(serialRobots[10].x * 100);
        pointMsg.data.infantry_4_position_y =(int)(serialRobots[10].y * 100);
        pointMsg.data.infantry_5_position_x =  (int)(serialRobots[11].x * 100);
        pointMsg.data.infantry_5_position_y = (int)(serialRobots[11].y * 100);
        pointMsg.data.sentry_position_x =  (int)(serialRobots[12].x * 100);
        pointMsg.data.sentry_position_y =  (int)(serialRobots[12].y * 100);
        /*pointMsg.data.hero_position_x = 1;
        pointMsg.data.hero_position_y = 1;
        pointMsg.data.engineer_position_x = 1;
        pointMsg.data.engineer_position_y = 1;
        pointMsg.data.infantry_3_position_x = 1;
        pointMsg.data.infantry_3_position_y = 1;
        pointMsg.data.infantry_4_position_x = 1;
        pointMsg.data.infantry_4_position_y = 1;
        pointMsg.data.infantry_5_position_x = 1;
        pointMsg.data.infantry_5_position_y = 1;
        pointMsg.data.sentry_position_x = 1;
        pointMsg.data.sentry_position_y = 1;*/
        pointMsg.crc = get_CRC16_check_sum((uint8_t *) &pointMsg, (sizeof(pointMsg) - sizeof(pointMsg.crc)), 0xffff);
        serial_port.write((uint8_t *) &pointMsg, sizeof(pointMsg));
        std::cout << "seq: " << seq << std::endl;
        std::cout <<  "hero_position x = " << pointMsg.data.hero_position_x << " y = " << pointMsg.data.hero_position_x << std::endl;
        std::cout <<  "engineer_position x = " << pointMsg.data.engineer_position_x << " y = " << pointMsg.data.engineer_position_y << std::endl;
        std::cout <<  "infantry_3_position x = " << pointMsg.data.infantry_3_position_x << " y = " << pointMsg.data.infantry_3_position_y << std::endl;
        std::cout <<  "infantry_4_position x = " << pointMsg.data.infantry_4_position_x << " y = " << pointMsg.data.infantry_4_position_y << std::endl;
        std::cout <<  "infantry_5_position x = " << pointMsg.data.infantry_5_position_x << " y = " << pointMsg.data.infantry_5_position_y << std::endl;
        std::cout <<  "sentry_position x = " << pointMsg.data.sentry_position_x << " y = " << pointMsg.data.sentry_position_y << std::endl;
        }
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