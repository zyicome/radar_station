from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os
import yaml

def generate_launch_description():
    img_handle_yaml = yaml.safe_load(open(os.path.join(get_package_share_directory('Img_Handle'), 'config', 'img_handle.yaml')))

    img_handle = Node(
        package='Img_Handle',
        executable='Img_Sub',
        name='Img_Handle',
        output='screen',
        parameters=[img_handle_yaml],
    )
    return LaunchDescription([img_handle])