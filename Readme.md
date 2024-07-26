基本信息介绍
文件
文件介绍
	test.sh // 测试用的脚本,现使用,直接启动便可启动所有需要的程序
	saveScreen.sh// 录像启动脚本
	launch.sh // 暂不用
	log       // 日志文件
	src
        	All_Topic // 所有的话题汇总
        	Game_Map // 比赛地图（暂不用,已舍弃）
        	get_depth  // 得到深度信息
        	Img_Handle // 处理图像，深度神经识别
        	judge_test // 裁判系统测试程序
        	map_test // 地图（现用）
        	my_msgss // 自定义消息类型
        	parameter_server // 参数管理程序，管理所有参数
        	Qt5_displayer // Qt界面可视化显示
        	ros2-hik-mutiple-camera // 启动相机,hikvision相机节点
        	ros2-mindvision-camera  // 启动相机,mindvision相机节点
        	servial // 串口程序，收发裁判系统
        	Video_save // 保存录像，方便后期回顾
 	.gitignore // git上传忽略的文件
 	
环境准备
这里假设你是一台装好了操作系统，其他啥环境都没有的电脑
系统环境：Ubuntu22.04
安装ros2
	鱼香ros一键安装：
	wget http://fishros.com/install -O fishros && . fishros
安装opencv4.8.0（请看看自己现在的时代的版本）
sudo apt-get install libopencv-dev(我暂时只能下到4.5.2的版本)
安装 pcl库（点云数据处理库）
	简单apt安装：
	sudo apt-get install libpcl-dev
	（未经测试，有报错请先自行查找原因）
安装雷达驱动
	驱动地址：
	https://github.com/zyicome/lidar_driver.git
	使用方法：
	mkdir livox_ros2_driver
	cd livox_ros2_driver
	git clone https://github.com/zyicome/lidar_driver.git
	colcon build
	//可能会出现报错，这时候就按照ctrl+左键对应到错误的地方，加上#include <memory>
	//测试:
	. install/setup.bash
	ros2 launch livox_ros2_driver livox_lidar_launch.py
	//能启动不报错一般就没什么问题
安装相机驱动
	文件地址：https://github.com/zyicome/ros2-hik-or-mindvision-camera-calibration.git
	直接git下来，一体化程序，既包含了海康同时含有迈德威视相机的驱动文档和标定程序，详见readme.md
安装Qt5.14.2
	百度网盘：https://pan.baidu.com/s/1gcti7boGQpxsd0mX7QsiZA 
	提取码:y8p3
	（学长精心辛苦准备的安装包😘🥺）
	网上找一下，建议安装5.14.2,后面版本安装比较麻烦。
安装serial库（用于串口通讯）
	库文件地址：
	https://github.com/zyicome/serial.git
请参考readme.md安装
	安装tinyxml2 (用于动态读取参数)
	库文件文件：
	https://github.com/leethomason/tinyxml2.git
	请参考readme.md安装
安装nvinfer (用于神经网络加速)
	请参考网上资料 -- NVIDIA
	
部署
	git clone -b radar_station_fixed_class https://github.com/zyicome/radar_station.git
	cd radar_station
	colcon build
	./test.sh
可能出现的问题：
	1：./test.sh: Permission denied
   	解决方法：找到test.sh文件，右键打开属性，进入permission，把execute打开即可
	2: test.sh文件的路径需要修改一下，根据雷达驱动的位置进行修改即可
