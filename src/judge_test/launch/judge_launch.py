from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    judge = Node(
        package='judge_test',
        executable='judge_test',
        name='judge_test',
        output='screen'
    )
    return LaunchDescription([judge])