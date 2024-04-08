#include "MvCameraControl.h"
// ROS
#include <camera_info_manager/camera_info_manager.hpp>
#include <image_transport/image_transport.hpp>
#include <rclcpp/logging.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/utilities.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/image.hpp>

namespace hik_camera
{
class HikCameraNode : public rclcpp::Node
{
public:
  explicit HikCameraNode(const rclcpp::NodeOptions & options) : Node("hik_camera", options)
  {
    RCLCPP_INFO(this->get_logger(), "Starting HikCameraNode!");

    MV_CC_DEVICE_INFO_LIST device_list;
    // enum device
    nRet = MV_CC_EnumDevices(MV_USB_DEVICE, &device_list);
    RCLCPP_INFO(this->get_logger(), "Found camera count = %d", device_list.nDeviceNum);

    while (device_list.nDeviceNum == 0 && rclcpp::ok()) {
      RCLCPP_ERROR(this->get_logger(), "No camera found!");
      RCLCPP_INFO(this->get_logger(), "Enum state: [%x]", nRet);
      std::this_thread::sleep_for(std::chrono::seconds(1));
      nRet = MV_CC_EnumDevices(MV_USB_DEVICE, &device_list);
    }

    MV_CC_CreateHandle(&left_camera_handle_, device_list.pDeviceInfo[0]);
    MV_CC_CreateHandle(&right_camera_handle_, device_list.pDeviceInfo[1]);

    MV_CC_OpenDevice(left_camera_handle_);
    MV_CC_OpenDevice(right_camera_handle_);

    // Get left_camera infomation
    MV_CC_GetImageInfo(left_camera_handle_, &left_img_info_);
    left_image_msg_.data.reserve(left_img_info_.nHeightMax * left_img_info_.nWidthMax * 3);

    // Init convert param
    left_convert_param_.nWidth = left_img_info_.nWidthValue;
    left_convert_param_.nHeight = left_img_info_.nHeightValue;
    left_convert_param_.enDstPixelType = PixelType_Gvsp_RGB8_Packed;

    bool left_use_sensor_data_qos = this->declare_parameter("left_use_sensor_data_qos", true);
    auto qos = left_use_sensor_data_qos ? rmw_qos_profile_sensor_data : rmw_qos_profile_default;
    left_camera_pub_ = image_transport::create_camera_publisher(this, "image_far", qos);

    left_camera_declareParameters();

    MV_CC_StartGrabbing(left_camera_handle_);

    // Load camera info
    left_camera_name_ = this->declare_parameter("left_camera_name", "left_camera");
    left_camera_info_manager_ =
      std::make_unique<camera_info_manager::CameraInfoManager>(this, left_camera_name_);
    auto left_camera_info_url =
      this->declare_parameter("left_camera_info_url", "package://hik_camera/config/left_camera_info.yaml");
    if (left_camera_info_manager_->validateURL(left_camera_info_url)) {
      left_camera_info_manager_->loadCameraInfo(left_camera_info_url);
      left_camera_info_msg_ = left_camera_info_manager_->getCameraInfo();
    } else {
      RCLCPP_WARN(this->get_logger(), "Invalid camera info URL: %s", left_camera_info_url.c_str());
    }

    // Get right_camera infomation
    MV_CC_GetImageInfo(right_camera_handle_, &right_img_info_);
    right_image_msg_.data.reserve(right_img_info_.nHeightMax * right_img_info_.nWidthMax * 3);

    // Init convert param
    right_convert_param_.nWidth = right_img_info_.nWidthValue;
    right_convert_param_.nHeight = right_img_info_.nHeightValue;
    right_convert_param_.enDstPixelType = PixelType_Gvsp_RGB8_Packed;

    bool right_use_sensor_data_qos = this->declare_parameter("right_use_sensor_data_qos", true);
    qos = right_use_sensor_data_qos ? rmw_qos_profile_sensor_data : rmw_qos_profile_default;
    right_camera_pub_ = image_transport::create_camera_publisher(this, "image_close", qos);

    right_camera_declareParameters();

    MV_CC_StartGrabbing(right_camera_handle_);

    // Load camera info
    right_camera_name_ = this->declare_parameter("right_camera_name", "right_camera");
    right_camera_info_manager_ =
      std::make_unique<camera_info_manager::CameraInfoManager>(this, right_camera_name_);
    auto right_camera_info_url =
      this->declare_parameter("right_camera_info_url", "package://hik_camera/config/right_camera_info.yaml");
    if (right_camera_info_manager_->validateURL(right_camera_info_url)) {
      right_camera_info_manager_->loadCameraInfo(right_camera_info_url);
      right_camera_info_msg_ = right_camera_info_manager_->getCameraInfo();
    } else {
      RCLCPP_WARN(this->get_logger(), "Invalid camera info URL: %s", right_camera_info_url.c_str());
    }

    params_callback_handle_ = this->add_on_set_parameters_callback(
      std::bind(&HikCameraNode::parametersCallback, this, std::placeholders::_1));

    left_capture_thread_ = std::thread{[this]() -> void {
      MV_FRAME_OUT left_out_frame;

      RCLCPP_INFO(this->get_logger(), "Publishing left_image!");

      left_image_msg_.header.frame_id = "camera_optical_frame";
      left_image_msg_.encoding = "rgb8";

      while (rclcpp::ok()) {
        left_nRet = MV_CC_GetImageBuffer(left_camera_handle_, &left_out_frame, 1000);
        if (MV_OK == left_nRet) {
          left_convert_param_.pDstBuffer = left_image_msg_.data.data();
          left_convert_param_.nDstBufferSize = left_image_msg_.data.size();
          left_convert_param_.pSrcData = left_out_frame.pBufAddr;
          left_convert_param_.nSrcDataLen = left_out_frame.stFrameInfo.nFrameLen;
          left_convert_param_.enSrcPixelType = left_out_frame.stFrameInfo.enPixelType;

          MV_CC_ConvertPixelType(left_camera_handle_, &left_convert_param_);

          left_image_msg_.header.stamp = this->now();
          left_image_msg_.height = left_out_frame.stFrameInfo.nHeight;
          left_image_msg_.width = left_out_frame.stFrameInfo.nWidth;
          left_image_msg_.step = left_out_frame.stFrameInfo.nWidth * 3;
          left_image_msg_.data.resize(left_image_msg_.width * left_image_msg_.height * 3);

          left_camera_info_msg_.header = left_image_msg_.header;
          left_camera_pub_.publish(left_image_msg_, left_camera_info_msg_);

          MV_CC_FreeImageBuffer(left_camera_handle_, &left_out_frame);
          left_fail_conut_ = 0;
        } else {
          RCLCPP_WARN(this->get_logger(), "Get buffer failed! left_nRet: [%x]", left_nRet);
          MV_CC_StopGrabbing(left_camera_handle_);
          MV_CC_StartGrabbing(left_camera_handle_);
          left_fail_conut_++;
        }

        if (left_fail_conut_ > 5) {
          RCLCPP_FATAL(this->get_logger(), "Left_Camera failed!");
          rclcpp::shutdown();
        }
      }
    }};

    right_capture_thread_ = std::thread{[this]() -> void {
      MV_FRAME_OUT right_out_frame;

      RCLCPP_INFO(this->get_logger(), "Publishing right_image!");

      right_image_msg_.header.frame_id = "camera_optical_frame";
      right_image_msg_.encoding = "rgb8";

      while (rclcpp::ok()) {
        right_nRet = MV_CC_GetImageBuffer(right_camera_handle_, &right_out_frame, 1000);
        if (MV_OK == right_nRet) {
          right_convert_param_.pDstBuffer = right_image_msg_.data.data();
          right_convert_param_.nDstBufferSize = right_image_msg_.data.size();
          right_convert_param_.pSrcData = right_out_frame.pBufAddr;
          right_convert_param_.nSrcDataLen = right_out_frame.stFrameInfo.nFrameLen;
          right_convert_param_.enSrcPixelType = right_out_frame.stFrameInfo.enPixelType;

          MV_CC_ConvertPixelType(right_camera_handle_, &right_convert_param_);

          right_image_msg_.header.stamp = this->now();
          right_image_msg_.height = right_out_frame.stFrameInfo.nHeight;
          right_image_msg_.width = right_out_frame.stFrameInfo.nWidth;
          right_image_msg_.step = right_out_frame.stFrameInfo.nWidth * 3;
          right_image_msg_.data.resize(right_image_msg_.width * right_image_msg_.height * 3);

          right_camera_info_msg_.header = right_image_msg_.header;
          right_camera_pub_.publish(right_image_msg_, right_camera_info_msg_);

          MV_CC_FreeImageBuffer(right_camera_handle_, &right_out_frame);
          right_fail_conut_ = 0;
        } else {
          RCLCPP_WARN(this->get_logger(), "Get buffer failed! right_nRet: [%x]", right_nRet);
          MV_CC_StopGrabbing(right_camera_handle_);
          MV_CC_StartGrabbing(right_camera_handle_);
          right_fail_conut_++;
        }

        if (right_fail_conut_ > 5) {
          RCLCPP_FATAL(this->get_logger(), "Right_Camera failed!");
          rclcpp::shutdown();
        }
      }
    }};
  }

  ~HikCameraNode() override
  {
    if (left_capture_thread_.joinable()) {
      left_capture_thread_.join();
    }
    if(right_capture_thread_.joinable()){
      right_capture_thread_.join();
    }
    if (left_camera_handle_) {
      MV_CC_StopGrabbing(left_camera_handle_);
      MV_CC_CloseDevice(left_camera_handle_);
      MV_CC_DestroyHandle(&left_camera_handle_);
    }
    if(right_camera_handle_){
      MV_CC_StopGrabbing(right_camera_handle_);
      MV_CC_CloseDevice(right_camera_handle_);
      MV_CC_DestroyHandle(&right_camera_handle_);
    }
    RCLCPP_INFO(this->get_logger(), "HikCameraNode destroyed!");
  }

private:
  void left_camera_declareParameters()
  {
    rcl_interfaces::msg::ParameterDescriptor left_param_desc;
    MVCC_FLOATVALUE f_value;
    left_param_desc.integer_range.resize(1);
    left_param_desc.integer_range[0].step = 1;
    // Exposure time
    left_param_desc.description = "Exposure time in microseconds";
    MV_CC_GetFloatValue(left_camera_handle_, "ExposureTime", &f_value);
    left_param_desc.integer_range[0].from_value = f_value.fMin;
    left_param_desc.integer_range[0].to_value = f_value.fMax;
    double exposure_time = this->declare_parameter("left_exposure_time", 5000, left_param_desc);
    MV_CC_SetFloatValue(left_camera_handle_, "ExposureTime", exposure_time);
    RCLCPP_INFO(this->get_logger(), "left Exposure time: %f", exposure_time);

    // Gain
    left_param_desc.description = "Gain";
    MV_CC_GetFloatValue(left_camera_handle_, "Gain", &f_value);
    left_param_desc.integer_range[0].from_value = f_value.fMin;
    left_param_desc.integer_range[0].to_value = f_value.fMax;
    double gain = this->declare_parameter("left_gain", f_value.fCurValue, left_param_desc);
    MV_CC_SetFloatValue(left_camera_handle_, "Gain", gain);
    RCLCPP_INFO(this->get_logger(), "left Gain: %f", gain);
  }

  void right_camera_declareParameters()
  {
    rcl_interfaces::msg::ParameterDescriptor right_param_desc;
    MVCC_FLOATVALUE f_value;
    right_param_desc.integer_range.resize(1);
    right_param_desc.integer_range[0].step = 1;
    // Exposure time
    right_param_desc.description = "Exposure time in microseconds";
    MV_CC_GetFloatValue(right_camera_handle_, "ExposureTime", &f_value);
    right_param_desc.integer_range[0].from_value = f_value.fMin;
    right_param_desc.integer_range[0].to_value = f_value.fMax;
    double exposure_time = this->declare_parameter("right_exposure_time", 5000, right_param_desc);
    MV_CC_SetFloatValue(right_camera_handle_, "ExposureTime", exposure_time);
    RCLCPP_INFO(this->get_logger(), "right Exposure time: %f", exposure_time);

    // Gain
    right_param_desc.description = "Gain";
    MV_CC_GetFloatValue(right_camera_handle_, "Gain", &f_value);
    right_param_desc.integer_range[0].from_value = f_value.fMin;
    right_param_desc.integer_range[0].to_value = f_value.fMax;
    double gain = this->declare_parameter("right_gain", f_value.fCurValue, right_param_desc);
    MV_CC_SetFloatValue(right_camera_handle_, "Gain", gain);
    RCLCPP_INFO(this->get_logger(), "right Gain: %f", gain);
  }

  rcl_interfaces::msg::SetParametersResult parametersCallback(
    const std::vector<rclcpp::Parameter> & parameters)
  {
    rcl_interfaces::msg::SetParametersResult result;
    result.successful = true;
    for (const auto & param : parameters) {
      if (param.get_name() == "left_exposure_time") {
        int status = MV_CC_SetFloatValue(left_camera_handle_, "ExposureTime", param.as_int());
        if (MV_OK != status) {
          result.successful = false;
          result.reason = "Failed to set left exposure time, status = " + std::to_string(status);
        }
      } 
      else if (param.get_name() == "left_gain") {
        int status = MV_CC_SetFloatValue(left_camera_handle_, "Gain", param.as_double());
        if (MV_OK != status) {
          result.successful = false;
          result.reason = "Failed to set left gain, status = " + std::to_string(status);
        }
      }
      else if(param.get_name() == "right_exposure_time"){
        int status = MV_CC_SetFloatValue(right_camera_handle_, "ExposureTime", param.as_int());
        if (MV_OK != status) {
          result.successful = false;
          result.reason = "Failed to set right exposure time, status = " + std::to_string(status);
        }
      }
      else if(param.get_name() == "right_gain"){
        int status = MV_CC_SetFloatValue(right_camera_handle_, "Gain", param.as_double());
        if (MV_OK != status) {
          result.successful = false;
          result.reason = "Failed to set right gain, status = " + std::to_string(status);
        }
      }
      else {
        result.successful = false;
        result.reason = "Unknown parameter: " + param.get_name();
      }
    }
    return result;
  }

  sensor_msgs::msg::Image left_image_msg_;
  sensor_msgs::msg::Image right_image_msg_;

  image_transport::CameraPublisher left_camera_pub_;
  image_transport::CameraPublisher right_camera_pub_;

  int nRet = MV_OK;
  int left_nRet = MV_OK;
  int right_nRet = MV_OK;
  void * left_camera_handle_;
  void * right_camera_handle_;
  MV_IMAGE_BASIC_INFO left_img_info_;
  MV_IMAGE_BASIC_INFO right_img_info_;

  MV_CC_PIXEL_CONVERT_PARAM left_convert_param_;
  MV_CC_PIXEL_CONVERT_PARAM right_convert_param_;

  std::string left_camera_name_;
  std::string right_camera_name_;
  std::unique_ptr<camera_info_manager::CameraInfoManager> left_camera_info_manager_;
  std::unique_ptr<camera_info_manager::CameraInfoManager> right_camera_info_manager_;
  sensor_msgs::msg::CameraInfo left_camera_info_msg_;
  sensor_msgs::msg::CameraInfo right_camera_info_msg_;

  int left_fail_conut_ = 0;
  int right_fail_conut_ = 0;
  std::thread left_capture_thread_;
  std::thread right_capture_thread_;

  OnSetParametersCallbackHandle::SharedPtr params_callback_handle_;
};
}  // namespace hik_camera

#include "rclcpp_components/register_node_macro.hpp"

RCLCPP_COMPONENTS_REGISTER_NODE(hik_camera::HikCameraNode)
