from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, Command
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    parameter_server_node = Node(
        package='parameter_server',
        executable='parameter_server',
        name='parameter_server',
        output='screen'
    )
    return LaunchDescription([parameter_server_node])