from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    qt5_displayer = Node(
        package='Qt5_displayer',
        executable='Qt5_displayer',
        name='Qt5_display',
        output='screen'
    )
    return LaunchDescription([qt5_displayer])