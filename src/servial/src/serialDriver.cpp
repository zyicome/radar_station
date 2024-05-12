#include "serialDriver.hpp"

SerialDriver::SerialDriver() : Node("serial")
{
    serial_init();

    robots_init();

    our_color = 0;

    worldPointsSub = this->create_subscription<my_msgss::msg::Points>("/serial/world_points", 10, std::bind(&SerialDriver::worldPointsCallback, this, std::placeholders::_1));

    color_sub = this->create_subscription<std_msgs::msg::Int8>("/our_color", 10, std::bind(&SerialDriver::colorCallback, this, std::placeholders::_1));

    gameStatePub = this->create_publisher<my_msgss::msg::Gamestate>("/game_state", 10);

    worldPointsPub = this->create_publisher<my_msgss::msg::Points>("/test_world_points", 10);

    send_timer = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&SerialDriver::serialCommunication, this));

    receive_timer = this->create_wall_timer(std::chrono::milliseconds(10), std::bind(&SerialDriver::receiveCommunication, this));
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
            gameResultMsg = (*(game_result_msg *) receiveData);
            robotHealthMsgs = (*(robot_health_msgs *) receiveData);
            siteEventMsgs = (*(site_event_msgs *) receiveData);
            supplyProjectileActionMsg = (*(supply_projectile_action_msg *) receiveData);
            refereeWarningMsg = (*(referee_warning_msg *) receiveData);
            dartRemainingTimeMsg = (*(dart_remaining_time_msg *) receiveData);

            uint8_t crc8_check = get_CRC8_check_sum((uint8_t *) &gameStatusMsgs,(sizeof(gameStatusMsgs.head) -sizeof(gameStatusMsgs.head.crc)), 0xff);
            uint16_t crc16_check = get_CRC16_check_sum((uint8_t *) &gameStatusMsgs,(sizeof(gameStatusMsgs) - sizeof(gameStatusMsgs.crc)), 0xffff);
            uint8_t hpcrc8_check = get_CRC8_check_sum((uint8_t *) &robotHealthMsgs,
                                                                (sizeof(robotHealthMsgs.head) -
                                                                 sizeof(robotHealthMsgs.head.crc)), 0xff);
               uint16_t hpcrc16_check = get_CRC16_check_sum((uint8_t *) &robotHealthMsgs,
                                                            (sizeof(robotHealthMsgs) - sizeof(robotHealthMsgs.crc)),
                                                            0xffff);

            std::cout << "sizeof(gameStatusMsgs.crc)" << sizeof(gameStatusMsgs.crc) <<std::endl;
            
            std::cout << "gameStatusMsgs.head.crc = " << (int)gameStatusMsgs.head.crc << std::endl;
            std::cout << "gameStatusMsgs.crc = " << gameStatusMsgs.crc << std::endl;
            std::cout << "crc8_check = " << (int)crc8_check << std::endl;
            std::cout << "crc16_check = " << crc16_check << std::endl;
            std::cout << "robotshp head crc = " << (int)robotHealthMsgs.head.crc << std::endl;
            std::cout << "robotshp crc = " << robotHealthMsgs.crc << std::endl;
            std::cout << "hpcrc8_check = " << (int)hpcrc8_check << std::endl;
            std::cout << "hpcrc16_check = " << hpcrc16_check << std::endl;
            std::cout << "gameStatusMsgs.data.game_progress = " << gameStatusMsgs.data.game_progress << std::endl;
            std::cout << "gameStatusMsgs.data.game_type = " << gameStatusMsgs.data.game_type << std::endl;
            std::cout << "gameStatusMsgs.data.stage_remain_time = " << gameStatusMsgs.data.stage_remain_time << std::endl;


            gameStateRosMsg.dart_remaining_time = 16;
            gameStateRosMsg.winner = 3;
            gameStateRosMsg.is_receive_game_message = false;

            if ((gameStatusMsgs.head.crc == get_CRC8_check_sum((uint8_t *) &gameStatusMsgs,
                                                               (sizeof(gameStatusMsgs.head) -
                                                                sizeof(gameStatusMsgs.head.crc)), 0xff)) &&
                (gameStatusMsgs.crc ==
                 get_CRC16_check_sum((uint8_t *) &gameStatusMsgs, (sizeof(gameStatusMsgs) - sizeof(gameStatusMsgs.crc) ),
                                     0xffff))) {
                std::cout << "receive gameStatusMsgs" << std::endl;
                std::cout << "receive gameStatusMsgs" << std::endl;
                std::cout << "receive gameStatusMsgs" << std::endl;
                std::cout << "receive gameStatusMsgs" << std::endl;
                std::cout << "receive gameStatusMsgs++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
                gameStateRosMsg.is_receive_game_message = true;
                gameStateRosMsg.game_progress = gameStatusMsgs.data.game_progress;
                gameStateRosMsg.game_type = gameStatusMsgs.data.game_type;
                gameStateRosMsg.stage_remain_time = gameStatusMsgs.data.stage_remain_time;
                if_pub = true;
            }
            if ((gameResultMsg.head.crc == get_CRC8_check_sum((uint8_t *) &gameResultMsg, (sizeof(gameResultMsg.head) -
                                                                                           sizeof(gameResultMsg.head.crc)),
                                                              0xff)) && (gameResultMsg.crc ==
                                                                         get_CRC16_check_sum((uint8_t *) &gameResultMsg,
                                                                                             (sizeof(gameResultMsg) -
                                                                                              sizeof(gameResultMsg.crc)),
                                                                                             0xffff))) {
                std::cout << "receive gameResultMsg" << std::endl;
                gameStateRosMsg.winner = gameResultMsg.data.winner;
                if_pub = true;
            }
            if ((robotHealthMsgs.head.crc == get_CRC8_check_sum((uint8_t *) &robotHealthMsgs,
                                                                (sizeof(robotHealthMsgs.head) -
                                                                 sizeof(robotHealthMsgs.head.crc)), 0xff)) &&
                (robotHealthMsgs.crc == get_CRC16_check_sum((uint8_t *) &robotHealthMsgs,
                                                            (sizeof(robotHealthMsgs) - sizeof(robotHealthMsgs.crc)),
                                                            0xffff))) {
                std::cout << "receive robotHealthMsgs" << std::endl;
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
            if ((siteEventMsgs.head.crc == get_CRC8_check_sum((uint8_t *) &siteEventMsgs, (sizeof(siteEventMsgs.head) -
                                                                                           sizeof(siteEventMsgs.head.crc)),
                                                              0xff)) && (siteEventMsgs.crc ==
                                                                         get_CRC16_check_sum((uint8_t *) &siteEventMsgs,
                                                                                             (sizeof(siteEventMsgs) -
                                                                                              sizeof(siteEventMsgs.crc)),
                                                                                             0xffff))) {
                std::cout << "receive siteEventMsgs" << std::endl;
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
                std::cout << "receive supplyProjectileActionMsg" << std::endl;
                supplyProjectileActionRosMsg.reserved = supplyProjectileActionMsg.data.reserved;
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
                std::cout << "receive refereeWarningMsg" << std::endl;
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
                std::cout << "receive dartRemainingTimeMsg" << std::endl;
                gameStateRosMsg.dart_remaining_time = dartRemainingTimeMsg.data.dart_remaining_time;
                if_pub = true;
            }
            if (if_pub) {
                std::cout << "publish game state" << std::endl;
                gameStatePub->publish(gameStateRosMsg);
                return true;
            }
            return false;
        }
        return false;
}

void SerialDriver::receiveAllData_two(volatile uint8_t *databuffer,uint8_t length)
{
    uint8_t pos=0;
    uint16_t data_length=0;
    uint16_t CMD_ID =0;
    
     while(pos<length)
     {
        //std::cout << "databuffer[" << pos << "] = " << databuffer[pos] << std::endl;
        if(databuffer[pos]==0xA5)
        {
            std::cout << "pos:   " << pos << std::endl;
            std::cout << "receive 0xA5" << std::endl;
            //if(verify_CRC8_check_sum(&databuffer[pos],5))
            {
                data_length = (databuffer[pos+1]&0xff)|((databuffer[pos+2]<<8)&0xff00);
                std::cout << "data_length = " << data_length << std::endl;
                /*if(pos+data_length+9>length)
                {
                    continue;
                }*/
            //if(verify_CRC16_check_sum(&databuffer[pos],data_length+9))
            {
              
             
                CMD_ID = (databuffer[pos+5]&0xff)|((databuffer[pos+6]<<8)&0xff00);
                std::cout << "CMD_ID = " << CMD_ID << std::endl;
                switch(CMD_ID)
                { 
                    case 0x0001:
                        std::cout << "receive game_status_msg" << std::endl;
                        data_length = 11;
                        memcpy((void*)(&gameStatusData), (const void*)(&databuffer[pos+7]), data_length);
                        std::cout << "gameStatusData.game_progress = " << gameStatusData.game_progress << std::endl;
                        std::cout << "gameStatusData.game_type = " << gameStatusData.game_type << std::endl;
                        std::cout << "gameStatusData.stage_remain_time = " << gameStatusData.stage_remain_time << std::endl;
                        break;
                    default:break;
                }
                pos+=(data_length+9);
                continue;

            }


          }

        }

        pos++;
     
     }
}

void SerialDriver::receiveAllData_three()
{
    bool is_head = false;
    uint8_t pos[1];
    uint8_t frame_head[7];
    uint16_t CMD_ID = 0;
    uint16_t data_length = 0;
    uint8_t data[1024];
    uint8_t receive_data[1024];
    int pos1 = 0;
    bool if_pub = false;

    if(serial_port.available())
    {
        while(1)
        {
        serial_port.read(receive_data, 1024);
        pos1 = 0;
        while(pos1<1024)
        {
        if(receive_data[pos1] == 0xA5)
        {
            for(int i = 0;i<7;i++)
            {
                frame_head[i] = receive_data[pos1+i];
            }
            if(frame_head[4] == get_CRC8_check_sum((uint8_t *) &gameStatusMsgs,
                                                               (sizeof(gameStatusMsgs.head) -
                                                                sizeof(gameStatusMsgs.head.crc)), 0xff))
                                                                {
                                                                    std::cout << "CRC8 check success" << std::endl;
                                                                }
            
            /*std::cout << "get_CRC8_check_sum = " << (int)get_CRC8_check_sum((uint8_t *) &gameStatusMsgs,
                                                               (sizeof(gameStatusMsgs.head) -
                                                                sizeof(gameStatusMsgs.head.crc)), 0xff) << std::endl;*/
            data_length = (int)frame_head[1] + (int)frame_head[2]*256;
            CMD_ID = (frame_head[5]&0xff)|((frame_head[6]<<8)&0xff00);
            switch (CMD_ID)
            {
            case 0x0001:
                memcpy(&gameStatusMsgs, receive_data + pos1, data_length + 9);
                if(gameStatusMsgs.head.crc == get_CRC8_check_sum((uint8_t *) &gameStatusMsgs,(sizeof(gameStatusMsgs.head) -sizeof(gameStatusMsgs.head.crc)), 0xff))
               {
                 std::cout << "CRC8 check success" << std::endl;
               }
               if(gameStatusMsgs.crc == get_CRC16_check_sum((uint8_t *) &gameStatusMsgs,(sizeof(gameStatusMsgs) - sizeof(gameStatusMsgs.crc)), 0xffff))
               {
                    std::cout << "CRC16 check success" << std::endl;
               }
                //memcpy(&gameStatusData, receive_data + 7 + pos1, data_length);
                gameStateRosMsg.is_receive_game_message = true;
                gameStateRosMsg.game_progress = gameStatusMsgs.data.game_progress;
                gameStateRosMsg.game_type = gameStatusMsgs.data.game_type;
                gameStateRosMsg.stage_remain_time = gameStatusMsgs.data.stage_remain_time;
                if_pub = true;
                break;
            case 0x0002:
                break;
            case 0x0003:
                memcpy(&robotHealthData, receive_data + 7 + pos1, data_length);
                break;

            default:
                break;
            }
        }
        pos1++;
        }
        if(if_pub)
        {
            gameStatePub->publish(gameStateRosMsg);
            if_pub = false;
        }
        }
    }
}

void SerialDriver::receiveAllData_four(uint8_t *buff)
{
    int LEN_HEADER = 5;
    int LEN_CMDID = 2;
    int LEN_TAIL = 2;
    int DATA_Offset = 7;
    int CMD_ID_Offset = 5;
    int LEN_CRC8 = 4;
    int SOF = 0;
    int DATA_LENGTH = 1;
    int SEQ= 3;
    int CRC8 = 4;
    int LEN_game_state =11;
    uint8_t REFEREE_SOF = 0xA5;

    uint16_t CmdID;

	uint16_t judge_length; // 统计一帧数据长度
	if (buff == NULL)	   // 空数据包，则不作任何处理
		return;

	// 写入帧头数据(5-byte),用于判断是否开始存储裁判数据
	memcpy(&frameHeader, buff, LEN_HEADER);

    std::cout << "frameHeader.SOF = " << frameHeader.SOF << std::endl;

	// 判断帧头数据(0)是否为0xA5
	if (buff[SOF] == REFEREE_SOF)
	{
		// 帧头CRC8校验
		if (verify_CRC8_check_sum(buff, LEN_HEADER) == true)
		{
			// 统计一帧数据长度(byte),用于CR16校验
			judge_length = buff[DATA_LENGTH] + LEN_HEADER + LEN_CMDID + LEN_TAIL;
			// 帧尾CRC16校验
			if (verify_CRC16_check_sum(buff, judge_length) == true)
			{
				// 2个8位拼成16位int
				CmdID = (buff[6] << 8 | buff[5]);
				// 解析数据命令码,将数据拷贝到相应结构体中(注意拷贝数据的长度)
				// 第8个字节开始才是数据 data=7
				switch (CmdID)
				{
				case 0x0001: // 0x0001
                std::cout << "receive game_status_msg" << std::endl;
					memcpy(&gameStatusData, (buff + DATA_Offset), LEN_game_state);
                    std::cout << "gameStatusData.game_progress = " << gameStatusData.game_progress << std::endl;
                    std::cout << "gameStatusData.game_type = " << gameStatusData.game_type << std::endl;
                    std::cout << "gameStatusData.stage_remain_time = " << gameStatusData.stage_remain_time << std::endl;
					break;
				/*case ID_game_result: // 0x0002
					memcpy(&referee_info.GameResult, (buff + DATA_Offset), LEN_game_result);
					break;
				case ID_game_robot_survivors: // 0x0003
					memcpy(&referee_info.GameRobotHP, (buff + DATA_Offset), LEN_game_robot_HP);
					break;
				case ID_event_data: // 0x0101
					memcpy(&referee_info.EventData, (buff + DATA_Offset), LEN_event_data);
					break;
				case ID_supply_projectile_action: // 0x0102
					memcpy(&referee_info.SupplyProjectileAction, (buff + DATA_Offset), LEN_supply_projectile_action);
					break;
				case ID_game_warning: // 0x0104
					memcpy(&referee_info.GameWarning, (buff + DATA_Offset), LEN_game_warning);
					break;
				case ID_dart_info: // 0x0105
					memcpy(&referee_info.DartInfo, (buff + DATA_Offset), LEN_dart_info);
					break;
				case ID_game_robot_state: // 0x0201
					memcpy(&referee_info.GameRobotState, (buff + DATA_Offset), LEN_game_robot_state);
					break;
				case ID_power_heat_data: // 0x0202
					memcpy(&referee_info.PowerHeatData, (buff + DATA_Offset), LEN_power_heat_data);
					break;
				case ID_game_robot_pos: // 0x0203
					memcpy(&referee_info.GameRobotPos, (buff + DATA_Offset), LEN_game_robot_pos);
					break;
				case ID_buff_musk: // 0x0204
					memcpy(&referee_info.BuffMusk, (buff + DATA_Offset), LEN_buff_musk);
					break;
				case ID_aerial_robot_energy: // 0x0205
					memcpy(&referee_info.AerialRobotEnergy, (buff + DATA_Offset), LEN_aerial_robot_energy);
					break;
				case ID_robot_hurt: // 0x0206
					memcpy(&referee_info.RobotHurt, (buff + DATA_Offset), LEN_robot_hurt);
					break;
				case ID_shoot_data: // 0x0207
					memcpy(&referee_info.ShootData, (buff + DATA_Offset), LEN_shoot_data);
					break;
				case ID_projectile_allowance: // 0x0208
					memcpy(&referee_info.ProjectileAllowance, (buff + DATA_Offset), LEN_projectile_allowance);
					break;
				case ID_rfid_status: // 0x0209
					memcpy(&referee_info.RFIDStatus, (buff + DATA_Offset), LEN_rfid_status);
					break;
				// @todo 本demo未添加哨兵、飞镖、雷达的裁判系统数据解析
				case ID_student_interactive: // 0x0301   syhtodo接收代码未测试
					memcpy(&referee_info.ReceiveData, (buff + DATA_Offset), LEN_receive_data);
					break;
				}*/
			}
		}
		// 首地址加帧长度,指向CRC16下一字节,用来判断是否为0xA5,从而判断一个数据包是否有多帧数据
		if (*(buff + sizeof(frameHeader) + LEN_CMDID + frameHeader.data_length + LEN_TAIL) == 0xA5)
		{ // 如果一个数据包出现了多帧数据,则再次调用解析函数,直到所有数据包解析完毕
			receiveAllData_four(buff + sizeof(frameHeader) + LEN_CMDID + frameHeader.data_length + LEN_TAIL);
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
    //sendPointsData();

    //receiveAllData();
}

void SerialDriver::receiveCommunication()
{
    bool is_first =true;
    if(is_first == true)
    {
    receiveAllData_three();
    is_first = false;
    }
    //receiveAllData();
    /*uin8_t first[1024];
    uin8_t second[1024];
    int a = 0;
    bool is_first = false;
    while(serial_port.available())
    {
        uint8_t receiveData[1024];
        serial_port.read(receiveData, 1);
        fisrt[a] = receiveData[0];
        if(first[a] == 0xA5 && is_first == 0)
        {

        }
        a++;
        receiveAllData_four(receiveData);
    }
    std::cout << "stop to receive" << std::endl;*/
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
        my_msgss::msg::Points test_robots;
        my_msgss::msg::Point test_robot;
        test_robot.id = best_robot.id;
        test_robot.x = best_robot.x;
        test_robot.y = best_robot.y;
        test_robots.data.push_back(test_robot);
        worldPointsPub->publish(test_robots);
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