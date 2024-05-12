// MindVision Camera SDK
#include "CameraDefine.h"
#include <CameraApi.h>

// ROS
#include <camera_info_manager/camera_info_manager.hpp>
#include <cstdint>
#include <image_transport/image_transport.hpp>
#include <rcl_interfaces/msg/detail/set_parameters_result__struct.hpp>
#include <rclcpp/logging.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/detail/camera_info__struct.hpp>
#include <sensor_msgs/msg/image.hpp>

// C++ system
#include <memory>
#include <string>
#include <sys/types.h>
#include <thread>
#include <vector>

namespace mindvision_camera
{
class MVCameraNode : public rclcpp::Node
{
public:
  explicit MVCameraNode(const rclcpp::NodeOptions & options) : Node("mv_camera", options)
  {
    RCLCPP_INFO(this->get_logger(), "Starting MVCameraNode!");

    CameraSdkInit(1);

    // 枚举设备，并建立设备列表
    int i_camera_counts = 2;
    int i_status = 0;
    int i_status1 = 0;
    int i_status2 = 0;
    tSdkCameraDevInfo t_camera_enum_list[2];
    i_status = CameraEnumerateDevice(t_camera_enum_list, &i_camera_counts);
    //CameraEnumerateDevice(t_Camera_enum_list,&i_Camera_Counts);
    RCLCPP_INFO(this->get_logger(), "Enumerate state = %d", i_status);
    RCLCPP_INFO(this->get_logger(), "Found camera count = %d", i_camera_counts);

    // 没有连接设备
    if (i_camera_counts == 0) {
      RCLCPP_ERROR(this->get_logger(), "No camera found!");
      return;
    }

    // 相机初始化。初始化成功后，才能调用任何其他相机相关的操作接口
    i_status1 = CameraInit(&t_camera_enum_list[0], -1, -1, &h_leftcamera_);
    i_status2 = CameraInit(&t_camera_enum_list[1], -1, -1, &h_rightcamera_);

    // 初始化失 败
    RCLCPP_INFO(this->get_logger(), "Init state = %d", i_status1);
    if (i_status1 != CAMERA_STATUS_SUCCESS) {
      RCLCPP_ERROR(this->get_logger(), "leftInit failed!");
      return;
    }
    RCLCPP_INFO(this->get_logger(), "Init state = %d", i_status2);
    if (i_status2 != CAMERA_STATUS_SUCCESS) {
      RCLCPP_ERROR(this->get_logger(), "rightInit failed!");
      return;
    }

    // 获得相机的特性描述结构体。该结构体中包含了相机可设置的各种参数的范围信息。决定了相关函数的参数
    CameraGetCapability(h_leftcamera_, &t_leftcapability_);
    CameraGetCapability(h_rightcamera_, &t_rightcapability_);

    // 直接使用vector的内存作为相机输出buffer
    leftimage_msg_.data.reserve(
      t_leftcapability_.sResolutionRange.iHeightMax * t_leftcapability_.sResolutionRange.iWidthMax * 3);
    rightimage_msg_.data.reserve(
      t_rightcapability_.sResolutionRange.iHeightMax * t_rightcapability_.sResolutionRange.iWidthMax * 3);

    // 设置手动曝光
    CameraSetAeState(h_leftcamera_, false);
    CameraSetExposureTime(h_rightcamera_, false);
    CameraSetAeState(h_rightcamera_, false);
    CameraSetExposureTime(h_leftcamera_, false);

    // Declare camera parameters
    declareleftParameters();
    declarerightParameters();

    // 让SDK进入工作模式，开始接收来自相机发送的图像
    // 数据。如果当前相机是触发模式，则需要接收到
    // 触发帧以后才会更新图像。
    CameraPlay(h_leftcamera_);
    CameraPlay(h_rightcamera_);

    CameraSetIspOutFormat(h_leftcamera_, CAMERA_MEDIA_TYPE_RGB8);
    CameraSetIspOutFormat(h_rightcamera_, CAMERA_MEDIA_TYPE_RGB8);

    // Create camera publisher
    // rqt_image_view can't subscribe image msg with sensor_data QoS
    // https://github.com/ros-visualization/rqt/issues/187
    bool use_sensor_data_qos = this->declare_parameter("use_sensor_data_qos", false);
    auto qos = use_sensor_data_qos ? rmw_qos_profile_sensor_data : rmw_qos_profile_default;
    leftcamera_pub_ = image_transport::create_camera_publisher(this, "image_far", qos);
    rightcamera_pub_ = image_transport::create_camera_publisher(this, "image_close", qos);

    // Load camera info
    leftcamera_name_ = this->declare_parameter("leftcamera_name", "leftcamera");
    rightcamera_name_ = this->declare_parameter("rightcamera_name", "rightcamera");
    leftcamera_info_manager_ =
      std::make_unique<camera_info_manager::CameraInfoManager>(this, leftcamera_name_);
    rightcamera_info_manager_ =
      std::make_unique<camera_info_manager::CameraInfoManager>(this, rightcamera_name_);
    auto leftcamera_info_url = this->declare_parameter(
      "leftcamera_info_url", "package://mindvision_camera/config/leftcamera_info.yaml");
    if (leftcamera_info_manager_->validateURL(leftcamera_info_url)) {
      leftcamera_info_manager_->loadCameraInfo(leftcamera_info_url);
      leftcamera_info_msg_ = leftcamera_info_manager_->getCameraInfo();
    } else {
      RCLCPP_WARN(this->get_logger(), "Invalid leftcamera info URL: %s", leftcamera_info_url.c_str());
    }
    auto rightcamera_info_url = this->declare_parameter(
      "rightcamera_info_url", "package://mindvision_camera/config/rightcamera_info.yaml");
    if (rightcamera_info_manager_->validateURL(rightcamera_info_url)) {
        rightcamera_info_manager_->loadCameraInfo(rightcamera_info_url);
        rightcamera_info_msg_ = rightcamera_info_manager_->getCameraInfo();
        } 
    else {
        RCLCPP_WARN(this->get_logger(), "Invalid rightcamera info URL: %s", rightcamera_info_url.c_str());
    }

    // Add callback to the set parameter event
    /*leftparams_callback_handle_ = this->add_on_set_parameters_callback(
      std::bind(&MVCameraNode::leftparametersCallback, this, std::placeholders::_1));
    rightparams_callback_handle_ = this->add_on_set_parameters_callback(
      std::bind(&MVCameraNode::rightparametersCallback, this, std::placeholders::_1));*/
    params_callback_handle_ = this->add_on_set_parameters_callback(
      std::bind(&MVCameraNode::parametersCallback, this, std::placeholders::_1));

    leftcapture_thread_ = std::thread{[this]() -> void {
      RCLCPP_INFO(this->get_logger(), "Publishing leftimage!");

      leftimage_msg_.header.frame_id = "leftcamera_optical_frame";
      leftimage_msg_.encoding = "rgb8";

      while (rclcpp::ok()) {
        int status1 = CameraGetImageBuffer(h_leftcamera_, &lefts_frame_info_, &leftpby_buffer_, 1000);
        if (status1 == CAMERA_STATUS_SUCCESS) {
          CameraImageProcess(h_leftcamera_, leftpby_buffer_, leftimage_msg_.data.data(), &lefts_frame_info_);
          if (leftflip_image_) {
            CameraFlipFrameBuffer(leftimage_msg_.data.data(), &lefts_frame_info_, 3);
          }
          leftcamera_info_msg_.header.stamp = leftimage_msg_.header.stamp = this->now();
          leftimage_msg_.height = lefts_frame_info_.iHeight;
          leftimage_msg_.width = lefts_frame_info_.iWidth;
          leftimage_msg_.step = lefts_frame_info_.iWidth * 3;
          leftimage_msg_.data.resize(lefts_frame_info_.iWidth * lefts_frame_info_.iHeight * 3);

          leftcamera_pub_.publish(leftimage_msg_, leftcamera_info_msg_);

          // 在成功调用CameraGetImageBuffer后，必须调用CameraReleaseImageBuffer来释放获得的buffer。
          // 否则再次调用CameraGetImageBuffer时，程序将被挂起一直阻塞，
          // 直到其他线程中调用CameraReleaseImageBuffer来释放了buffer
          CameraReleaseImageBuffer(h_leftcamera_, leftpby_buffer_);
          leftfail_conut_ = 0;
        } else {
          RCLCPP_WARN(this->get_logger(), "Failed to get leftimage buffer, status1 = %d", status1);
          leftfail_conut_++;
        }

        if (leftfail_conut_ > 5) {
          RCLCPP_FATAL(this->get_logger(), "Failed to get leftimage buffer, exit!");
          rclcpp::shutdown();
        }
      }
    }};

    rightcapture_thread_ = std::thread{[this]() -> void {
      RCLCPP_INFO(this->get_logger(), "Publishing rightimage!");

      rightimage_msg_.header.frame_id = "rightcamera_optical_frame";
      rightimage_msg_.encoding = "rgb8";

      while (rclcpp::ok()) {
        int status2 = CameraGetImageBuffer(h_rightcamera_, &rights_frame_info_, &rightpby_buffer_, 1000);
        if (status2 == CAMERA_STATUS_SUCCESS) {
          CameraImageProcess(h_rightcamera_, rightpby_buffer_, rightimage_msg_.data.data(), &rights_frame_info_);
          if (rightflip_image_) {
            CameraFlipFrameBuffer(rightimage_msg_.data.data(), &rights_frame_info_, 3);
          }
          rightcamera_info_msg_.header.stamp = rightimage_msg_.header.stamp = this->now();
          rightimage_msg_.height = rights_frame_info_.iHeight;
          rightimage_msg_.width = rights_frame_info_.iWidth;
          rightimage_msg_.step = rights_frame_info_.iWidth * 3;
          rightimage_msg_.data.resize(rights_frame_info_.iWidth * rights_frame_info_.iHeight * 3);

          rightcamera_pub_.publish(rightimage_msg_, rightcamera_info_msg_);

          // 在成功调用CameraGetImageBuffer后，必须调用CameraReleaseImageBuffer来释放获得的buffer。
          // 否则再次调用CameraGetImageBuffer时，程序将被挂起一直阻塞，
          // 直到其他线程中调用CameraReleaseImageBuffer来释放了buffer
          CameraReleaseImageBuffer(h_rightcamera_, rightpby_buffer_);
          rightfail_conut_ = 0;
        } else {
          RCLCPP_WARN(this->get_logger(), "Failed to get rightimage buffer, status2 = %d", status2);
          rightfail_conut_++;
        }

        if (rightfail_conut_ > 5) {
          RCLCPP_FATAL(this->get_logger(), "Failed to get rightimage buffer, exit!");
          rclcpp::shutdown();
        }
      }
    }};
  }

~MVCameraNode() override
  {
    if (leftcapture_thread_.joinable()) {
      leftcapture_thread_.join();
    }

    if(rightcapture_thread_.joinable()) {
      rightcapture_thread_.join();
    }

    CameraUnInit(h_leftcamera_);
    CameraUnInit(h_rightcamera_);

    RCLCPP_INFO(this->get_logger(), "Camera node destroyed!");
  }

private:
  void declareleftParameters()
  {
    rcl_interfaces::msg::ParameterDescriptor leftparam_desc;
    leftparam_desc.integer_range.resize(1);
    leftparam_desc.integer_range[0].step = 1;

    // Exposure time
    leftparam_desc.description = "Exposure time in microseconds";
    // 对于CMOS传感器，其曝光的单位是按照行来计算的
    double leftexposure_line_time;
    CameraGetExposureLineTime(h_leftcamera_, &leftexposure_line_time);
    leftparam_desc.integer_range[0].from_value =
      t_leftcapability_.sExposeDesc.uiExposeTimeMin * leftexposure_line_time;
    leftparam_desc.integer_range[0].to_value =
      t_leftcapability_.sExposeDesc.uiExposeTimeMax * leftexposure_line_time;
    double leftexposure_time = this->declare_parameter("leftexposure_time", 20000, leftparam_desc);
    CameraSetExposureTime(h_leftcamera_, leftexposure_time);
    RCLCPP_INFO(this->get_logger(), "leftExposure time = %f", leftexposure_time);

    // Analog gain
    leftparam_desc.description = "leftAnalog gain";
    leftparam_desc.integer_range[0].from_value = t_leftcapability_.sExposeDesc.uiAnalogGainMin;
    leftparam_desc.integer_range[0].to_value = t_leftcapability_.sExposeDesc.uiAnalogGainMax;
    int leftanalog_gain;
    CameraGetAnalogGain(h_leftcamera_, &leftanalog_gain);
    leftanalog_gain = this->declare_parameter("leftanalog_gain", 64.0, leftparam_desc);
    CameraSetAnalogGain(h_leftcamera_, leftanalog_gain);
    RCLCPP_INFO(this->get_logger(), "leftAnalog gain = %d", leftanalog_gain);

    // RGB Gain
    // Get default value
    CameraGetGain(h_leftcamera_, &leftr_gain_, &leftg_gain_, &leftb_gain_);
    // R Gain
    leftparam_desc.integer_range[0].from_value = t_leftcapability_.sRgbGainRange.iRGainMin;
    leftparam_desc.integer_range[0].to_value = t_leftcapability_.sRgbGainRange.iRGainMax;
    leftr_gain_ = this->declare_parameter("leftrgb_gain.r", leftr_gain_, leftparam_desc);
    // G Gain
    leftparam_desc.integer_range[0].from_value = t_leftcapability_.sRgbGainRange.iGGainMin;
    leftparam_desc.integer_range[0].to_value = t_leftcapability_.sRgbGainRange.iGGainMax;
    leftg_gain_ = this->declare_parameter("leftrgb_gain.g", leftg_gain_, leftparam_desc);
    // B Gain
    leftparam_desc.integer_range[0].from_value = t_leftcapability_.sRgbGainRange.iBGainMin;
    leftparam_desc.integer_range[0].to_value = t_leftcapability_.sRgbGainRange.iBGainMax;
    leftb_gain_ = this->declare_parameter("leftrgb_gain.b", leftb_gain_, leftparam_desc);
    // Set gain
    CameraSetGain(h_leftcamera_, leftr_gain_, leftg_gain_, leftb_gain_);
    RCLCPP_INFO(this->get_logger(), "leftRGB Gain: R = %d", leftr_gain_);
    RCLCPP_INFO(this->get_logger(), "leftRGB Gain: G = %d", leftg_gain_);
    RCLCPP_INFO(this->get_logger(), "leftRGB Gain: B = %d", leftb_gain_);

    // Saturation
    leftparam_desc.description = "Saturation";
    leftparam_desc.integer_range[0].from_value = t_leftcapability_.sSaturationRange.iMin;
    leftparam_desc.integer_range[0].to_value = t_leftcapability_.sSaturationRange.iMax;
    int leftsaturation;
    CameraGetSaturation(h_leftcamera_, &leftsaturation);
    leftsaturation = this->declare_parameter("leftsaturation", leftsaturation, leftparam_desc);
    CameraSetSaturation(h_leftcamera_, leftsaturation);
    RCLCPP_INFO(this->get_logger(), "leftSaturation = %d", leftsaturation);

    // Gamma
    leftparam_desc.integer_range[0].from_value = t_leftcapability_.sGammaRange.iMin;
    leftparam_desc.integer_range[0].to_value = t_leftcapability_.sGammaRange.iMax;
    int leftgamma;
    CameraGetGamma(h_leftcamera_, &leftgamma);
    leftgamma = this->declare_parameter("leftgamma", leftgamma, leftparam_desc);
    CameraSetGamma(h_leftcamera_, leftgamma);
    RCLCPP_INFO(this->get_logger(), "leftGamma = %d", leftgamma);

    // Flip
    leftflip_image_ = this->declare_parameter("leftflip_image", false);
  }

  void declarerightParameters()
    {
        rcl_interfaces::msg::ParameterDescriptor rightparam_desc;
        rightparam_desc.integer_range.resize(1);
        rightparam_desc.integer_range[0].step = 1;
    
        // Exposure time
        rightparam_desc.description = "Exposure time in microseconds";
        // 对于CMOS传感器，其曝光的单位是按照行来计算的
        double rightexposure_line_time;
        CameraGetExposureLineTime(h_rightcamera_, &rightexposure_line_time);
        rightparam_desc.integer_range[0].from_value =
        t_rightcapability_.sExposeDesc.uiExposeTimeMin * rightexposure_line_time;
        rightparam_desc.integer_range[0].to_value =
        t_rightcapability_.sExposeDesc.uiExposeTimeMax * rightexposure_line_time;
        double rightexposure_time = this->declare_parameter("rightexposure_time", 20000, rightparam_desc);
        CameraSetExposureTime(h_rightcamera_, rightexposure_time);
        RCLCPP_INFO(this->get_logger(), "rightExposure time = %f", rightexposure_time);
    
        // Analog gain
        rightparam_desc.description = "rightAnalog gain";
        rightparam_desc.integer_range[0].from_value = t_rightcapability_.sExposeDesc.uiAnalogGainMin;
        rightparam_desc.integer_range[0].to_value = t_rightcapability_.sExposeDesc.uiAnalogGainMax;
        int rightanalog_gain;
        CameraGetAnalogGain(h_rightcamera_, &rightanalog_gain);
        rightanalog_gain = this->declare_parameter("rightanalog_gain", 64.0, rightparam_desc);
        CameraSetAnalogGain(h_rightcamera_, rightanalog_gain);
        RCLCPP_INFO(this->get_logger(), "rightAnalog gain = %d", rightanalog_gain);
    
        // RGB Gain
        // Get default value
        CameraGetGain(h_rightcamera_, &rightr_gain_, &rightg_gain_, &rightb_gain_);
        // R Gain
        rightparam_desc.integer_range[0].from_value = t_rightcapability_.sRgbGainRange.iRGainMin;
        rightparam_desc.integer_range[0].to_value = t_rightcapability_.sRgbGainRange.iRGainMax;
        rightr_gain_ = this->declare_parameter("rightrgb_gain.r", rightr_gain_, rightparam_desc);
        // G Gain
        rightparam_desc.integer_range[0].from_value = t_rightcapability_.sRgbGainRange.iGGainMin;
        rightparam_desc.integer_range[0].to_value = t_rightcapability_.sRgbGainRange.iGGainMax;
        rightg_gain_ = this->declare_parameter("rightrgb_gain.g", rightg_gain_, rightparam_desc);
        // B Gain
        rightparam_desc.integer_range[0].from_value = t_rightcapability_.sRgbGainRange.iBGainMin;
        rightparam_desc.integer_range[0].to_value = t_rightcapability_.sRgbGainRange.iBGainMax;
        rightb_gain_ = this->declare_parameter("rightrgb_gain.b", rightb_gain_, rightparam_desc);
        // Set gain
        CameraSetGain(h_rightcamera_, rightr_gain_, rightg_gain_, rightb_gain_);
        RCLCPP_INFO(this->get_logger(), "rightRGB Gain: R = %d", rightr_gain_);
        RCLCPP_INFO(this->get_logger(), "rightRGB Gain: G = %d", rightg_gain_);
        RCLCPP_INFO(this->get_logger(), "rightRGB Gain: B = %d", rightb_gain_);

        // Saturation
        rightparam_desc.description = "Saturation";
        rightparam_desc.integer_range[0].from_value = t_rightcapability_.sSaturationRange.iMin;
        rightparam_desc.integer_range[0].to_value = t_rightcapability_.sSaturationRange.iMax;
        int rightsaturation;
        CameraGetSaturation(h_rightcamera_, &rightsaturation);
        rightsaturation = this->declare_parameter("rightsaturation", rightsaturation, rightparam_desc);
        CameraSetSaturation(h_rightcamera_, rightsaturation);
        RCLCPP_INFO(this->get_logger(), "rightSaturation = %d", rightsaturation);

        // Gamma
        rightparam_desc.integer_range[0].from_value = t_rightcapability_.sGammaRange.iMin;
        rightparam_desc.integer_range[0].to_value = t_rightcapability_.sGammaRange.iMax;
        int rightgamma;
        CameraGetGamma(h_rightcamera_, &rightgamma);
        rightgamma = this->declare_parameter("rightgamma", rightgamma, rightparam_desc);
        CameraSetGamma(h_rightcamera_, rightgamma);
        RCLCPP_INFO(this->get_logger(), "rightGamma = %d", rightgamma);
        
        // Flip
        rightflip_image_ = this->declare_parameter("rightflip_image", false);
    }

  rcl_interfaces::msg::SetParametersResult parametersCallback(const std::vector<rclcpp::Parameter> & parameters)
  {
    rcl_interfaces::msg::SetParametersResult result;
    result.successful = true;
    for (const auto & param : parameters) {
      if (param.get_name() == "rightexposure_time") {
        int status = CameraSetExposureTime(h_rightcamera_, param.as_int());
        if (status != CAMERA_STATUS_SUCCESS) {
          result.successful = false;
          result.reason = "Failed to set exposure time, status = " + std::to_string(status);
        }
      } else if (param.get_name() == "rightanalog_gain") {
        int status = CameraSetAnalogGain(h_rightcamera_, param.as_int());
        if (status != CAMERA_STATUS_SUCCESS) {
          result.successful = false;
          result.reason = "Failed to set analog gain, status = " + std::to_string(status);
        }
      } else if (param.get_name() == "rightgamma") {
        int gamma = param.as_int();
        int status = CameraSetGamma(h_rightcamera_, gamma);
        if (status != CAMERA_STATUS_SUCCESS) {
          result.successful = false;
          result.reason = "Failed to set Gamma, status = " + std::to_string(status);
        }
      } else if (param.get_name() == "leftexposure_time") {
        int status = CameraSetExposureTime(h_leftcamera_, param.as_int());
        if (status != CAMERA_STATUS_SUCCESS) {
          result.successful = false;
          result.reason = "Failed to set exposure time, status = " + std::to_string(status);
        }
      } else if (param.get_name() == "leftanalog_gain") {
        int status = CameraSetAnalogGain(h_leftcamera_, param.as_int());
        if (status != CAMERA_STATUS_SUCCESS) {
          result.successful = false;
          result.reason = "Failed to set analog gain, status = " + std::to_string(status);
        }
      } else if (param.get_name() == "leftgamma") {
        int gamma = param.as_int();
        int status = CameraSetGamma(h_leftcamera_, gamma);
        if (status != CAMERA_STATUS_SUCCESS) {
          result.successful = false;
          result.reason = "Failed to set Gamma, status = " + std::to_string(status);
        }
      } else {
        result.successful = false;
        result.reason = "Unknown parameter: " + param.get_name();
      }
    }
    return result;
  }

  int h_leftcamera_;
  int h_rightcamera_;
  uint8_t * leftpby_buffer_;
  uint8_t * rightpby_buffer_;
  tSdkCameraCapbility t_leftcapability_;  // 设备描述信息
  tSdkCameraCapbility t_rightcapability_;  // 设备描述信息  
  tSdkFrameHead lefts_frame_info_;        // 图像帧头信息
  tSdkFrameHead rights_frame_info_;        // 图像帧头信息

  sensor_msgs::msg::Image leftimage_msg_;
  sensor_msgs::msg::Image rightimage_msg_;

  image_transport::CameraPublisher leftcamera_pub_;
  image_transport::CameraPublisher rightcamera_pub_;

  // RGB Gain
  int leftr_gain_, leftg_gain_, leftb_gain_;
  int rightr_gain_, rightg_gain_, rightb_gain_;

  bool leftflip_image_;
  bool rightflip_image_;

  std::string leftcamera_name_;
  std::string rightcamera_name_;
  std::unique_ptr<camera_info_manager::CameraInfoManager> leftcamera_info_manager_;
  std::unique_ptr<camera_info_manager::CameraInfoManager> rightcamera_info_manager_;
  sensor_msgs::msg::CameraInfo leftcamera_info_msg_;
  sensor_msgs::msg::CameraInfo rightcamera_info_msg_;

  int leftfail_conut_ = 0;
  int rightfail_conut_ = 0;
  std::thread leftcapture_thread_;
  std::thread rightcapture_thread_;

  /*OnSetParametersCallbackHandle::SharedPtr leftparams_callback_handle_;
  OnSetParametersCallbackHandle::SharedPtr rightparams_callback_handle_;*/
  OnSetParametersCallbackHandle::SharedPtr params_callback_handle_;
};

}  // namespace mindvision_camera

#include "rclcpp_components/register_node_macro.hpp"

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(mindvision_camera::MVCameraNode)