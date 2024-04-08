import os

from ament_index_python.packages import get_package_share_directory, get_package_prefix
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    params_file = os.path.join(
        get_package_share_directory('hik_camera'), 'config', 'camera_params.yaml')

    left_camera_info_url = 'package://hik_camera/config/left_camera_info.yaml'
    right_camera_info_url = 'package://hik_camera/config/right_camera_info.yaml'

    return LaunchDescription([
        DeclareLaunchArgument(name='params_file',
                              default_value=params_file),
        DeclareLaunchArgument(name='left_camera_info_url',
                              default_value=left_camera_info_url),
        DeclareLaunchArgument(name='right_camera_info_url',
                              default_value=right_camera_info_url),
        DeclareLaunchArgument(name='left_use_sensor_data_qos',
                              default_value='false'),
        DeclareLaunchArgument(name='right_use_sensor_data_qos',
                              default_value='false'),

        Node(
            package='hik_camera',
            executable='hik_camera_node',
            output='screen',
            emulate_tty=True,
            parameters=[LaunchConfiguration('params_file'), {
                'left_camera_info_url': LaunchConfiguration('left_camera_info_url'),
                'right_camera_info_url': LaunchConfiguration('right_camera_info_url'),
                'left_use_sensor_data_qos': LaunchConfiguration('left_use_sensor_data_qos'),
                'right_use_sensor_data_qos': LaunchConfiguration('right_use_sensor_data_qos'),
            }],
        )
    ])
