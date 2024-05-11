#include "judge.hpp"

Judge::Judge() : rclcpp::Node("judge")
{
    robots_init();

    our_color = 0;
    // Initialize the publisher
    radar_mark_pub = this->create_publisher<my_msgss::msg::Radarmark>(
        "/radar_mark", 10);

    // Initialize the subscriber
    worldpoints_sub = this->create_subscription<my_msgss::msg::Points>(
        "/test_world_points", 10, std::bind(&Judge::worldPointsCallback, this, std::placeholders::_1));

    color_sub = this->create_subscription<std_msgs::msg::Int8>("/our_color", 10, std::bind(&Judge::colorCallback, this, std::placeholders::_1));

    send_timer = this->create_wall_timer(std::chrono::milliseconds(1000), std::bind(&Judge::sendRadarMarkProgress, this));
}

void Judge::colorCallback(const std_msgs::msg::Int8::SharedPtr msg)
{
    our_color = msg->data;
}

void Judge::worldPointsCallback(const my_msgss::msg::Points::SharedPtr msg)
{
    /*std::cout << "msg->data.size()" << msg->data.size() << std::endl;
    std::cout << "msg->data[0]" << msg->data[0].id << std::endl;*/
    if(msg->data.size() == 0)
    {
        return;
    }
    int armor_num = 0;
    if(our_color ==0)
    {
        armor_num = msg->data[0].id;
    }
    else
    {
        armor_num = msg->data[0].id - 6;
    }
    for(int i =1;i<robots.size();i++)
    {
        if(robots[i].radar_mark_progress != 0.0 && i != armor_num)
        {
            robots[i].last_radar_mark_progress = robots[i].radar_mark_progress;
            robots[i].radar_mark_progress = robots[i].radar_mark_progress - 0.8 + robots[i].x;
            if(robots[i].radar_mark_progress <= 0.0)
            {
                robots[i].radar_mark_progress = 0.0;
            }
        }
        else if(i == armor_num)
        {
            robots[armor_num].last_radar_mark_progress = robots[armor_num].radar_mark_progress;
            robots[armor_num].radar_mark_progress = robots[armor_num].radar_mark_progress + 1.0 + robots[armor_num].x;
            if(robots[armor_num].radar_mark_progress >= 120.0)
            {
                robots[armor_num].radar_mark_progress = 120.0;
            }
        }
        robots[i].x = robots[i].radar_mark_progress - robots[i].last_radar_mark_progress;
    }
}

void Judge::sendRadarMarkProgress()
{
    my_msgss::msg::Radarmark msg;
    msg.mark_hero_progress = robots[1].radar_mark_progress;
    msg.mark_engineer_progress = robots[2].radar_mark_progress;
    msg.mark_standard_3_progress = robots[3].radar_mark_progress;
    msg.mark_standard_4_progress = robots[4].radar_mark_progress;
    msg.mark_standard_5_progress = robots[5].radar_mark_progress;
    msg.mark_sentry_progress = robots[6].radar_mark_progress;
    radar_mark_pub->publish(msg);
}

void Judge::robots_init()
{
    robot robot;
    robot.id = -1;
    robot.radar_mark_progress = 0.0;
    robot.last_radar_mark_progress = 0.0;
    robot.x = 0;
    for (int i = 0; i < 7; i++)
    {
        robot.id++;
        robots.push_back(robot);
    }
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Judge>());
    rclcpp::shutdown();
    return 0;
}
