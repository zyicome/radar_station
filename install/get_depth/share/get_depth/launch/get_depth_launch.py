from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    get_depth =Node(
        package='get_depth',
        executable='get_depth_node',
        name='Get_Depth',
        output='screen'
    )
    return LaunchDescription([get_depth])