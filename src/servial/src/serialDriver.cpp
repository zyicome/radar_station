#include "serialDriver.hpp"

SerialDriver::SerialDriver() : Node("serial")
{
    serial_init();

    robots_init();

    our_color = 0;

    worldPointsSub = this->create_subscription<my_msgss::msg::Points>("/serial/world_points", 10, std::bind(&SerialDriver::worldPointsCallback, this, std::placeholders::_1));

    color_sub = this->create_subscription<std_msgs::msg::Int8>("/our_color", 10, std::bind(&SerialDriver::colorCallback, this, std::placeholders::_1));

    radarInfoSub = this->create_subscription<my_msgss::msg::Radarinfo>("/serial/radar_info", 10, std::bind(&SerialDriver::radarInfoCallback, this, std::placeholders::_1));

    gameStatePub = this->create_publisher<my_msgss::msg::Gamestate>("/game_state", 10);

    worldPointsPub = this->create_publisher<my_msgss::msg::Points>("/test_world_points", 10);

    radarMarkPub = this->create_publisher<my_msgss::msg::Radarmark>("/radar_mark", 10);

    hpPub = this->create_publisher<my_msgss::msg::Hp>("/hp", 10);

    radarInfoPub = this->create_publisher<my_msgss::msg::Radarinfo>("/radar_info", 10);

    send_timer = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&SerialDriver::serialCommunication, this));

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
                                radarInfoPub->publish(radarInfoRosMsg);
                                //std::cout << "Send one radar info msg" << std::endl;
                            }
                            break;
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
    radarCmdMsg.head.data_length = 1;
    radarCmdMsg.head.seq = 1;
    radarCmdMsg.head.crc = get_CRC8_check_sum((uint8_t *) &radarCmdMsg, (sizeof(radarCmdMsg.head) - sizeof(radarCmdMsg.head.crc)),
                                             0xff);
    radarCmdMsg.cmd_id = 0x0121;
    radarCmdMsg.data.radar_cmd = msg.radar_cmd;
    radarCmdMsg.crc = get_CRC16_check_sum((uint8_t *) &radarCmdMsg, (sizeof(radarCmdMsg) - sizeof(radarCmdMsg.crc)), 0xffff);
    serial_port.write((uint8_t *) &radarCmdMsg, sizeof(radarCmdMsg));
    std::cout << "Send one radar cmd msg radar_cmd = " << radarCmdMsg.data.radar_cmd << std::endl;
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
    /*for(int i = 0;i<serialRobots.size();i++)
    {
        std::cout << "serialRobots[" << i << "].x = " << serialRobots[i].x << " serialRobots[" << i << "].y = " << serialRobots[i].y << std::endl;
    }*/
    bool if_send = false;
    serialRobot best_robot;
    best_robot.confidence = 0.0;
    if(our_color == 0) // 我们是红方
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
    else if(our_color == 1) // 我们是蓝方
    {
        for(int i =7;i<serialRobots.size();i++)
        {
            if(serialRobots[i].x !=0.0 && serialRobots[i].y != 0.0)
            {
                if(serialRobots[i].confidence > best_robot.confidence)
                {
                    best_robot = serialRobots[i];
                    best_robot.id = i - 6;
                    if_send = true;
                }
            }
        }
    }
    bool test = false;
    if(test)
    {
    pointMsg.head.SOF = 0xA5;
    pointMsg.head.data_length = 10;
        pointMsg.head.seq = 1;
        pointMsg.head.crc = get_CRC8_check_sum((uint8_t *) &pointMsg, (sizeof(pointMsg.head) - sizeof(pointMsg.head.crc)),
                                             0xff);
        pointMsg.cmd_id = 0x0305;
        pointMsg.data.target_position_x = 0.0;
        pointMsg.data.target_position_y = 0.0;
        pointMsg.data.target_robot_id = 103;
        pointMsg.crc = get_CRC16_check_sum((uint8_t *) &pointMsg, (sizeof(pointMsg) - sizeof(pointMsg.crc)), 0xffff);
        serial_port.write((uint8_t *) &pointMsg, sizeof(pointMsg));
        /*std::cout << "Send one point msg target_id = " << pointMsg.data.target_robot_id << " x = "
             << pointMsg.data.target_position_x << " y = " << pointMsg.data.target_position_y << std::endl;*/
             return false;
    }
    if(if_send == false)
    {
        return false;
    }
    else
    {
        if(our_color == 0) //我们是红方
        {
        pointMsg.head.SOF = 0xA5;
        pointMsg.head.data_length = 10;
        pointMsg.head.seq = 1;
        pointMsg.head.crc = get_CRC8_check_sum((uint8_t *) &pointMsg, (sizeof(pointMsg.head) - sizeof(pointMsg.head.crc)),
                                             0xff);
        pointMsg.cmd_id = 0x0305;
        pointMsg.data.target_position_x = best_robot.x;
        pointMsg.data.target_position_y = best_robot.y;
        switch(best_robot.id)
        {
            case 1:
                pointMsg.data.target_robot_id = 101;
                break;
            case 2:
                pointMsg.data.target_robot_id = 102;
                break;
            case 3:
                pointMsg.data.target_robot_id = 103;
                break;
            case 4:
                pointMsg.data.target_robot_id = 104;
                break;
            case 5:
                pointMsg.data.target_robot_id = 105;
                break;
            case 6:
                pointMsg.data.target_robot_id = 107;
                break;
            default:
                break;
        }
        pointMsg.crc = get_CRC16_check_sum((uint8_t *) &pointMsg, (sizeof(pointMsg) - sizeof(pointMsg.crc)), 0xffff);
        serial_port.write((uint8_t *) &pointMsg, sizeof(pointMsg));
        std::cout << "Send one point msg target_id = " << pointMsg.data.target_robot_id << " x = "
             << pointMsg.data.target_position_x << " y = " << pointMsg.data.target_position_y << std::endl;
        }
        else if(our_color == 1) //我们是蓝方
        {
        pointMsg.head.SOF = 0xA5;
        pointMsg.head.data_length = 10;
        pointMsg.head.seq = 1;
        pointMsg.head.crc = get_CRC8_check_sum((uint8_t *) &pointMsg, (sizeof(pointMsg.head) - sizeof(pointMsg.head.crc)),
                                             0xff);
        pointMsg.cmd_id = 0x0305;
        pointMsg.data.target_position_x = best_robot.x;
        pointMsg.data.target_position_y = best_robot.y;
        if(best_robot.id != 6)
        {
            pointMsg.data.target_robot_id = best_robot.id;
        }
        else if(best_robot.id == 6)
        {
            pointMsg.data.target_robot_id = 7;
        }
        pointMsg.crc = get_CRC16_check_sum((uint8_t *) &pointMsg, (sizeof(pointMsg) - sizeof(pointMsg.crc)), 0xffff);
        serial_port.write((uint8_t *) &pointMsg, sizeof(pointMsg));
        std::cout << "Send one point msg target_id = " << pointMsg.data.target_robot_id << " x = "
             << pointMsg.data.target_position_x << " y = " << pointMsg.data.target_position_y << std::endl;
        /*my_msgss::msg::Points test_robots;
        my_msgss::msg::Point test_robot;
        test_robot.id = best_robot.id;
        test_robot.x = best_robot.x;
        test_robot.y = best_robot.y;
        test_robots.data.push_back(test_robot);
        worldPointsPub->publish(test_robots);*/
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