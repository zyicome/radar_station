import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    leftcamera_params = os.path.join(
        get_package_share_directory('mindvision_camera'),"config", "leftcamera_params.yaml"
    )

    rightcamera_params = os.path.join(
        get_package_share_directory('mindvision_camera'),"config", "rightcamera_params.yaml"
    )

    leftcamera_info_url = 'package://mindvision_camera/config/leftcamera_info.yaml'

    rightcamera_info_url = 'package://mindvision_camera/config/rightcamera_info.yaml'
    
    mv_launch =  Node(
        package='mindvision_camera',
        executable='mindvision_camera_node',
        name='mindvision_camera_node',
        output='screen',
        emulate_tty=True,
        parameters=[LaunchConfiguration('leftcamera_params'),
                    LaunchConfiguration('rightcamera_params'),{
                'leftcamera_info_url': LaunchConfiguration('leftcamera_info_url'),
                'rightcamera_info_url': LaunchConfiguration('rightcamera_info_url'),
                'use_sensor_data_qos': LaunchConfiguration('use_sensor_data_qos'),
        }],
    )

    return LaunchDescription([
        DeclareLaunchArgument(name='leftcamera_params',
                              default_value='leftcamera_params'),
        DeclareLaunchArgument(name='rightcamera_params',
                              default_value='rightcamera_params'),
        DeclareLaunchArgument(name='leftcamera_info_url',
                              default_value=leftcamera_info_url),
        DeclareLaunchArgument(name='rightcamera_info_url',
                              default_value=rightcamera_info_url),
        DeclareLaunchArgument(name='use_sensor_data_qos',
                              default_value='false'),
        mv_launch,
    ])