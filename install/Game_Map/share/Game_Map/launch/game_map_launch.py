from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, Command
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    radar_yaml = os.path.join(
        get_package_share_directory('Game_Map'),
        'config',
        'radar.yaml'
    )
    ros2_displayer_yaml = os.path.join(
        get_package_share_directory('Game_Map'),
        'config',
        'ros2_displayer.yaml'
    )
    game_map = Node(
        package='Game_Map',
        executable='Game_Map',
        name='game_map',
        output='screen',
        parameters=[radar_yaml, ros2_displayer_yaml]
    )
    return LaunchDescription([game_map])