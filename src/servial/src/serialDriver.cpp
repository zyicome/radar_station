#include "serialDriver.hpp"

SerialDriver::SerialDriver() : Node("serial")
{
    serial_init();

    robots_init();

    our_color = 0;

    test = false;

    is_first_create_ui = true;

    seq = 0;

    game_progress = 0;

    remaining_game_time = 0;

    energy_start_time = {360, 270, 180, 105, 30};

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
                                game_progress = gameStatusMsgs.data.game_progress;
                                remaining_game_time = gameStatusMsgs.data.stage_remain_time;
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
    //sendHeroData();
    sendRobotThreeData();
    sendUIData();
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

bool SerialDriver::sendHeroData()
{
    uint16_t sender_id;
    uint16_t receiver_id;
    // 将坐标保留两位小数，再乘于1000变为整数，再转化为uint8_t为高8位和低8位
    float our_hero_x = 0.0;
    float our_hero_y = 0.0;
    uint16_t our_hero_x_int = 0;
    uint16_t our_hero_y_int = 0;
    uint8_t user_data[112] = {0};
    if(our_color == 0) //我们是红方
    {
        sender_id = 9;
        receiver_id = 1;
        our_hero_x = serialRobots[7].x; // m
        our_hero_y = serialRobots[7].y;
    }
    else if(our_color == 1) //我们是蓝方
    {
        sender_id = 109;
        receiver_id = 101;
        our_hero_x = serialRobots[1].x;
        our_hero_y = serialRobots[1].y;
    }
    our_hero_x_int = (uint16_t)(our_hero_x * 1000); // mm
    our_hero_y_int = (uint16_t)(our_hero_y * 1000);

    if(test)
    {
        sender_id = 9;
        receiver_id = 1;
        our_hero_x_int = 800;
        our_hero_y_int = 700;
        user_data[0] = (our_hero_x_int >> 8) & 0xff;
        user_data[1] = our_hero_x_int & 0xff;
        user_data[2] = (our_hero_y_int >> 8) & 0xff;
        user_data[3] = our_hero_y_int & 0xff;
        sendRobotData(toHeroMsgs, sender_id, receiver_id, user_data, 112);
        return false;
    }
    else
    {
        user_data[0] = (our_hero_x_int >> 8) & 0xff;
        user_data[1] = our_hero_x_int & 0xff;
        user_data[2] = (our_hero_y_int >> 8) & 0xff;
        user_data[3] = our_hero_y_int & 0xff;
        sendRobotData(toHeroMsgs, sender_id, receiver_id, user_data, 112);
        return true;
    }
}

bool SerialDriver::sendRobotThreeData()
{
    uint16_t sender_id;
    uint16_t receiver_id;
    // 将坐标保留两位小数，再乘于1000变为整数，再转化为uint8_t为高8位和低8位
    float our_three_x = 0.0;
    float our_three_y = 0.0;
    uint16_t our_three_x_int = 0;
    uint16_t our_three_y_int = 0;
    uint8_t data_size = 4;
    uint8_t user_data[data_size] = {0};
    if(our_color == 0) //我们是红方
    {
        sender_id = 9;
        receiver_id = 3;
        our_three_x = serialRobots[9].x; // m
        our_three_y = serialRobots[9].y;
    }
    else if(our_color == 1) //我们是蓝方
    {
        sender_id = 109;
        receiver_id = 103;
        our_three_x = serialRobots[3].x;
        our_three_y = serialRobots[3].y;
    }
    our_three_x_int = (uint16_t)(our_three_x * 1000); // mm
    our_three_y_int = (uint16_t)(our_three_y * 1000);

    if(test)
    {
        sender_id = 9;
        receiver_id = 3;
        our_three_x_int = 800;
        our_three_y_int = 700;
        user_data[0] = (our_three_x_int >> 8) & 0xff;
        user_data[1] = our_three_x_int & 0xff;
        user_data[2] = (our_three_y_int >> 8) & 0xff;
        user_data[3] = our_three_y_int & 0xff;
        toThreeMsgs.head.SOF = 0xA5;
        toThreeMsgs.head.data_length = data_size + 6;
        toThreeMsgs.head.seq = seq;
        seq++;
        toThreeMsgs.head.crc = get_CRC8_check_sum((uint8_t *) &toThreeMsgs, (sizeof(toThreeMsgs.head) - sizeof(toThreeMsgs.head.crc)),
                                                0xff);
        toThreeMsgs.cmd_id = 0x0301;
        toThreeMsgs.data.data_cmd_id = 0x0201;
        toThreeMsgs.data.sender_id = sender_id;
        toThreeMsgs.data.receiver_id = receiver_id;
        std::copy(user_data, user_data + data_size, toThreeMsgs.data.user_data);
        toThreeMsgs.crc = get_CRC16_check_sum((uint8_t *) &toThreeMsgs, (sizeof(toThreeMsgs) - sizeof(toThreeMsgs.crc)), 0xffff);
        serial_port.write((uint8_t *) &toThreeMsgs, sizeof(toThreeMsgs));
        std::cout << "user_data[0]" << (int)toThreeMsgs.data.user_data[0] << std::endl;
        std::cout << "user_data[1]" << (int)toThreeMsgs.data.user_data[1] << std::endl;
        std::cout << "user_data[2]" << (int)toThreeMsgs.data.user_data[2] << std::endl;
        std::cout << "user_data[3]" << (int)toThreeMsgs.data.user_data[3] << std::endl;
        return false;
    }
    else
    {
        user_data[0] = (our_three_x_int >> 8) & 0xff;
        user_data[1] = our_three_x_int & 0xff;
        user_data[2] = (our_three_y_int >> 8) & 0xff;
        user_data[3] = our_three_y_int & 0xff;
        toThreeMsgs.head.SOF = 0xA5;
        toThreeMsgs.head.data_length = data_size + 6;
        toThreeMsgs.head.seq = seq;
        seq++;
        toThreeMsgs.head.crc = get_CRC8_check_sum((uint8_t *) &toThreeMsgs, (sizeof(toThreeMsgs.head) - sizeof(toThreeMsgs.head.crc)),
                                                0xff);
        toThreeMsgs.cmd_id = 0x0301;
        toThreeMsgs.data.data_cmd_id = 0x0201;
        toThreeMsgs.data.sender_id = sender_id;
        toThreeMsgs.data.receiver_id = receiver_id;
        std::copy(user_data, user_data + data_size, toThreeMsgs.data.user_data);
        toThreeMsgs.crc = get_CRC16_check_sum((uint8_t *) &toThreeMsgs, (sizeof(toThreeMsgs) - sizeof(toThreeMsgs.crc)), 0xffff);
        serial_port.write((uint8_t *) &toThreeMsgs, sizeof(toThreeMsgs));
        std::cout << "user_data[0]" << (int)toThreeMsgs.data.user_data[0] << std::endl;
        std::cout << "user_data[1]" << (int)toThreeMsgs.data.user_data[1] << std::endl;
        std::cout << "user_data[2]" << (int)toThreeMsgs.data.user_data[2] << std::endl;
        std::cout << "user_data[3]" << (int)toThreeMsgs.data.user_data[3] << std::endl;
        return true;
    }
}

bool SerialDriver::sendRobotData(robot_interaction_msgs &robot_interaction_msg, uint16_t sender_id, uint16_t receiver_id, const uint8_t *user_data, size_t data_size)
{
    robot_interaction_msg.head.SOF = 0xA5;
    robot_interaction_msg.head.data_length = data_size + 15;
    robot_interaction_msg.head.seq = seq;
    seq++;
    robot_interaction_msg.head.crc = get_CRC8_check_sum((uint8_t *) &robot_interaction_msg, (sizeof(robot_interaction_msg.head) - sizeof(robot_interaction_msg.head.crc)),
                                             0xff);
    robot_interaction_msg.cmd_id = 0x0301;
    robot_interaction_msg.data.data_cmd_id = 0x0200;
    robot_interaction_msg.data.sender_id = sender_id;
    robot_interaction_msg.data.receiver_id = receiver_id;
    std::copy(user_data, user_data + data_size, robot_interaction_msg.data.user_data);
    robot_interaction_msg.crc = get_CRC16_check_sum((uint8_t *) &robot_interaction_msg, (sizeof(robot_interaction_msg) - sizeof(robot_interaction_msg.crc)), 0xffff);
    serial_port.write((uint8_t *) &robot_interaction_msg, sizeof(robot_interaction_msg));
    std::cout << "user_data[0]" << (int)robot_interaction_msg.data.user_data[0] << std::endl;
    std::cout << "user_data[1]" << (int)robot_interaction_msg.data.user_data[1] << std::endl;
    std::cout << "user_data[2]" << (int)robot_interaction_msg.data.user_data[2] << std::endl;
    std::cout << "user_data[3]" << (int)robot_interaction_msg.data.user_data[3] << std::endl;
    return true;
}

void SerialDriver::sendUIData()
{
    if(is_first_create_ui)
    {
        createUI();
        is_first_create_ui = false;
        std::cout << "createUI -----------------------" << std::endl;
    }
    else
    {
        createUI();
        std::cout << "updateUI -----------------------" << std::endl;
    }
}

void SerialDriver::createUI()
{
    client_ui_msgs client_ui_msg;
    client_ui_msg.head.SOF = 0xA5;
    client_ui_msg.head.data_length = 60;
    client_ui_msg.head.seq = seq;
    seq++;
    client_ui_msg.head.crc = get_CRC8_check_sum((uint8_t *) &client_ui_msg, (sizeof(client_ui_msg.head) - sizeof(client_ui_msg.head.crc)),
                                             0xff);
    client_ui_msg.cmd_id = 0x0301;
    client_ui_msg.data.cmd_id = 0x0110;
    if(our_color == 0) //我们是红方
    {
        client_ui_msg.data.sender_id = 9;
        client_ui_msg.data.receiver_id = 0x0106;
    }
    else if(our_color == 1) //我们是蓝方
    {
        client_ui_msg.data.sender_id = 109;
        client_ui_msg.data.receiver_id = 0x016A;
    }

    // 创建一个字符，然后将字符转化为uint8_t类型的data[30]数据

    // 拼接字符串并拷贝到字符数组
    char energy_remaining_time[30];
    std::string energy_remaining_time_str = "能量机关剩余时间: 0";

    // 使用 strncpy 拷贝字符串，确保不超过数组大小
    std::strncpy(energy_remaining_time, energy_remaining_time_str.c_str(), sizeof(energy_remaining_time) - 1);

    // 确保字符数组以 '\0' 结尾
    energy_remaining_time[sizeof(energy_remaining_time) - 1] = '\0';

    // 获取实际字符串长度
    int size = std::strlen(energy_remaining_time);

    // 拷贝到 client_ui_msg.data.char_data
    std::memcpy(client_ui_msg.data.char_data, energy_remaining_time, size);

    // 如果需要，手动设置 client_ui_msg.data.char_data 的末尾为 '\0'
    client_ui_msg.data.char_data[size] = '\0';

    // 创建能力机关剩余时间UI
    client_ui_msg.data.graphic_data.graphic_name[0] = 1;
    client_ui_msg.data.graphic_data.graphic_name[1] = 1;
    client_ui_msg.data.graphic_data.graphic_name[2] = 1;
    client_ui_msg.data.graphic_data.operate_tpye = 1;
    client_ui_msg.data.graphic_data.graphic_tpye = 7;
    client_ui_msg.data.graphic_data.layer = 0;
    client_ui_msg.data.graphic_data.color = 1;
    client_ui_msg.data.graphic_data.details_a = 200;
    // 字符长度
    client_ui_msg.data.graphic_data.details_b = size;
    client_ui_msg.data.graphic_data.width = 20;
    client_ui_msg.data.graphic_data.start_x = 500;
    client_ui_msg.data.graphic_data.start_y = 500;
    client_ui_msg.data.graphic_data.details_c = 0;
    client_ui_msg.data.graphic_data.details_d = 0;
    client_ui_msg.data.graphic_data.details_e = 0;

    client_ui_msg.crc = get_CRC16_check_sum((uint8_t *) &client_ui_msg, (sizeof(client_ui_msg) - sizeof(client_ui_msg.crc)), 0xffff);
    serial_port.write((uint8_t *) &client_ui_msg, sizeof(client_ui_msg));
}

void SerialDriver::updateUI()
{
    client_ui_msgs client_ui_msg;
    client_ui_msg.head.SOF = 0xA5;
    client_ui_msg.head.data_length = 60;
    client_ui_msg.head.seq = seq;
    seq++;
    client_ui_msg.head.crc = get_CRC8_check_sum((uint8_t *) &client_ui_msg, (sizeof(client_ui_msg.head) - sizeof(client_ui_msg.head.crc)),
                                             0xff);
    client_ui_msg.cmd_id = 0x0301;
    client_ui_msg.data.cmd_id = 0x0110;
    if(our_color == 0) //我们是红方
    {
        client_ui_msg.data.sender_id = 9;
        client_ui_msg.data.receiver_id = 0x0106;
    }
    else if(our_color == 1) //我们是蓝方
    {
        client_ui_msg.data.sender_id = 109;
        client_ui_msg.data.receiver_id = 0x016A;
    }

    uint16_t bias_time = 0;
    uint16_t min_bias_time = 420;
    uint16_t remaining_time = 0;

    if(game_progress == 4 && remaining_game_time > 20)
    {
        for(size_t i = 0; i< energy_start_time.size(); i++)
        {
            bias_time = remaining_game_time - energy_start_time[i];
            if(bias_time >= 0 && bias_time < min_bias_time)
            {
                min_bias_time = bias_time;
                remaining_time = bias_time;
            }
        }
    }
    else
    {
        remaining_time = 0;
    }

    // 创建一个字符，然后将字符转化为uint8_t类型的data[30]数据

    // 拼接字符串并拷贝到字符数组
    char energy_remaining_time[30];
    std::string energy_remaining_time_str = "能量机关剩余时间: " + std::to_string(remaining_time);

    // 使用 strncpy 拷贝字符串，确保不超过数组大小
    std::strncpy(energy_remaining_time, energy_remaining_time_str.c_str(), sizeof(energy_remaining_time) - 1);

    // 确保字符数组以 '\0' 结尾
    energy_remaining_time[sizeof(energy_remaining_time) - 1] = '\0';

    // 获取实际字符串长度
    int size = std::strlen(energy_remaining_time);

    // 拷贝到 client_ui_msg.data.char_data
    std::memcpy(client_ui_msg.data.char_data, energy_remaining_time, size);

    // 如果需要，手动设置 client_ui_msg.data.char_data 的末尾为 '\0'
    client_ui_msg.data.char_data[size] = '\0';

    // 确保字符数组以 '\0' 结尾
    energy_remaining_time[energy_remaining_time_str.length()] = '\0';

    client_ui_msg.data.graphic_data.graphic_name[0] = 1;
    client_ui_msg.data.graphic_data.graphic_name[1] = 1;
    client_ui_msg.data.graphic_data.graphic_name[2] = 1;
    client_ui_msg.data.graphic_data.operate_tpye = 2;
    client_ui_msg.data.graphic_data.graphic_tpye = 7;
    client_ui_msg.data.graphic_data.layer = 0;
    client_ui_msg.data.graphic_data.color = 1;
    client_ui_msg.data.graphic_data.details_a = 200;
    client_ui_msg.data.graphic_data.details_b = size;
    client_ui_msg.data.graphic_data.width = 20;
    client_ui_msg.data.graphic_data.start_x = 500;
    client_ui_msg.data.graphic_data.start_y = 500;
    
    client_ui_msg.data.graphic_data.details_c = 0;
    client_ui_msg.data.graphic_data.details_d = 0;
    client_ui_msg.data.graphic_data.details_e = 0;

    client_ui_msg.crc = get_CRC16_check_sum((uint8_t *) &client_ui_msg, (sizeof(client_ui_msg) - sizeof(client_ui_msg.crc)), 0xffff);
    serial_port.write((uint8_t *) &client_ui_msg, sizeof(client_ui_msg));
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