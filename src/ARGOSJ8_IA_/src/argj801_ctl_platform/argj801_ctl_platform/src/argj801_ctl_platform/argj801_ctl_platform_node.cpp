#include "argj801_ctl_platform/argj801_ctl_platform_node.hpp"

// -----------------------------------------------------------------------------
// Implementación del nodo de control de plataforma (Lifecycle).
//
// Objetivo:
// - Recibir comandos de alto nivel (Twist) y convertirlos a comandos de actuador
//   según el modo de operación (Arduino / LCM / Gazebo).
// - Implementar watchdog/self-test/diagnóstico para aumentar la seguridad.
//
// Nota de mantenimiento: en este repositorio estamos añadiendo documentación
// (comentarios) sin modificar la lógica.
// -----------------------------------------------------------------------------


/// LifecycleTalker constructor
/**
 * The lifecycletalker/lifecyclenode constructor has the same
 * arguments a regular node.
 */
CtlPlatformNode::CtlPlatformNode(const std::string &node_name, bool intra_process_comms):
rclcpp_lifecycle::LifecycleNode(node_name,rclcpp::NodeOptions().use_intra_process_comms(intra_process_comms))
{
  platformHearbeatCount = 0;
  this->declare_parameter("operation_mode", 2);
  this->declare_parameter("self_test_active", false);
  this->declare_parameter("throttle_topic_name", "cmd_throttle_msg");
  this->declare_parameter("kinematic_debug_topic_name", "kinematic_debug");
  this->declare_parameter("secured_cmd_vel_topic_name", "secure_cmd_vel");
  this->declare_parameter("arduino_params.port", "/dev/ttyUSB0");
  this->declare_parameter("arduino_params.watchdog_active", true);
  this->declare_parameter("lcm_params.lcm_config_file", "argj801_lcm_config_platform.yaml");
  // Safety: by default we require vehicle heartbeat in LCM mode. Set to false ONLY for debugging/bringup.
  this->declare_parameter("lcm_params.require_vehicle_heartbeat", true);
  this->declare_parameter("kinematic_parameters.efective_radius", 0.285);
  this->declare_parameter("kinematic_parameters.xICR", 1.0);
  this->declare_parameter("kinematic_parameters.throttle_to_percent", 3.9276);
  this->declare_parameter("kinematic_parameters.steer_to_percent",  17.2790);
  this->declare_parameter("kinematic_parameters.steer_acc", 1.23);
  this->declare_parameter("kinematic_parameters.throttle_acc",  1.860);
}

void CtlPlatformNode::cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
{

  if(this->get_current_state().id() == lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE) {
    switch (pOpMode)
    {
      case PlatformOperationMode::Arduino:
        if(serialArduino.getIs_started()) {
          if(watchdog_active)
            timerWatchdog->reset();
          try {
            serialArduino.sendSetPoints({msg->linear.x, msg->linear.y, msg->linear.z},{msg->angular.x,msg->angular.y,msg->angular.z});
          }
          catch(std::exception &e) {
            RCLCPP_ERROR(get_logger(),"%s", e.what());
          }
          freq_diag_->tick();
        }
        // else {
        //   RCLCPP_ERROR(get_logger(),"Arduino not started");
        //   diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"Arduino not started");
        //   this->shutdown();
        // }  
        break;
      case PlatformOperationMode::LCM:
        last_twist_time_ = this->now();  // Update the timestamp with the current time
        target_x_ = msg->linear.x;
        target_rot_ = msg->angular.z;
        break;
      case PlatformOperationMode::Gazebo:
        last_twist_time_ = this->now();  // Update the timestamp with the current time
        target_x_ = msg->linear.x;
        target_rot_ = msg->angular.z;
        break;  
    }
    
  }
}

void CtlPlatformNode::timerWatchdogCallback() {
  RCLCPP_INFO(get_logger(),"Timer callback");
  if(serialArduino.getIs_started())
    serialArduino.stop();
  timerWatchdog->cancel();
  this->deactivate();
}



rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
CtlPlatformNode::on_configure(const rclcpp_lifecycle::State &)
{
  RCLCPP_INFO(get_logger(), "Configuring...");

  pOpMode = static_cast<PlatformOperationMode>(this->get_parameter("operation_mode").get_parameter_value().get<int>());

  std::cout <<"Pop mode is "<<this->get_parameter("operation_mode").get_parameter_value().get<int>() <<std::endl;
  
  this->get_parameter("self_test_active", self_test_active);
  this->get_parameter("arduino_params.port", port);
  this->get_parameter("arduino_params.watchdog_active", watchdog_active);
  this->get_parameter("lcm_params.lcm_config_file", lcm_config_file);
  this->get_parameter("lcm_params.require_vehicle_heartbeat", require_vehicle_heartbeat);
  this->get_parameter("kinematic_parameters.efective_radius", efective_radius );
  this->get_parameter("kinematic_parameters.xICR", xICR );
  this->get_parameter("kinematic_parameters.throttle_to_percent", throttle_to_percent );
  this->get_parameter("kinematic_parameters.steer_to_percent", steer_to_percent );
  this->get_parameter("throttle_topic_name", cmd_throttle_name);
  this->get_parameter("kinematic_debug_topic_name", kinematic_debug_topic_name);
  this->get_parameter("secured_cmd_vel_topic_name", cmd_vel_name_in);
  this->get_parameter("steer_acc", steer_acc);
  this->get_parameter("throttle_acc", throttle_acc);
  this->get_parameter("kinematic_parameters.steer_acc", steer_acc);
  this->get_parameter("kinematic_parameters.throttle_acc", throttle_acc);

  emergencyStopServ = this->create_service<argj801_ctl_platform_interfaces::srv::EmergencyStop>("emergency_stop",std::bind(&CtlPlatformNode::emergencyStop,this,std::placeholders::_1,std::placeholders::_2));
  subscription = this->create_subscription<geometry_msgs::msg::Twist>("/ARGJ801/cmd_vel" ,rclcpp::SensorDataQoS(),std::bind(&CtlPlatformNode::cmd_vel_callback, this, std::placeholders::_1));

  diagnostic_ = std::make_shared<diagnostic_updater::Updater>(this);
  diagnostic_->setHardwareID("Ctl_Platform_J8");
  switch (pOpMode) {
    case PlatformOperationMode::Arduino:
      RCLCPP_INFO(get_logger(), "Arduino mode");


      setVelocityServ = this->create_service<argj801_ctl_platform_interfaces::srv::SetVelocity>("set_velocity",std::bind(&CtlPlatformNode::setVelocity, this,std::placeholders::_1,std::placeholders::_2));
      pingServ = this->create_service<std_srvs::srv::Empty>("ping",std::bind(&CtlPlatformNode::ping,this,std::placeholders::_1,std::placeholders::_2));
      getVelocityServ = this->create_service<argj801_ctl_platform_interfaces::srv::GetVelocity>("get_velocity",std::bind(&CtlPlatformNode::getVelocity,this,std::placeholders::_1,std::placeholders::_2));


      desired_freq_ = 10;
      freq_diag_ = std::make_unique<diagnostic_updater::FrequencyStatus>(diagnostic_updater::FrequencyStatusParam(&desired_freq_, &desired_freq_, 0.12));
      freq_diag_->clear();

      diagnostic_->add( *(freq_diag_.get()));
      diagnostic_->add("LCM Control Platform Status", this, &CtlPlatformNode::arduinoStatus);

      break;
    case PlatformOperationMode::LCM:
      RCLCPP_INFO(get_logger(), "LCM mode");

      cmdThrottlePub = this->create_publisher<argj801_ctl_platform_interfaces::msg::CmdThrottleMsg>(cmd_throttle_name, 10);
      kinematicDebugPub = this->create_publisher<argj801_ctl_platform_interfaces::msg::KinematicDebugMsg>(kinematic_debug_topic_name, 10);

      
      lcm_config_path = ament_index_cpp::get_package_share_directory("argj801_ctl_platform");
      lcm_config_path = lcm_config_path + "/../argj801_lcm/config/" + lcm_config_file;

      try {
        lcmInterface = std::make_shared<argj801_lcm::LCMInterface>(lcm_config_path,false,false,true,false,true,false,false,false,false,false);
      }
      catch(std::exception &e) {
        RCLCPP_ERROR(get_logger(),"Imposible to create LCM interface: %s", e.what());
        diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"Imposible to create LCM interface: "+ std::string(e.what()));
        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::ERROR;
      }
     



      desired_freq_ = 50; // Hz
      freq_diag_ = std::make_unique<diagnostic_updater::FrequencyStatus>(diagnostic_updater::FrequencyStatusParam(&desired_freq_, &desired_freq_, 0.12));
      freq_diag_->clear();
      argo_kinematic_model = std::make_unique<Argo_J8_KinematicModel>(efective_radius, xICR, throttle_to_percent, steer_to_percent, 
                                                                      steer_acc, throttle_acc, desired_freq_);

      diagnostic_->add( *(freq_diag_.get()));
      // diagnostic_->add("LCM Control Platform Status", this, &CtlPlatformNode::lcmStatus);
    
      break;
    case PlatformOperationMode::Gazebo:
      RCLCPP_INFO(get_logger(), "Gazebo mode");

      cmdThrottlePub = this->create_publisher<argj801_ctl_platform_interfaces::msg::CmdThrottleMsg>(cmd_throttle_name, 10);
      kinematicDebugPub = this->create_publisher<argj801_ctl_platform_interfaces::msg::KinematicDebugMsg>(kinematic_debug_topic_name, 10);

      argo_kinematic_model = std::make_unique<Argo_J8_KinematicModel>(efective_radius, xICR, throttle_to_percent, steer_to_percent, 
                                                                      steer_acc, throttle_acc, desired_freq_);
      desired_freq_ = 50; // Hz
      freq_diag_ = std::make_unique<diagnostic_updater::FrequencyStatus>(diagnostic_updater::FrequencyStatusParam(&desired_freq_, &desired_freq_, 0.12));
      freq_diag_->clear();

      diagnostic_->add( *(freq_diag_.get()));
      break;
    default:
      RCLCPP_ERROR(get_logger(),"Node bad configuration");
      diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"Node bad configuration");
      return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::ERROR;
      break;
  }
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
CtlPlatformNode::on_activate(const rclcpp_lifecycle::State &state)
{
  lcmmessages::dat_vehicle_heartbeat_msg platformHeartbeatMsg;
  lcmmessages::request_connection_response_msg requestConnectionResponseMsg;

  // Overriding this method is optional, a lot of times the default is enough.
  LifecycleNode::on_activate(state);

  RCUTILS_LOG_INFO_NAMED(get_name(), "Activating...");


  switch (pOpMode) {
    case PlatformOperationMode::Arduino:
      try {
        serialArduino.openPort(port.c_str());
        serialArduino.start();
      }
      catch(std::exception &e) {
        RCLCPP_ERROR(get_logger(),"Imposible to communicate with Arduino %s", e.what());
        diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"Node bad configuration"+ std::string(e.what()));
        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::FAILURE;
      }
      if(watchdog_active && !timerWatchdog)
        timerWatchdog = this->create_wall_timer(3s, std::bind(&CtlPlatformNode::timerWatchdogCallback,this));

      if(self_test_active) {
        self_test_ = std::make_unique<self_test::TestRunner>(get_node_base_interface(), get_node_services_interface(), get_node_logging_interface());

        self_test_->setID("Ctl_Platform_J8");

        self_test_->add("Interruption Test", this, &CtlPlatformNode::interruptionTest);
        self_test_->add("Connect Test", this, &CtlPlatformNode::connectStartTest);
        self_test_->add("Ping Test", this, &CtlPlatformNode::pingTest);
        self_test_->add("Move Test", this, &CtlPlatformNode::moveTest);
        self_test_->add("Disconnect Test", this, &CtlPlatformNode::disconnectTest);
        self_test_->add("Resume Test", this, &CtlPlatformNode::resumeTest);
      }
    break;
    case PlatformOperationMode::LCM:
        try
        {
          if(!group1)
            group1 = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
          if(!timerPlatfomHeartbeat)
            timerPlatfomHeartbeat =  this->create_wall_timer(0.5s, std::bind(&CtlPlatformNode::timerPlatformHeartBeatCallback,this),group1);
          else
            timerPlatfomHeartbeat->reset();

          if(!timerStationHeartbeat)
            timerStationHeartbeat = this->create_wall_timer(1s, std::bind(&CtlPlatformNode::timerStationHeartBeatCallback,this),group1);
          else  
            timerStationHeartbeat->reset();

          lcmInterface->sendRequestConnectionMsg(argj801_lcm::RequestConnectionType::Control);
          requestConnectionResponseMsg = lcmInterface->receiveRequestConnectionResponseMsg(1000);
          
          if(!requestConnectionResponseMsg.granted && requestConnectionResponseMsg.connection_type != int8_t(argj801_lcm::RequestConnectionType::Control)) {
            RCLCPP_ERROR(get_logger(),"Could not connect: %s", "Problem with connection response message");
            diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"Could not connect: "+ std::string("Problem with connection response message"));
            return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::ERROR;
          }
          RCLCPP_INFO(get_logger(),"Connection to CONTROL granted to the station %d", requestConnectionResponseMsg.station_id);
          for(int i=0;i<5;i++)
            lcmInterface->sendDiscreteDeviceMsg(argj801_lcm::DiscreteDeviceType::Contractors,true);
        }

        catch(const std::exception& e)
        {
          RCLCPP_INFO(get_logger(),"Station heartbeat not received: %s", e.what());
          diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"Could not connect: "+ std::string("No heartbeat message received"));
          return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::ERROR;
        }
        
        last_twist_time_ = this->now();

        if(!timerWellsSpeed)
          timerWellsSpeed = this->create_wall_timer(0.02s,std::bind(&CtlPlatformNode::timerWellsSpeedCallback,this));
        else
          timerWellsSpeed->reset();
        
        platformHearbeatCount =0;
        

      break;
    case PlatformOperationMode::Gazebo:
        last_twist_time_ = this->now();
        timerWellsSpeed = this->create_wall_timer(0.02s,std::bind(&CtlPlatformNode::timerWellsSpeedCallback,this));
      break;
    default:
      diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"Node bad configuration");
      RCLCPP_ERROR(get_logger(),"Node bad configuration");
      return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::ERROR;
    break;
  }
  RCUTILS_LOG_INFO_NAMED(get_name(),"Node Activated");

  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
CtlPlatformNode::on_deactivate(const rclcpp_lifecycle::State &state)
{

  LifecycleNode::on_deactivate(state);

  RCUTILS_LOG_INFO_NAMED(get_name(), "Deactivating...");
  switch (pOpMode) {
    case PlatformOperationMode::Arduino:
      try {
        if(timerWatchdog)
          timerWatchdog->cancel();
        serialArduino.stop();
        serialArduino.closePort();
      }
      catch(std::exception &e) {
        diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"Impossible to close the port"+ std::string(e.what()));
        RCLCPP_ERROR(get_logger(),"Impossible to close the port: %s",e.what());
        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::ERROR;
      }
    break;
    case PlatformOperationMode::LCM:
      //timerPlatfomHeartbeat->cancel();
      //timerStationHeartbeat->cancel();
      timerWellsSpeed->cancel();
      lcmInterface->sendDiscreteDeviceMsg(argj801_lcm::DiscreteDeviceType::Contractors,false);
      lcmInterface->sendRequestConnectionMsg(argj801_lcm::RequestConnectionType::DisconectControl); 
      break;
    default:
      RCLCPP_ERROR(get_logger(),"Node bad configuration");
      diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"Node bad configuration");
      return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::ERROR;
    break;
  }
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
CtlPlatformNode::on_cleanup(const rclcpp_lifecycle::State &)
{
  RCUTILS_LOG_INFO_NAMED(get_name(), "Cleaning...");
  subscription.reset();
  emergencyStopServ.reset();
  diagnostic_.reset();
  switch (pOpMode) {
    case PlatformOperationMode::Arduino:
      setVelocityServ.reset();
      pingServ.reset();
      getVelocityServ.reset();
    break;
    case PlatformOperationMode::LCM:
      if(timerPlatfomHeartbeat)
        timerPlatfomHeartbeat->cancel();
      if(timerStationHeartbeat)
        timerStationHeartbeat->cancel();
      if(timerWellsSpeed)
        timerWellsSpeed->cancel();
      lcmInterface.reset();
      break;
    default:
      RCLCPP_ERROR(get_logger(),"Node bad configuration");
      diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"Node bad configuration");
      return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::ERROR;
    break;
  }
  
  RCUTILS_LOG_INFO_NAMED(get_name(), "Cleaned.");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
CtlPlatformNode::on_shutdown(const rclcpp_lifecycle::State &state)
{

  RCUTILS_LOG_INFO_NAMED(get_name(),"Shutdowning: %s.", state.label().c_str());  

  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
CtlPlatformNode::on_error(const rclcpp_lifecycle::State &state) {

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

bool CtlPlatformNode::emergencyStop(const std::shared_ptr<argj801_ctl_platform_interfaces::srv::EmergencyStop::Request> req,const std::shared_ptr<argj801_ctl_platform_interfaces::srv::EmergencyStop::Response> res) {
  
  if(req->stop && this->get_current_state().id() == lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE)
    this->deactivate();
  else if(!req->stop && this->get_current_state().id() == lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE)
    this->activate();
  
  return true;
}

//////////// ARDUINO METHODS //////////////

bool CtlPlatformNode::setVelocity(const std::shared_ptr<argj801_ctl_platform_interfaces::srv::SetVelocity::Request> req,const std::shared_ptr<argj801_ctl_platform_interfaces::srv::SetVelocity::Response> res) {
  if((req.get()->new_velocity < 0 || req.get()->new_velocity > 10) || this->get_current_state().id() != lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE)
    return false;

  try {
    if(serialArduino.getIs_started())
      res->actual_velocity = serialArduino.set_speed(req.get()->new_velocity);
    return true;
  }
  catch(std::exception &e) {
    res->actual_velocity = -1;
    diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,std::string(e.what()));
    RCLCPP_ERROR(get_logger(),"%s", e.what());
    return false;
  }
}


bool CtlPlatformNode::getVelocity(const std::shared_ptr<argj801_ctl_platform_interfaces::srv::GetVelocity::Request> req,const std::shared_ptr<argj801_ctl_platform_interfaces::srv::GetVelocity::Response> res) {
    if(this->get_current_state().id() != lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE)
      return false;

    try {
      res->actual_velocity = serialArduino.get_speed();
    }
    catch(std::exception &e) {
      diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,std::string(e.what()));
      RCLCPP_ERROR(get_logger(),"%s", e.what());
      return false;
    }
    return true;
}

bool CtlPlatformNode::ping(const std::shared_ptr<std_srvs::srv::Empty::Request> &req,const std::shared_ptr<std_srvs::srv::Empty::Response> &res) {
    if(this->get_current_state().id() != lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE)
      return false;

    try {
      serialArduino.ping();
    }
    catch(std::exception &e) {
      diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,std::string(e.what()));
      RCLCPP_ERROR(get_logger(),"%s", e.what());
      return false;
    }
    return true;
}

void CtlPlatformNode::arduinoStatus(diagnostic_updater::DiagnosticStatusWrapper &status) {

  if(this->get_current_state().id() == lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE) {
      status.add("Is Started",serialArduino.getIs_started());
      try {
        serialArduino.ping();
        status.summary(diagnostic_msgs::msg::DiagnosticStatus::OK, "CONTROL PLATFORM ARDUINO STATUS");
        status.add("Arduino active", true);
      }
      catch(std::exception &e) {
        RCLCPP_ERROR(get_logger(),"PING: %s", e.what());
        status.summary(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "CONTROL PLATFORM ARDUINO STATUS");
        status.add("Arduino active", false);
      }
  }
  else {
    status.add("Arduino active", false);
    status.summary(diagnostic_msgs::msg::DiagnosticStatus::OK, "CONTROL PLATFORM ARDUINO NOT CONFIGURED");
  }
}

void CtlPlatformNode::interruptionTest(diagnostic_updater::DiagnosticStatusWrapper &status) {
  if(watchdog_active)
    timerWatchdog->cancel();

  try {
    serialArduino.stop();
    serialArduino.closePort();
    status.summary(diagnostic_msgs::msg::DiagnosticStatus::OK, "Interruption test successfully.");

  }
  catch(std::exception &e) {
    RCLCPP_ERROR(get_logger(),"Could not be done the interruption test: %s",e.what());
    status.summary(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Could not be done the interruption test: "+ std::string(e.what()));
  }
}

void CtlPlatformNode::connectStartTest(diagnostic_updater::DiagnosticStatusWrapper &status) {
    try {
      serialArduino.openPort(port.c_str());
      serialArduino.start();
      status.summary(diagnostic_msgs::msg::DiagnosticStatus::OK, "Connect test successfully.");
    }
    catch(std::exception &e) {
      RCLCPP_ERROR(get_logger(),"Could not be done the connect test: %s",e.what());
      status.summary(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Could not be done the connect test: "+ std::string(e.what()));
    }
}

void CtlPlatformNode::pingTest(diagnostic_updater::DiagnosticStatusWrapper &status) {
    try {
      serialArduino.ping();
      status.summary(diagnostic_msgs::msg::DiagnosticStatus::OK, "Ping test successfully.");
    }
    catch(std::exception &e) {
      RCLCPP_ERROR(get_logger(),"Could not be done the ping test: %s", e.what());
      status.summary(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Could not be done the ping test: "+ std::string(e.what()));
    }
}  

void CtlPlatformNode::moveTest(diagnostic_updater::DiagnosticStatusWrapper &status) {
    try {
        serialArduino.sendSetPoints({0.0,0.0,0.0},{0.0,0.0,0.0});
        status.summary(diagnostic_msgs::msg::DiagnosticStatus::OK, "Move test successfully.");
    }
    catch(std::exception &e) {
        RCLCPP_ERROR(get_logger(),"Could not be done the move test: %s", e.what());
        status.summary(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Could not be done the move test: "+ std::string(e.what()));
    }
}  

void CtlPlatformNode::disconnectTest(diagnostic_updater::DiagnosticStatusWrapper &status) {
  try {
    serialArduino.stop();
    serialArduino.closePort();
    status.summary(diagnostic_msgs::msg::DiagnosticStatus::OK, "Disconnect test successfully.");
  }
  catch(std::exception &e) {
    RCLCPP_ERROR(get_logger(),"Could not be done the disconnect test: %s", e.what());
    status.summary(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Could not be done the disconnect test: "+ std::string(e.what()));
  }
}

void CtlPlatformNode::resumeTest(diagnostic_updater::DiagnosticStatusWrapper &status) {
  try {
    serialArduino.openPort(port.c_str());
    serialArduino.start();

  }
  catch(std::exception &e) {
    RCLCPP_ERROR(get_logger(),"Could not be done the resume test: %s", e.what());
    status.summary(diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Could not be done the resume test: "+ std::string(e.what()));
  }

  if(watchdog_active)
    timerWatchdog->reset();
  status.summary(diagnostic_msgs::msg::DiagnosticStatus::OK, "Resume test successfully.");
}
//////////// END ARDUINO METHODS //////////////


//////////////// LCM METHODS /////////////////
void CtlPlatformNode::timerPlatformHeartBeatCallback() {
  if(!require_vehicle_heartbeat) {
    // Debug/bringup mode: do not require vehicle heartbeat.
    // Note: platformHeartbeatMsg won't be updated in this mode.
    return;
  }
  try {
    platformHeartbeatMsg = lcmInterface->receiveDatPlatformHeartbeatMsg(1000);
    platformHearbeatCount=0;
    if(platformHeartbeatMsg.control_mode.mode == int8_t(argj801_lcm::ControlMode::FollowMe)) {
      RCLCPP_ERROR(get_logger(),"Follow me mode activated");
      diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"Follow me mode activated");
      if(this->get_current_state().id() == lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE)
        this->deactivate();
    }
    else if(platformHeartbeatMsg.control_mode.mode == int8_t(argj801_lcm::ControlMode::USBGamepad)) {
      RCLCPP_ERROR(get_logger(),"USB Gamepad mode activated");
      diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"USB Gamepad mode activated");
      if(this->get_current_state().id() == lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE)
        this->deactivate();
    }
    else if(platformHeartbeatMsg.control_mode.mode == int8_t(argj801_lcm::ControlMode::RadioController)) {
      RCLCPP_ERROR(get_logger(),"Radio Controller mode activated");
      diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"Radio Controller mode activated");
      if(this->get_current_state().id() == lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE)
        this->deactivate();
    }
    else if(platformHeartbeatMsg.state == -1) {
      RCLCPP_ERROR(get_logger(),"Emergency Remote mode activated");
      diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"Emergency Remote mode activated");
      if(this->get_current_state().id() == lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE)
        this->deactivate();
    }
  }  
  catch(const std::exception& e) {
    RCLCPP_ERROR(get_logger(),"Vehicle heartbeat not received: %s", e.what());
    diagnostic_->broadcast(diagnostic_msgs::msg::DiagnosticStatus::ERROR,"Vehicle heartbeat not received: "+ std::string(e.what()));
    if(++platformHearbeatCount > HEARBEAT_COUNT_MAX)
      this->deactivate();
  }
}

void CtlPlatformNode::timerStationHeartBeatCallback() {
  if(cont < 200) {
    lcmInterface->sendStationHeartbeatMsg();
    // RCLCPP_INFO(get_logger(),"Station heartbeat sent");
    //cont++;
  }
  else
    RCLCPP_INFO(get_logger(),"Station heartbeat NOT sent");

}

void CtlPlatformNode::publishCmdThrottle(double throttle, double steering) {
  if(!cmdThrottlePub) {
    return;
  }

  argj801_ctl_platform_interfaces::msg::CmdThrottleMsg cmdThrottleMsg;
  cmdThrottleMsg.throttle = throttle;
  cmdThrottleMsg.steering = steering;
  cmdThrottlePub->publish(cmdThrottleMsg);
}

void CtlPlatformNode::publishKinematicDebug(double throttle, double steering) {
  if(!kinematicDebugPub || !argo_kinematic_model) {
    return;
  }

  argj801_ctl_platform_interfaces::msg::KinematicDebugMsg debugMsg;
  debugMsg.throttle = throttle;
  debugMsg.steering = steering;
  debugMsg.requested_throttle_acc = argo_kinematic_model->getRequestedThrottleAcc();
  debugMsg.requested_steering_acc = argo_kinematic_model->getRequestedSteeringAcc();
  debugMsg.throttle_limited = argo_kinematic_model->isThrottleLimited();
  debugMsg.steering_limited = argo_kinematic_model->isSteeringLimited();
  kinematicDebugPub->publish(debugMsg);
}

void CtlPlatformNode::timerWellsSpeedCallback() {
  rclcpp::Time current_time = this->now();
  double elapsed_time = (current_time - last_twist_time_).seconds();

  if (elapsed_time > MESSAGE_TIMEOUT) {
      // The last Twist message is considered old, log a warning and send 0,0 commands
      // RCLCPP_WARN(this->get_logger(), "The last Twist message is too old (%.2f seconds). Sending 0,0 commands.", elapsed_time);
      if(pOpMode == PlatformOperationMode::LCM)
        lcmInterface->sendThrottleMsg(0.0, 0.0);
      publishCmdThrottle(0.0, 0.0);
      publishKinematicDebug(0.0, 0.0);
  } else {
      // Process the message as usual
      argo_kinematic_model->update(target_x_, target_rot_);
      const double throttle = argo_kinematic_model->getThrottle();
      const double steering = argo_kinematic_model->getSteering();
      if(pOpMode == PlatformOperationMode::LCM) {
        lcmInterface->sendThrottleMsg(throttle, steering);
        // RCLCPP_INFO(get_logger(),"Throttle: %f, Steering: %f", throttle, steering);
      }  
      publishCmdThrottle(throttle, steering);
      publishKinematicDebug(throttle, steering);
  }
  freq_diag_->tick();
}

void CtlPlatformNode::lcmStatus(diagnostic_updater::DiagnosticStatusWrapper &status) {

  if(this->get_current_state().id() == lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE) {
      switch (platformHeartbeatMsg.state)
      {
        case int8_t(argj801_lcm::PlatformState::EmergencyStop):
          status.add("Platform Status","Emergency Stop");
          break;
        case int8_t(argj801_lcm::PlatformState::Stopped):
          status.add("Platform Status","Stopped");
          break;
        case int8_t(argj801_lcm::PlatformState::LinearMoving):
          status.add("Platform Status","Moving");
          break;
      default:
        break;
      }
      status.add("Taget X",target_x_);
      status.add("Taget Rot",target_rot_);
      status.summary(diagnostic_msgs::msg::DiagnosticStatus::OK, "CONTROL PLATFORM LCM STATUS");
  }
  else {
    status.add("CONTROL PLATFORM LCM ACTIVE", false);
    status.summary(diagnostic_msgs::msg::DiagnosticStatus::OK, "CONTROL PLATFORM LCM STATUS NOT CONFIGURED");
  }
}
