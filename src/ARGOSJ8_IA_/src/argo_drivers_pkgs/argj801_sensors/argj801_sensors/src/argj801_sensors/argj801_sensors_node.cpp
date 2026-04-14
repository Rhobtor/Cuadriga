#include "argj801_sensors/argj801_sensors_node.hpp"

// -----------------------------------------------------------------------------
// Argj801SensorsNode
// -----------------------------------------------------------------------------
// Nodo Lifecycle que publica datos de sensores en tópicos ROS a partir de un
// backend configurable.
//
// Flujo principal:
// - on_configure(): lee parámetros, construye sensores vía Builder y prepara publishers.
// - on_activate(): arranca el sensor y lanza timers para publicar a rate fijo.
// - getFastData()/getSlowData(): llama al backend y actualiza diagnósticos.
// -----------------------------------------------------------------------------


/// LifecycleTalker constructor
/**
 * The lifecycletalker/lifecyclenode constructor has the same
 * arguments a regular node.
 */

Argj801SensorsNode::Argj801SensorsNode(std::shared_ptr<Builder::SensorBuilder> builder):
rclcpp_lifecycle::LifecycleNode("argj08_sensors_node",rclcpp::NodeOptions().use_intra_process_comms(false)), builder(builder){
  this->declare_parameter("lcm_params.lcm_config_file", "argj801_lcm_config_sensors.yaml");
  
  // Frames usados para rellenar `header.frame_id`.
  this->declare_parameter("robot_frame", "base_link");
  this->declare_parameter("velodyne_frame", "velodyne_link");
  this->declare_parameter("sick_frame", "sick_link");

  // Flags para activar/desactivar streams concretos sin recompilar.
  this->declare_parameter("velodyne", true);
  this->declare_parameter("sick", true);
  this->declare_parameter("left_telemetry", true);
  this->declare_parameter("right_telemetry", true);
  this->declare_parameter("odometer", true);
  this->declare_parameter("twist", true);
  
  // Parámetros de la cámara (RTSP) cuando se habilita `camera`.
  this->declare_parameter("camera",true);
  this->declare_parameter("camera_url","rtsp://192.168.0.61/axis-media/media.amp?camera=1&streamprofile=Balanced");
  this->declare_parameter("camera_frame","camera_link");
  this->declare_parameter("camera_topic","front_camera_compressed");
  this->declare_parameter<int>("image_compression_ratio", 75);
  this->declare_parameter<bool>("resize_image", true);


}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
Argj801SensorsNode::on_configure(const rclcpp_lifecycle::State &)
{
  RCLCPP_INFO(get_logger(), "Configuring...");

  // Get parameters
  this->get_parameter("lcm_params.lcm_config_file", lcm_config_file);  
  this->get_parameter("robot_frame", robot_frame);
  this->get_parameter("velodyne_frame", velodyne_frame);
  this->get_parameter("sick_frame", sick_frame);
  this->get_parameter("velodyne", velodyne);
  this->get_parameter("sick", sick);
  this->get_parameter("left_telemetry", left_motor);
  this->get_parameter("right_telemetry", right_motor);
  this->get_parameter("odometer", odometer);
  this->get_parameter("twist", twist);

  this->get_parameter("camera",camera);
  this->get_parameter("camera_url",camera_url);
  this->get_parameter("camera_frame",camera_frame);
  this->get_parameter("camera_topic",camera_topic);
  this->get_parameter("image_compression_ratio",compression_ratio);
  this->get_parameter("resize_image",resize_image);

 
  try {
    // Nota: buildPreviousConfig() se usa para cargar config (LCM o cámara)
    // antes de construir los objetos concretos.
    if(velodyne || sick || left_motor || right_motor || odometer || twist){
      builder->buildPreviousConfig(lcm_config_file);
    }
    else
      builder->buildPreviousConfig(camera_url);
      
    visitor = std::make_shared<Visitor::SensorDataVisitor>();

    if(velodyne){
      velodyne_publisher = this->create_publisher<sensor_msgs::msg::LaserScan>("velodyne_scan", 10);
      builder->buildVelocine(velodyne_publisher, velodyne_frame);
    }
    if(sick){
      sick_publisher = this->create_publisher<sensor_msgs::msg::LaserScan>("sick_scan", 10);
      builder->buildSick(sick_publisher, sick_frame);
    }
    if(left_motor){
      left_motor_publisher = this->create_publisher<argj801_sensors_msgs::msg::MotorTelemetry>("left_motor_telemetry", 10);
      builder->buildMotorTelemetryLeft(left_motor_publisher, robot_frame);
    }
    if(right_motor){
      right_motor_publisher = this->create_publisher<argj801_sensors_msgs::msg::MotorTelemetry>("right_motor_telemetry", 10);
      builder->buildMotorTelemetryRigth(right_motor_publisher, robot_frame);
    }
    if(odometer){
      odometer_publisher = this->create_publisher<argj801_sensors_msgs::msg::Odometer>("odometer", 10);
      builder->buildOdometer(odometer_publisher, robot_frame);
    }
    if(twist){
      twist_publisher = this->create_publisher<geometry_msgs::msg::TwistStamped>("twist", 10);
      builder->buildTwist(twist_publisher, robot_frame);
    }
    if(camera){
      // // Create a temporary rclcpp::Node pointer from the LifecycleNode
      //   auto temp_node = std::make_shared<rclcpp::Node>(
      //       "_temp_node",
      //       rclcpp::NodeOptions().arguments(this->get_node_options().arguments())
      //   );

      //   // Initialize ImageTransport using the temporary node
      //   it_ = std::make_shared<image_transport::ImageTransport>(temp_node);

      //   // Advertise the topic using the temporary node
      //   camera_publisher = std::make_shared<image_transport::Publisher>(
      //       it_->advertise("camera_compressed", 1)
      //   );

        // Clean up the temporary node as it is no longer needed
        // temp_node.reset();

      camera_publisher = this->create_publisher<sensor_msgs::msg::CompressedImage>(camera_topic, 2);
      builder->buildCamera(camera_publisher, camera_frame, compression_ratio, resize_image);
    }
    sensor = builder->getSensor();

    diagnostic_ = std::make_unique<diagnostic_updater::Updater>(this);
    diagnostic_->setHardwareID(sensor->getID());

    if(velodyne || sick) {
      lidar_desired_freq_ = 5.0;
      fast_freq_diag_ = std::make_unique<diagnostic_updater::FrequencyStatus>(diagnostic_updater::FrequencyStatusParam(&lidar_desired_freq_, &lidar_desired_freq_, 0.15),"Lidars Frequency");
      diagnostic_->add(*(fast_freq_diag_.get()));
    }
    
    if(left_motor || right_motor || odometer || twist) {
      drive_line_desired_freq_ = 2.0;
      slow_freq_diag_ = std::make_unique<diagnostic_updater::FrequencyStatus>(diagnostic_updater::FrequencyStatusParam(&drive_line_desired_freq_, &drive_line_desired_freq_, 0.15),"Drive Line Frequency");
      diagnostic_->add(*(slow_freq_diag_.get()));
    }
    
    if(camera) {
      camera_desired_freq_ = 10.0;
      fast_freq_diag_ = std::make_unique<diagnostic_updater::FrequencyStatus>(diagnostic_updater::FrequencyStatusParam(&camera_desired_freq_, &camera_desired_freq_, 0.15),"Camera Frequency");
      diagnostic_->add(*(fast_freq_diag_.get()));
    }
  } 
  catch (const std::exception& e) {
    RCLCPP_ERROR(get_logger(), "Problem when configuring: %s", e.what());
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::FAILURE;
  }
  RCLCPP_INFO(get_logger(), "Configured.");

  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
Argj801SensorsNode::on_activate(const rclcpp_lifecycle::State &state)
{
  // Overriding this method is optional, a lot of times the default is enough.
  LifecycleNode::on_activate(state);

  RCUTILS_LOG_INFO_NAMED(get_name(), "Activating...");

  try
  {
    sensor->start();
  }
  catch(const std::exception& e)
  {
    RCLCPP_ERROR(get_logger(), "Problem activating the node: %s", e.what());
    diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Problem activating the node " + std::string(e.what()));
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::FAILURE;
  }
  
  if(!group1)
    group1 = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
  
  if(velodyne || sick) {
    if(!lidarTimer) 
      lidarTimer = this->create_wall_timer(0.2s, std::bind(&Argj801SensorsNode::getFastData, this),group1);
    else 
      lidarTimer->reset();
  }    

  if(left_motor || right_motor || odometer || twist) {
    if(!driveLineTimer)
      driveLineTimer = this->create_wall_timer(0.5s, std::bind(&Argj801SensorsNode::getSlowData, this),group1);
    else
      driveLineTimer->reset();
  }

  if(camera) {
    if(!cameraTimer)
      //cameraTimer = this->create_wall_timer(0.07s, std::bind(&Argj801SensorsNode::getCameraData, this),group1);
      cameraTimer = this->create_wall_timer(0.07s, std::bind(&Argj801SensorsNode::getFastData, this));
    else
      cameraTimer->reset();
  }

  RCUTILS_LOG_INFO_NAMED(get_name(), "Activated.");

  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
Argj801SensorsNode::on_deactivate(const rclcpp_lifecycle::State &state)
{

  LifecycleNode::on_deactivate(state);
  RCUTILS_LOG_INFO_NAMED(get_name(), "Deactivating...");
  if(driveLineTimer) {
    driveLineTimer->cancel();
    driveLineTimer.reset();
  }
  if(lidarTimer) {
    lidarTimer->cancel();
    lidarTimer.reset();
  }
  if(cameraTimer) {
    cameraTimer->cancel();
    cameraTimer.reset();
  }

  RCUTILS_LOG_INFO_NAMED(get_name(), "Deactivated");

  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
Argj801SensorsNode::on_cleanup(const rclcpp_lifecycle::State &)
{
  RCUTILS_LOG_INFO_NAMED(get_name(), "Cleaning...");
  diagnostic_.reset();
  RCUTILS_LOG_INFO_NAMED(get_name(), "Cleaned");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
Argj801SensorsNode::on_shutdown(const rclcpp_lifecycle::State &state)
{
  RCUTILS_LOG_INFO_NAMED(get_name(),"Shutdowning: %s.", state.label().c_str());
  
  RCUTILS_LOG_INFO_NAMED(get_name(),"Shutdowned.");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
Argj801SensorsNode::on_error(const rclcpp_lifecycle::State &state) {

  RCUTILS_LOG_INFO_NAMED(get_name(),"On error: %s.", state.label().c_str());
  if(this->get_current_state().id() == lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE){
    this->deactivate();
  }
  if(this->get_current_state().id() == lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE){
    this->cleanup();
  }
  this->shutdown();
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::FAILURE;
}

void Argj801SensorsNode::getFastData() {
  try
  {
    sensor->getFastData(visitor);    
    fast_freq_diag_->tick();  
  }
  catch(const std::exception& e)
  {
    RCLCPP_ERROR(get_logger(), "Problem getting data: %s", e.what());
    diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Problem getting data " + std::string(e.what()));

  }
}

void Argj801SensorsNode::getSlowData() {
  try
  {
    sensor->getSlowData(visitor);
    slow_freq_diag_->tick();
  }
  catch(const std::exception& e)
  {
    RCLCPP_ERROR(get_logger(), "Problem getting data: %s", e.what());
    diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Problem getting data " + std::string(e.what()));
  }
}





