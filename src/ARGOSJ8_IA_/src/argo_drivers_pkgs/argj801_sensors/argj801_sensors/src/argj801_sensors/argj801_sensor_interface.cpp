    #include "argj801_sensors/argj801_sensors_interface.hpp"
    
    using namespace ARGJ801Sensors;

    ARGJ801SensorsInterface::ARGJ801SensorsInterface() {
      compositeLidarVelodine = std::make_shared<SensorData::Composite>();
      compositeLidarSick = std::make_shared<SensorData::Composite>();
      compositeDriveLine = std::make_shared<SensorData::Composite>();
    }

    
    ARGJ801SensorsInterface::~ARGJ801SensorsInterface() {
      stop();
    }

    std::string ARGJ801SensorsInterface::getID() {
      return "ARGJ801_sensors";
    }

    void ARGJ801SensorsInterface::createPreviousConfig(std::string config_file) {
      lcm_config_file = config_file;
    }

    void ARGJ801SensorsInterface::createVelocineLidarData(std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher, std::string frame_id) {
      velodineLidarData = std::make_shared<SensorData::LidarData>(publisher, frame_id);
      compositeLidarVelodine->addLidar(velodineLidarData);

    }

    void ARGJ801SensorsInterface::createSickLidarData(std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> publisher, std::string frame_id) {
      sickLidarData = std::make_shared<SensorData::LidarData>(publisher, frame_id);
      compositeLidarSick->addLidar(sickLidarData);
    }

    void ARGJ801SensorsInterface::createLeftMotorTelemetryData(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher, std::string frame_id) {
      leftMotorTelemetry = std::make_shared<SensorData::MotorTelemetry>(publisher, frame_id);
      compositeDriveLine->addDriveLine(leftMotorTelemetry);
    }

    void ARGJ801SensorsInterface::createRightMotorTelemetryData(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::MotorTelemetry>> publisher, std::string frame_id) {
      rightMotorTelemetry = std::make_shared<SensorData::MotorTelemetry>(publisher, frame_id);
      compositeDriveLine->addDriveLine(rightMotorTelemetry);
    }

    void ARGJ801SensorsInterface::createOdometerData(std::shared_ptr<rclcpp::Publisher<argj801_sensors_msgs::msg::Odometer>> publisher, std::string frame_id) {
      odometerData = std::make_shared<SensorData::OdometerData>(publisher, frame_id);
      compositeDriveLine->addDriveLine(odometerData);
    }
    void ARGJ801SensorsInterface::createTwistData(std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> publisher, std::string frame_id) {
      twist = std::make_shared<SensorData::Twist>(publisher, frame_id);
      compositeDriveLine->addDriveLine(twist);
    }

    void ARGJ801SensorsInterface::start() {
      std::string lcm_config_path;
      
      lcm_config_path = ament_index_cpp::get_package_share_directory("argj801_sensors");
      lcm_config_path = lcm_config_path + "/../argj801_lcm/config/" + lcm_config_file;
      
      lcmInterface = std::make_shared<argj801_lcm::LCMInterface>(lcm_config_path,true,true,false,false,true,false,false,false,false,false);

      lcmmessages::request_connection_response_msg requestConnectionResponseMsg;

      lcmInterface->sendRequestConnectionMsg(argj801_lcm::RequestConnectionType::Monitor);
      requestConnectionResponseMsg = lcmInterface->receiveRequestConnectionResponseMsg(1000);
      if(!requestConnectionResponseMsg.granted && requestConnectionResponseMsg.connection_type != int8_t(argj801_lcm::RequestConnectionType::Control)) 
        throw argj801_lcm::LCMException("Could not get control of the platform");
    }

    void ARGJ801SensorsInterface::stop() {
      lcmInterface->sendRequestConnectionMsg(argj801_lcm::RequestConnectionType::DisconectControl); 
      lcmInterface.reset();
    }

    void ARGJ801SensorsInterface::getFastData(std::shared_ptr<Visitor::Visitor> visitor) {
      lcmmessages::dat_lidar_scan_line_msg datLidarScanLineMsg;
      for(int i=0;i<2;i++) {
        if(velodineLidarData || sickLidarData) {
          datLidarScanLineMsg = lcmInterface->receiveDatLidarScanLineMsg(500);
          // printf("Sensor ID: %d\n", datLidarScanLineMsg.sensor_id);
          // printf("Num Ranges: %d\n", datLidarScanLineMsg.num_ranges);
          // printf("sensor_name: %s\n", datLidarScanLineMsg.sensor_name.c_str());
          // printf("timestamp: %lf\n", datLidarScanLineMsg.hdr.timestamp);
          if(datLidarScanLineMsg.sensor_id == int8_t(LidarType::VELODINE) && velodineLidarData) {
            // printf("AA Velodine\n");
            if(velodineFirstTime) {
              velodineFirstTime=false;
            }
            else {
              velodineLidarData->setAngleMax(datLidarScanLineMsg.max_azimuth_angle);
              velodineLidarData->setAngleMin(datLidarScanLineMsg.min_azimuth_angle);
              velodineLidarData->setRangeMax(datLidarScanLineMsg.max_sensor_range);
              velodineLidarData->setRangeMin(0);
              velodineLidarData->setRanges(datLidarScanLineMsg.ranges);
              velodineLidarData->setAngleIncrement((datLidarScanLineMsg.max_azimuth_angle - datLidarScanLineMsg.min_azimuth_angle) / datLidarScanLineMsg.num_ranges);
              velodineLidarData->setScanTime(datLidarScanLineMsg.hdr.timestamp - velodinePreviousTime);
              velodineLidarData->setTimeIncrement(velodineLidarData->getScanTime() / datLidarScanLineMsg.num_ranges);
              velodineLidarData->setTime(datLidarScanLineMsg.hdr.timestamp);

              compositeLidarVelodine->acceptLidar(visitor);
            }
            velodinePreviousTime = datLidarScanLineMsg.hdr.timestamp;
          }
          if(datLidarScanLineMsg.sensor_id == int8_t(LidarType::SICK) && sickLidarData) {
                      // printf("AA ick\n");
            if(sickFirstTime) {
              sickFirstTime=false;
            }
            else {
              sickLidarData->setAngleMax(datLidarScanLineMsg.max_azimuth_angle);
              sickLidarData->setAngleMin(datLidarScanLineMsg.min_azimuth_angle);
              sickLidarData->setRangeMax(datLidarScanLineMsg.max_sensor_range);
              sickLidarData->setRangeMin(0);
              sickLidarData->setRanges(datLidarScanLineMsg.ranges);
              sickLidarData->setAngleIncrement((datLidarScanLineMsg.max_azimuth_angle - datLidarScanLineMsg.min_azimuth_angle) / datLidarScanLineMsg.num_ranges);
              sickLidarData->setScanTime(datLidarScanLineMsg.hdr.timestamp - sickPreviousTime);
              sickLidarData->setTimeIncrement(sickLidarData->getScanTime() / datLidarScanLineMsg.num_ranges);
              sickLidarData->setTime(datLidarScanLineMsg.hdr.timestamp);

              compositeLidarSick->acceptLidar(visitor);
            }
            sickPreviousTime = datLidarScanLineMsg.hdr.timestamp;
          }
        }
      }
    }

    void ARGJ801SensorsInterface::getSlowData(std::shared_ptr<Visitor::Visitor> visitor) {
      lcmmessages::dat_driveline_msg datDriveLineMsg;

      if(leftMotorTelemetry || rightMotorTelemetry || odometerData || twist) {
        datDriveLineMsg = lcmInterface->receiveDatDrivelineMsg(1000);

        if(leftMotorTelemetry) {
          leftMotorTelemetry->setEncoder(datDriveLineMsg.motor_data[0].encoder);
          leftMotorTelemetry->setRpm(datDriveLineMsg.motor_data[0].rpm);
          leftMotorTelemetry->setTime(datDriveLineMsg.hdr.timestamp);
        }
        if(rightMotorTelemetry) {
          rightMotorTelemetry->setEncoder(datDriveLineMsg.motor_data[1].encoder);
          rightMotorTelemetry->setRpm(datDriveLineMsg.motor_data[1].rpm);
          rightMotorTelemetry->setTime(datDriveLineMsg.hdr.timestamp);
        }
        if(odometerData) {
          odometerData->setOdometer(datDriveLineMsg.odometer);
          odometerData->setTime(datDriveLineMsg.hdr.timestamp);
        }
        if(twist) {
          twist->setAngularX(0);
          twist->setAngularY(0);
          twist->setAngularZ(-datDriveLineMsg.yaw_rate);
          twist->setLinearX(datDriveLineMsg.fwd_speed);
          twist->setLinearY(0);
          twist->setLinearZ(0);
          twist->setTime(datDriveLineMsg.hdr.timestamp);
        }
        compositeDriveLine->acceptDriveLine(visitor);
      }
    }