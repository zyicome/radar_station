echo "Starting colcon build"
colcon build
echo "Colcon build finished"

cmds=(  "ros2 launch mindvision_camera mv_camera.py"
	      #"ros2 launch hik_camera hik_camera.launch.py"
        "ros2 launch Img_Handle img_handle_launch.py"
        "ros2 launch get_depth get_depth_launch.py"
        "ros2 launch Qt5_displayer qt5_display_launch.py"
        "ros2 launch map_test map_test_launch.py"
        #"ros2 launch my_msgss radar_station_launch.py"
  	    "ros2 launch foxglove_bridge foxglove_bridge_launch.xml port:=8765"
        )

cmd=("ros2 launch livox_ros2_driver livox_lidar_launch.py")

for cmd in "${cmd[@]}";
do
    gnome-terminal -- bash -c "cd ~/livox_ros2_driver;source install/setup.bash;$cmd;exec bash;"
    sleep 0.2
done

for cmd in "${cmds[@]}"; 
do
    echo Current CMD : "$cmd"
  	gnome-terminal -- bash -c "cd $(pwd);source install/setup.bash;$cmd;exec bash;"
    sleep 0.2
done

echo "Script finished"
