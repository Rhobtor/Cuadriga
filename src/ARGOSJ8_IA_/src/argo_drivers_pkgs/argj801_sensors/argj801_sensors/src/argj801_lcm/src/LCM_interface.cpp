#include "argj801_lcm/LCM_interface.hpp"

using namespace argj801_lcm;

LCMInterface::LCMInterface(std::string configPath,bool data_drive_line_msg,bool data_lidar_scan_line_msg,bool data_platform_heartbeat_msg,bool data_platform_telemetry_msg,bool request_connection_response_msg,bool data_battery_msg, bool data_commponet_state_msg, bool data_cpu_msg,bool data_hardware_status_msg,bool data_temperature_msg) : configPath(configPath),data_drive_line_msg(data_drive_line_msg),data_lidar_scan_line_msg(data_lidar_scan_line_msg),data_platform_heartbeat_msg(data_platform_heartbeat_msg),data_platform_telemetry_msg(data_platform_telemetry_msg),request_connection_response_msg(request_connection_response_msg),data_battery_msg(data_battery_msg),data_component_state_msg(data_commponet_state_msg),data_cpu_msg(data_cpu_msg),data_hardware_status_msg(data_hardware_status_msg),data_temperature_msg(data_temperature_msg)
{   
    loadConfig();
    //To send messages
    lcmHandleExternal = std::make_shared<lcm::LCM>(external_url);
    lcmHandleBroadcast = std::make_shared<lcm::LCM>(broadcast_url);

    if(!lcmHandleExternal->good())
        throw LCMException("LCMInterface External constructor failed to initialize LCM handle.");
    if(!lcmHandleBroadcast->good())
        throw LCMException("LCMInterface Broadcast constructor failed to initialize LCM handle.");


    if(data_drive_line_msg) {
        lcmHandleExternalDriveLine = std::make_shared<lcm::LCM>(external_url);
        if(!lcmHandleExternalDriveLine->good())
            throw LCMException("LCMInterface ExternalDriveLine constructor failed to initialize LCM handle.");
        lcmHandleExternalDriveLine->subscribe("dat_driveline_msg", &LCMInterface::datDrivelineMsgCallback, this);
        sem_init(&semDatDrivelineMsg, 0, 0);
    }    
    if(data_lidar_scan_line_msg) {
        lcmHandleExternalLidar = std::make_shared<lcm::LCM>(external_url);
        if(!lcmHandleExternalLidar->good())
            throw LCMException("LCMInterface ExternalLidar constructor failed to initialize LCM handle.");
        lcmHandleExternalLidar->subscribe("dat_lidar_scan_line_msg", &LCMInterface::datLidarScanLineMsgCallback, this);
        sem_init(&semDatLidarScanLineMsg, 0, 0);
    }
    if(data_platform_heartbeat_msg) {
        lcmHandleBroadcastHeartbeat = std::make_shared<lcm::LCM>(broadcast_url);
        if(!lcmHandleBroadcastHeartbeat->good())
            throw LCMException("LCMInterface BroadcastHeartbeat constructor failed to initialize LCM handle.");
        lcmHandleBroadcastHeartbeat->subscribe("dat_vehicle_heartbeat_msg", &LCMInterface::datPlatformHeartbeatMsgCallback, this);
        sem_init(&semDatPlatformHeartbeatMsg, 0, 0);
    }    
    if(data_platform_telemetry_msg) {
        lcmHandleBroadcastTelemetry = std::make_shared<lcm::LCM>(broadcast_url);
        if(!lcmHandleBroadcastTelemetry->good())
            throw LCMException("LCMInterface BroadcastTelemetry constructor failed to initialize LCM handle.");
        lcmHandleBroadcastTelemetry->subscribe("dat_vehicle_telemetry_msg", &LCMInterface::datPlatformTelemetryMsgCallback, this);
        sem_init(&semDatPlatformTelemetryMsg, 0, 0);
    }    
    if(request_connection_response_msg) {
        lcmHandleExternalRequestConnectionResponse = std::make_shared<lcm::LCM>(external_url);
        if(!lcmHandleExternalRequestConnectionResponse->good())
            throw LCMException("LCMInterface ExternalRequestConnectionResponse constructor failed to initialize LCM handle.");
        lcmHandleExternalRequestConnectionResponse->subscribe("request_connection_response_msg", &LCMInterface::requestConnectionResponseMsgCallback, this);
        sem_init(&semRequestConnectionResponseMsg, 0, 0);
    }
    if(data_battery_msg) {
        lcmHandleExternalBattery = std::make_shared<lcm::LCM>(external_url);
        if(!lcmHandleExternalBattery->good())
            throw LCMException("LCMInterface ExternalBattery constructor failed to initialize LCM handle.");
        lcmHandleExternalBattery->subscribe("dat_battery_msg", &LCMInterface::datBatteryMsgCallback, this);
        sem_init(&semDataBatteryMsg, 0, 0);
    }
    if(data_component_state_msg) {
        lcmHandleExternalComponentState = std::make_shared<lcm::LCM>(external_url);
        if(!lcmHandleExternalComponentState->good())
            throw LCMException("LCMInterface ExternalComponentState constructor failed to initialize LCM handle.");
        lcmHandleExternalComponentState->subscribe("dat_component_state_msg", &LCMInterface::datComponentStateMsgCallback, this);
        sem_init(&semDataCommponetStateMsg, 0, 0);
    }
    if(data_cpu_msg) {
        lcmHandleExternalCpu = std::make_shared<lcm::LCM>(external_url);
        if(!lcmHandleExternalCpu->good())
            throw LCMException("LCMInterface ExternalCpu constructor failed to initialize LCM handle.");
        lcmHandleExternalCpu->subscribe("dat_cpu_msg", &LCMInterface::datCpuMsgCallback, this);
        sem_init(&semDatCpuMsg, 0, 0);
    }
    if(data_hardware_status_msg) {
        lcmHandleExternalHardwareStatus = std::make_shared<lcm::LCM>(external_url);
        if(!lcmHandleExternalHardwareStatus->good())
            throw LCMException("LCMInterface ExternalHardwareStatus constructor failed to initialize LCM handle.");
        lcmHandleExternalHardwareStatus->subscribe("dat_hardware_status_msg", &LCMInterface::datHardwareStatusMsgCallback, this);
        sem_init(&semDatHardwareStatusMsg, 0, 0);
    }
    if(data_temperature_msg) {
        lcmHandleExternalTemperature = std::make_shared<lcm::LCM>(external_url);
        if(!lcmHandleExternalTemperature->good())
            throw LCMException("LCMInterface ExternalTemperature constructor failed to initialize LCM handle.");
        lcmHandleExternalTemperature->subscribe("dat_temperature_msg", &LCMInterface::datTemperatureMsgCallback, this);
        sem_init(&semDatTemperatureMsg, 0, 0);
    }
    start_time = time_helper();

}


LCMInterface::~LCMInterface()
{
    if(data_drive_line_msg) 
        sem_destroy(&semDatDrivelineMsg);
    if(data_lidar_scan_line_msg)
        sem_destroy(&semDatLidarScanLineMsg);
    if(data_platform_heartbeat_msg)
        sem_destroy(&semDatPlatformHeartbeatMsg);
    if(data_platform_telemetry_msg)
        sem_destroy(&semDatPlatformTelemetryMsg);
    if(request_connection_response_msg)
        sem_destroy(&semRequestConnectionResponseMsg);
    if(data_battery_msg)
        sem_destroy(&semDataBatteryMsg);
    if(data_component_state_msg)
        sem_destroy(&semDataCommponetStateMsg);
    if(data_cpu_msg)
        sem_destroy(&semDatCpuMsg);
    if(data_hardware_status_msg)
        sem_destroy(&semDatHardwareStatusMsg);
    if(data_temperature_msg)
        sem_destroy(&semDatTemperatureMsg);
}

void LCMInterface::loadConfig()
{
    YAML::Node config = YAML::LoadFile(configPath);

    std::cout << configPath << std::endl;
    if(config["external_url"])
        external_url = config["external_url"].as<std::string>();
    else
        throw LCMException("LCMInterface failed to load external_url from config file.");
    
    if(config["broadcast_url"])
        broadcast_url = config["broadcast_url"].as<std::string>();
    else
        throw LCMException("LCMInterface failed to load broadcast_url from config file.");
    
    YAML::Node header2 = config["header2"];
    if(header2["component_id"])
        header_from_station.component_id = uint8_t(header2["component_id"].as<int>());
    else
        throw LCMException("LCMInterface failed to load component_id from config file.");
    
    if(header2["vehicle_id"])
        header_from_station.vehicle_id = header2["vehicle_id"].as<int32_t>();
    else
        throw LCMException("LCMInterface failed to load vehicle_id from config file.");
    
    if(header2["station_id"])
        header_from_station.station_id = header2["station_id"].as<int32_t>();
    else
        throw LCMException("LCMInterface failed to load station_id from config file.");
    if(header2["timestamp"])
        header_from_station.timestamp = header2["timestamp"].as<double>();
    else
        throw LCMException("LCMInterface failed to load timestamp from config file.");
    if(header2["sequence_id"])
        header_from_station.sequence_id = header2["sequence_id"].as<int32_t>();
    else
        throw LCMException("LCMInterface failed to load sequence_id from config file.");    

    YAML::Node discrete_device = config["discrete_device"];

    if(discrete_device["subsystem"])
        discreteDevice.subsystem = uint8_t(discrete_device["subsystem"].as<int>());
    else
        throw LCMException("LCMInterface failed to load subsystem from config file.");

    if(discrete_device["automatic"])
        discreteDevice.automatic = int8_t(discrete_device["automatic"].as<bool>());
    else    
        throw LCMException("LCMInterface failed to load automatic from config file.");

    YAML::Node request_connection = config["request_connection"];

    if(request_connection["forced"])
        requestConnectionMsg.forced = int8_t(request_connection["forced"].as<bool>());
    else
        throw LCMException("LCMInterface failed to load forced from config file.");

}

void LCMInterface::sendDiscreteDeviceMsg(DiscreteDeviceType type, bool state)
{
    discreteDeviceMsg.hdr = header_from_station;
    discreteDevice.state = int8_t(state);
    discreteDevice.type = int8_t(type);
    discreteDeviceMsg.device = discreteDevice;

    if(lcmHandleExternal->publish("cmd_discrete_device_msg", &discreteDeviceMsg))
        throw LCMException("LCMInterface failed to publish cmd_discrete_device_msg.");
}

void LCMInterface::sendThrottleMsg(float throttle,float steering) {
    
    throttleMsg.hdr = header_from_station;
    throttleMsg.throttle = throttle;
    throttleMsg.steering = steering;

    if(lcmHandleExternal->publish("cmd_throttle_msg", &throttleMsg))
        throw LCMException("LCMInterface failed to publish cmd_throttle_msg.");
}

void LCMInterface::sendVelocityMsg(float forward_velocity, float angular_velocity) {
        
    velocityMsg.hdr = header_from_station;
    velocityMsg.forward_velocity = forward_velocity;
    velocityMsg.angular_velocity = angular_velocity;

    if(lcmHandleExternal->publish("cmd_velocity_msg", &velocityMsg))
        throw LCMException("LCMInterface failed to publish cmd_velocity_msg.");
}

void LCMInterface::sendStationHeartbeatMsg() {

    stationHeartbeatMsg.hdr = header_from_station;

    if(lcmHandleBroadcast->publish("dat_station_heartbeat_msg", &stationHeartbeatMsg))
        throw LCMException("LCMInterface failed to publish dat_station_heartbeat_msg.");
}

void LCMInterface::sendRequestConnectionMsg(RequestConnectionType connection_type) {

    requestConnectionMsg.hdr = header_from_station;
    requestConnectionMsg.connection_type = int8_t(connection_type);

    if(lcmHandleExternal->publish("request_connection_msg", &requestConnectionMsg))
        throw LCMException("LCMInterface failed to publish request_connection_msg.");
}

lcmmessages::dat_driveline_msg LCMInterface::receiveDatDrivelineMsg(int timeout_ms) {
    int r = lcmHandleExternalDriveLine->handleTimeout(timeout_ms);
    if(r < 0)
        throw LCMException("LCMInterface failed to receive dat_driveline_msg.");
    else if(r == 0)
        throw LCMException("LCMInterface timeout reached in dat_driveline_msg.");
    sem_wait(&semDatDrivelineMsg);
    return datDrivelineMsg;
}

void LCMInterface::datDrivelineMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_driveline_msg* msg) {

    datDrivelineMsg = *msg;
    sem_post(&semDatDrivelineMsg);
}

lcmmessages::dat_lidar_scan_line_msg LCMInterface::receiveDatLidarScanLineMsg(int timeout_ms) {
    int r = lcmHandleExternalLidar->handleTimeout(timeout_ms);

    if(r < 0)
        throw LCMException("LCMInterface failed to receive dat_lidar_scan_line_msg.");
    else if(r == 0)
        throw LCMException("LCMInterface timeout reached in dat_lidar_scan_line_msg.");
    sem_wait(&semDatLidarScanLineMsg);
    start_time = time_helper();
    return datLidarScanLineMsg;
}

void LCMInterface::datLidarScanLineMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_lidar_scan_line_msg* msg) {
    datLidarScanLineMsg = *msg;
    sem_post(&semDatLidarScanLineMsg);
}

lcmmessages::dat_vehicle_heartbeat_msg LCMInterface::receiveDatPlatformHeartbeatMsg(int timeout_ms) {
    int r = lcmHandleBroadcastHeartbeat->handleTimeout(timeout_ms);
    if(r < 0)
        throw LCMException("LCMInterface failed to receive dat_vehicle_heartbeat_msg.");
    else if(r == 0) {
        throw LCMException("LCMInterface timeout reached in dat_vehicle_heartbeat_msg.");
    }    
    sem_wait(&semDatPlatformHeartbeatMsg);
    return datPlatformHeartbeatMsg;
}

void LCMInterface::datPlatformHeartbeatMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_vehicle_heartbeat_msg* msg) {
    datPlatformHeartbeatMsg = *msg;
    sem_post(&semDatPlatformHeartbeatMsg);
}

lcmmessages::dat_vehicle_telemetry_msg LCMInterface::receiveDatPlatformTelemetryMsg(int timeout_ms) {
    int r = lcmHandleBroadcastTelemetry->handleTimeout(timeout_ms);
    
    if(r < 0)
        throw LCMException("LCMInterface failed to receive dat_vehicle_telemetry_msg.");
    else if(r == 0)
        throw LCMException("LCMInterface timeout reached in dat_vehicle_telemetry_msg.");

    sem_wait(&semDatPlatformTelemetryMsg);
    return datPlatformTelemetryMsg;
}

void LCMInterface::datPlatformTelemetryMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_vehicle_telemetry_msg* msg) {
    datPlatformTelemetryMsg = *msg;
    sem_post(&semDatPlatformTelemetryMsg);
}

lcmmessages::request_connection_response_msg LCMInterface::receiveRequestConnectionResponseMsg(int timeout_ms) {
    int r = lcmHandleExternalRequestConnectionResponse->handleTimeout(timeout_ms);

    if(r < 0)
        throw LCMException("LCMInterface failed to receive request_connection_response_msg.");
    else if(r == 0)
        throw LCMException("LCMInterface timeout reached in request_connection_response_msg.");

    sem_wait(&semRequestConnectionResponseMsg);
    return requestConnectionResponseMsg;
}

void LCMInterface::requestConnectionResponseMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::request_connection_response_msg* msg) {
    requestConnectionResponseMsg = *msg;
    sem_post(&semRequestConnectionResponseMsg);
}

double LCMInterface::time_helper()
{
  struct timeval timeofday;
  gettimeofday(&timeofday,NULL);
  return double(timeofday.tv_sec + timeofday.tv_usec /1e6);
}

lcmmessages::dat_cpu_msg LCMInterface::receiveDatCpuMsg(int timeout_ms) {
    int r = lcmHandleExternalCpu->handleTimeout(timeout_ms);

    if(r < 0)
        throw LCMException("LCMInterface failed to receive dat_cpu_msg.");
    else if(r == 0)
        throw LCMException("LCMInterface timeout reached in dat_cpu_msg.");

    sem_wait(&semDatCpuMsg);
    return datCpuMsg;
}

void LCMInterface::datCpuMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_cpu_msg* msg) {
    datCpuMsg = *msg;
    sem_post(&semDatCpuMsg);
}

lcmmessages::dat_hardware_status_msg LCMInterface::receiveDatHardwareStatusMsg(int timeout_ms) {
    int r = lcmHandleExternalHardwareStatus->handleTimeout(timeout_ms);

    if(r < 0)
        throw LCMException("LCMInterface failed to receive dat_hardware_status_msg.");
    else if(r == 0)
        throw LCMException("LCMInterface timeout reached in dat_hardware_status_msg.");

    sem_wait(&semDatHardwareStatusMsg);
    return datHardwareStatusMsg;
}

void LCMInterface::datHardwareStatusMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_hardware_status_msg* msg) {
    datHardwareStatusMsg = *msg;
    sem_post(&semDatHardwareStatusMsg);
}

lcmmessages::dat_temperature_msg LCMInterface::receiveDatTemperatureMsg(int timeout_ms) {
    int r = lcmHandleExternalTemperature->handleTimeout(timeout_ms);

    if(r < 0)
        throw LCMException("LCMInterface failed to receive dat_temperature_msg.");
    else if(r == 0)
        throw LCMException("LCMInterface timeout reached in dat_temperature_msg.");

    sem_wait(&semDatTemperatureMsg);
    return datTemperatureMsg;
}

void LCMInterface::datTemperatureMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_temperature_msg* msg) {
    datTemperatureMsg = *msg;
    sem_post(&semDatTemperatureMsg);
}

lcmmessages::dat_battery_msg LCMInterface::receiveDataBatteryMsg(int timeout_ms) {
    int r = lcmHandleExternalBattery->handleTimeout(timeout_ms);

    if(r < 0)
        throw LCMException("LCMInterface failed to receive dat_battery_msg.");
    else if(r == 0)
        throw LCMException("LCMInterface timeout reached in dat_battery_msg.");

    sem_wait(&semDataBatteryMsg);
    return datBatteryMsg;
}

void LCMInterface::datBatteryMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_battery_msg* msg) {
    datBatteryMsg = *msg;
    sem_post(&semDataBatteryMsg);
}

lcmmessages::dat_component_state_msg LCMInterface::receiveDataCommponetStateMsg(int timeout_ms) {
    int r = lcmHandleExternalComponentState->handleTimeout(timeout_ms);

    if(r < 0)
        throw LCMException("LCMInterface failed to receive dat_component_state_msg.");
    else if(r == 0)
        throw LCMException("LCMInterface timeout reached in dat_component_state_msg.");

    sem_wait(&semDataCommponetStateMsg);
    return datComponentStateMsg;
}

void LCMInterface::datComponentStateMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_component_state_msg* msg) {
    datComponentStateMsg = *msg;
    sem_post(&semDataCommponetStateMsg);
}



