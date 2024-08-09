from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, Command
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    map_test = Node(
        package='map_test',
        executable='map_test',
        name='map_test',
        output='screen'
    )
    return LaunchDescription([map_test])