from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    img_handle = Node(
        package='Img_Handle',
        executable='Img_Sub',
        name='Img_Handle',
        output='screen'
    )
    return LaunchDescription([img_handle])