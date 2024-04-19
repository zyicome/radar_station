from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, Command
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    video_save = Node(
        package='Video_save',
        executable='video_save',
        name='video_save',
        output='screen'
    )
    return LaunchDescription([video_save])