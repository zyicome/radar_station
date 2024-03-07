from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, Command
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    # config = os.path.join(
    #     get_package_share_directory('my_msgss'),
    #     'config',
    #     'radar.yaml'
    # )
    # config_1 = os.path.join(
    #     get_package_share_directory('my_msgss'),
    #     'config',
    #     'ros2_displayer.yaml'
    # )
    img_handle = Node(
        package='Img_Handle',
        executable='Img_Sub',
        name='Img_Handle',
        output='screen'
    )
    get_depth =Node(
        package='get_depth',
        executable='get_depth_node',
        name='Get_Depth',
        output='screen'
    )
    map_test = Node(
        package='map_test',
        executable='map_test',
        name='map_test',
        output='screen'
    )
    # qt5_displayer = Node(
    #     package='Qt5_displayer',
    #     executable='Qt5_displayer',
    #     name='Qt5_display',
    #     output='screen'
    # )
    return LaunchDescription([img_handle, get_depth,map_test])


