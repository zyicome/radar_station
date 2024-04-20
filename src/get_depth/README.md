   get_depth_node 在main函数中，获取了很多的参数，被我移出来了
   
   
   
   点云队列长度  
   图片宽度  
   图片高度  
    ros::param::get("/length_of_cloud_queue", length_of_cloud_queue);//点云队列长度
    ros::param::get("/image_width", imgCols);
    ros::param::get("/image_height", imgRows);
   //相机内参
    ros::param::get("/sensor_far/camera_matrix/zerozero", far_camera_matrix.at<float>(0, 0));
    ros::param::get("/sensor_far/camera_matrix/zerotwo", far_camera_matrix.at<float>(0, 2));
    ros::param::get("/sensor_far/camera_matrix/oneone", far_camera_matrix.at<float>(1, 1));
    ros::param::get("/sensor_far/camera_matrix/onetwo", far_camera_matrix.at<float>(1, 2));
    ros::param::get("/sensor_far/camera_matrix/twotwo", far_camera_matrix.at<float>(2, 2));
    far_camera_matrix.at<float>(0, 1) = 0;
    far_camera_matrix.at<float>(1, 0) = 0;
    far_camera_matrix.at<float>(2, 0) = 0;
    far_camera_matrix.at<float>(2, 1) = 0;
    cout << "far_Camera matrix load done!" << far_camera_matrix << endl;
    ros::param::get("/sensor_far/distortion_coefficient/zero", far_distortion_coefficient.at<float>(0, 0));
    ros::param::get("/sensor_far/distortion_coefficient/one", far_distortion_coefficient.at<float>(1, 0));
    ros::param::get("/sensor_far/distortion_coefficient/two", far_distortion_coefficient.at<float>(2, 0));
    ros::param::get("/sensor_far/distortion_coefficient/three", far_distortion_coefficient.at<float>(3, 0));
    ros::param::get("/sensor_far/distortion_coefficient/four", far_distortion_coefficient.at<float>(4, 0));
    cout << "far_Distortion coefficient load done!" << far_distortion_coefficient << endl;
    //相机外参默认值
    ros::param::get("/sensor_far/uni_matrix/zerozero", far_uni_matrix.at<float>(0, 0));
    ros::param::get("/sensor_far/uni_matrix/zeroone", far_uni_matrix.at<float>(0, 1));
    ros::param::get("/sensor_far/uni_matrix/zerotwo", far_uni_matrix.at<float>(0, 2));
    ros::param::get("/sensor_far/uni_matrix/zerothree", far_uni_matrix.at<float>(0, 3));
    ros::param::get("/sensor_far/uni_matrix/onezero", far_uni_matrix.at<float>(1, 0));
    ros::param::get("/sensor_far/uni_matrix/oneone", far_uni_matrix.at<float>(1, 1));
    ros::param::get("/sensor_far/uni_matrix/onetwo", far_uni_matrix.at<float>(1, 2));
    ros::param::get("/sensor_far/uni_matrix/onethree", far_uni_matrix.at<float>(1, 3));
    ros::param::get("/sensor_far/uni_matrix/twozero", far_uni_matrix.at<float>(2, 0));
    ros::param::get("/sensor_far/uni_matrix/twoone", far_uni_matrix.at<float>(2, 1));
    ros::param::get("/sensor_far/uni_matrix/twotwo", far_uni_matrix.at<float>(2, 2));
    ros::param::get("/sensor_far/uni_matrix/twothree", far_uni_matrix.at<float>(2, 3));
    cout << "far Uni matrix load done!" << far_uni_matrix << endl;

    //相机内参
    ros::param::get("/sensor_close/camera_matrix/zerozero", close_camera_matrix.at<float>(0, 0));
    ros::param::get("/sensor_close/camera_matrix/zerotwo", close_camera_matrix.at<float>(0, 2));
    ros::param::get("/sensor_close/camera_matrix/oneone", close_camera_matrix.at<float>(1, 1));
    ros::param::get("/sensor_close/camera_matrix/onetwo", close_camera_matrix.at<float>(1, 2));
    ros::param::get("/sensor_close/camera_matrix/twotwo", close_camera_matrix.at<float>(2, 2));
    close_camera_matrix.at<float>(0, 1) = 0;
    close_camera_matrix.at<float>(1, 0) = 0;
    close_camera_matrix.at<float>(2, 0) = 0;
    close_camera_matrix.at<float>(2, 1) = 0;
    cout << "close_Camera matrix load done!" << close_camera_matrix << endl;
    ros::param::get("/sensor_close/distortion_coefficient/zero", close_distortion_coefficient.at<float>(0, 0));
    ros::param::get("/sensor_close/distortion_coefficient/one", close_distortion_coefficient.at<float>(1, 0));
    ros::param::get("/sensor_close/distortion_coefficient/two", close_distortion_coefficient.at<float>(2, 0));
    ros::param::get("/sensor_close/distortion_coefficient/three", close_distortion_coefficient.at<float>(3, 0));
    ros::param::get("/sensor_close/distortion_coefficient/four", close_distortion_coefficient.at<float>(4, 0));
    cout << "close_Distortion coefficient load done!" << close_distortion_coefficient << endl;
    //相机外参默认值
    ros::param::get("/sensor_close/uni_matrix/zerozero", close_uni_matrix.at<float>(0, 0));
    ros::param::get("/sensor_close/uni_matrix/zeroone", close_uni_matrix.at<float>(0, 1));
    ros::param::get("/sensor_close/uni_matrix/zerotwo", close_uni_matrix.at<float>(0, 2));
    ros::param::get("/sensor_close/uni_matrix/zerothree", close_uni_matrix.at<float>(0, 3));
    ros::param::get("/sensor_close/uni_matrix/onezero", close_uni_matrix.at<float>(1, 0));
    ros::param::get("/sensor_close/uni_matrix/oneone", close_uni_matrix.at<float>(1, 1));
    ros::param::get("/sensor_close/uni_matrix/onetwo", close_uni_matrix.at<float>(1, 2));
    ros::param::get("/sensor_close/uni_matrix/onethree", close_uni_matrix.at<float>(1, 3));
    ros::param::get("/sensor_close/uni_matrix/twozero", close_uni_matrix.at<float>(2, 0));
    ros::param::get("/sensor_close/uni_matrix/twoone", close_uni_matrix.at<float>(2, 1));
    ros::param::get("/sensor_close/uni_matrix/twotwo", close_uni_matrix.at<float>(2, 2));
    ros::param::get("/sensor_close/uni_matrix/twothree", close_uni_matrix.at<float>(2, 3));
    cout << "close Uni matrix load done!" << close_uni_matrix << endl;