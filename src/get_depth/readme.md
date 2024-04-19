雷达追踪机制说明:
tracking参数说明： "not"(识别到且为被雷达追踪) "tracking"(被相机锁定) “locking"(被雷达锁定) “tracking_locking”(同时被相机和雷达锁定，最高优先级)

固定机器人位置，通过比较来决定机器人的存在和去留: 顺序 蓝方 1 2 3 4 5 6 7 8 9 红方 10 11 12 13 14 15 16 17 18 

需调整参数说明：
far_camera_matrix
far_uni_matrix
far_distortion_coefficient
close_camera_matrix
close_uni_matrix
close_distortion_coefficient
times;
imgRows;
imgCols;
length_of_cloud_queue;//default length is 5
post_pub_flag;