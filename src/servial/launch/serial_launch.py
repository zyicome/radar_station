from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    serial = Node(
        package='serial_driver',
        executable='serial_driver',
        name='serial_driver',
        output='screen',
        respawn=True
    )
    return LaunchDescription([serial])
