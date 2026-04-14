#include "ctl_mission/Dynamic_LAPP.h"


DynamicLAPP::DynamicLAPP(){
     look_ahead_v_gain = 1.0;
     min_look_ahead_d = 2.0;
     max_speed = 3.0;
     max_ang_acc = 0.4;
     max_ang_dec = 2.0;
     max_lin_acc = 0.52;
     max_lin_dec = 2.0;
     last_wz_sp = 0.0;
     last_vx_sp = 0.0;
     dt = 0.02;
     min_speed = 0.5;
     speed_power = 1.0;
     min_look_ahead_d  = 2.0;

}
 
   void DynamicLAPP::controller_reinit(){
     this->last_wz_sp = 0;
     this->last_vx_sp = 0;
    }

    void DynamicLAPP::params_init( double look_ahead_v_gain, double max_speed, double max_ang_acc, double max_ang_dec, 
                                double max_lin_acc, double max_lin_dec, double speed_pow, double min_look_ahead_d){

     this->look_ahead_v_gain = look_ahead_v_gain;
     this->max_speed = max_speed;
     this->max_ang_acc = max_ang_acc;
     this->max_ang_dec = max_ang_dec;
     this->max_lin_acc = max_lin_acc;
     this->max_lin_dec = max_lin_dec;
     this->speed_power = speed_pow;
     this->min_look_ahead_d = min_look_ahead_d;

    }

    void DynamicLAPP::control_init(double look_ahead_v_gain, double max_speed, double max_ang_acc, double max_ang_dec, 
                                double max_lin_acc, double max_lin_dec, double dt, double min_speed){


     this->look_ahead_v_gain = look_ahead_v_gain;
     this->max_speed = max_speed;
     this->max_ang_acc = max_ang_acc;
     this->max_ang_dec = max_ang_dec;
     this->max_lin_acc = max_lin_acc;
     this->max_lin_dec = max_lin_dec;
     this->dt = dt;
     this->min_speed = min_speed;

    }
    int DynamicLAPP::find_goal_point(const nav_msgs::msg::Path& pose_vector, const double& la_distance  ){
        geometry_msgs::msg::PoseStamped pose ;
        float distance;
        float  min_diff = std::numeric_limits<int>::max();
        int idx = -1;
        float diff;

        int n_points_search = 50;
        int min_lim = this->dynamic_lapp_output.goal_idx - n_points_search;
        int max_lim = this->dynamic_lapp_output.goal_idx + n_points_search;
        if (min_lim < 0) min_lim = 0;
        if(max_lim > pose_vector.poses.size()) max_lim = pose_vector.poses.size();

        for(unsigned int i = min_lim ; i < max_lim; i = i+1){
 
            pose = pose_vector.poses[i];
            distance = sqrt(pow(pose.pose.position.x,2)+pow(pose.pose.position.y,2));

             diff = fabs(distance - la_distance);
            if (diff < min_diff){
                min_diff = diff;
                idx = i;
            }
        }
        if (idx == -1) {
            std::cout<<"Not found point at lookahead distance"<< std::endl;
        }
    return idx;
  }

double DynamicLAPP::calculateTotal2DDistance(const nav_msgs::msg::Path& path) {
    double total_distance = 0.0;
    if (path.poses.size() < 2) {
        return total_distance;  // Return 0.0 if there are less than two points
    }

    for (size_t i = 1; i < path.poses.size(); ++i) {
        const auto& pose1 = path.poses[i - 1].pose.position;
        const auto& pose2 = path.poses[i].pose.position;
        double dx = pose1.x - pose2.x;
        double dy = pose1.y - pose2.y;
        total_distance += sqrt(dx * dx + dy * dy);
    }

    return total_distance;
}


std::pair<double, double> DynamicLAPP::calculateCommands(const geometry_msgs::msg::PoseStamped& look_ahead_pose, double last_vx_sp, double last_wz_sp, double max_ang_acc, double max_lin_acc, double dt, double max_speed) {
    double la_distance = this->min_look_ahead_d + this->look_ahead_v_gain * pow(this->dynamic_lapp_output.linear_sp, this->speed_power);
    double v_obj = std::min(last_vx_sp + max_lin_acc * dt, max_speed); // Calculate desired linear speed
    float w_z_sp = -look_ahead_pose.pose.position.y * 2.0 * v_obj / (la_distance * la_distance) ; // Calculate desired angular speed
    std::cout << "Initial values:" << std::endl;
    std::cout << "  la_distance: " << la_distance << std::endl;
    std::cout << "  y_error: " << -look_ahead_pose.pose.position.y << std::endl;
    std::cout << "  last_vx_sp: " << last_vx_sp << std::endl;
    std::cout << "  last_wz_sp: " << last_wz_sp << std::endl;
    std::cout << "  max_ang_acc: " << max_ang_acc << std::endl;
    std::cout << "  max_lin_acc: " << max_lin_acc << std::endl;
    std::cout << "  dt: " << dt << std::endl;
    std::cout << "  max_speed: " << max_speed << std::endl;
    std::cout << "  v_obj: " << v_obj << std::endl;
    std::cout << "  w_z_sp: " << w_z_sp << std::endl;

    double v_command = last_vx_sp;
    double w_command = last_wz_sp;

    const double dead_zone_threshold = 0.1; // Define a dead zone threshold for angular speed


    if (std::fabs(w_z_sp) < std::fabs(last_wz_sp)) {
        // Decreasing angular speed
        std::cout << "Decreasing angular speed." << std::endl;
        w_command = w_z_sp;
        v_command = std::min(v_command + max_lin_acc * dt, max_speed);
    } else if (std::fabs(w_z_sp) > std::fabs(last_wz_sp)) {
        // Increasing angular speed
        std::cout << "Increasing angular speed." << std::endl;
        if (std::fabs(w_z_sp - last_wz_sp) <= max_ang_acc * dt) {
            // Within angular acceleration limits
            w_command = w_z_sp;
            v_command = std::min(v_command + max_lin_acc * dt, max_speed);
        } else {
            std::cout << "Braking." << std::endl;

            // Exceeds angular acceleration limits
            if (w_z_sp > 0.0) {
                std::cout << "Turning left." << std::endl;
                w_command = last_wz_sp + max_ang_acc * dt;
            } else {
                std::cout << "Turning right." << std::endl;
                w_command = last_wz_sp - max_ang_acc * dt;
            }
            v_command = std::fabs(w_command) * (la_distance * la_distance) / (2 * std::fabs(look_ahead_pose.pose.position.y));
        }
    }

    if (v_command < 0.5) v_command = 0.5; // Ensure minimum command speed

    std::cout << "Final commands:" << std::endl;
    std::cout << "  v_command: " << v_command << std::endl;
    std::cout << "  w_command: " << w_command << std::endl;

    return std::make_pair(v_command, w_command);
}



void DynamicLAPP::calc_dynamic_lapp_actions(const nav_msgs::msg::Path& pose_vector, float dist_last_obj, double v_x_odom, double w_z_odom) {
    double la_distance = this->min_look_ahead_d + this->look_ahead_v_gain * pow(this->dynamic_lapp_output.linear_sp, this->speed_power);
    int goal = find_goal_point(pose_vector, la_distance);
    geometry_msgs::msg::PoseStamped look_ahead_pose = pose_vector.poses[goal];
    float yaw_error = tf2::getYaw(look_ahead_pose.pose.orientation);
    
    geometry_msgs::msg::PoseStamped last_objective = pose_vector.poses[pose_vector.poses.size() - 10];
    float distance_to_last_obj = sqrt(pow(last_objective.pose.position.x, 2) + pow(last_objective.pose.position.y, 2));

    auto commands = calculateCommands(look_ahead_pose, last_vx_sp, last_wz_sp, max_ang_acc, max_lin_acc, dt, max_speed);
    double v_command = commands.first;
    double w_command = commands.second;


    double stopping_acc = 2.0;
    double safetyFactor = 2.0;  // Example safety factor
    double stopDistance = safetyFactor * (v_command * v_command) / (2 * stopping_acc) + 1.0; // Add 1.0, because the robot stops 1.0 meters before the last waypoint
    double minSpeed = 0.5;
    double remainingDistance = calculateTotal2DDistance(pose_vector);
    if (dist_last_obj < stopDistance ) {
        std::cout<<"STOPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPmm " <<std::endl;

       v_command = last_vx_sp - stopping_acc * this->dt;
        if (v_command < 0.5) v_command = 0.5;        }
    last_wz_sp = w_command;
    last_vx_sp = v_command;

    this->dynamic_lapp_output.angular_sp = w_command;
    this->dynamic_lapp_output.linear_sp = v_command;
    this->dynamic_lapp_output.goal_idx = goal;
    this->dynamic_lapp_output.dist_last_obj = distance_to_last_obj;
    this->dynamic_lapp_output.look_ahead_pose = look_ahead_pose;
    this->dynamic_lapp_output.cte = la_distance;


    }