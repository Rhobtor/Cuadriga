#include "ctl_mission/Stanley_Ctrl.h"


Stanley_Ctrl::Stanley_Ctrl(){
     look_ahead_distance = 3.0;
     K_angle = 1.0;
     K_position = 0.7;
     forward_speed = 1.0;

}

    void Stanley_Ctrl::params_init( float look_ahead_distance, float K_angle, float K_position, float forward_speed){

     this->look_ahead_distance = look_ahead_distance;
     this->K_angle = K_angle;
     this->K_position = K_position;
     this->forward_speed = forward_speed;
     }

    void Stanley_Ctrl::control_init(){

    }

int Stanley_Ctrl::find_goal_point(const nav_msgs::msg::Path& path, int min_dist_idx) {
    float min_diff = std::numeric_limits<float>::max();
    int idx = -1;
    float diff;
    int limit = path.poses.size();
    if (limit > path.poses.size()) limit = path.poses.size(); // This check is redundant, consider removing it

    for(unsigned int i = min_dist_idx; i < limit; i++) {
        const auto& pose = path.poses[i];
        float distance = std::sqrt(std::pow(pose.pose.position.x, 2) + std::pow(pose.pose.position.y, 2));
        diff = std::fabs(distance - this->look_ahead_distance);
        if (diff < min_diff) {
            min_diff = diff;
            idx = i;
        }
    }

    if (idx == -1) {
        idx = min_dist_idx;
        std::cout << "Not found point at lookahead distance" << std::endl;
    }
    return idx;
}

int Stanley_Ctrl::get_idx_min_distance_to_path(const nav_msgs::msg::Path& path) {
    float min_dist = std::numeric_limits<float>::max();
    int idx = -1;
    float distance;

    for(unsigned int i = 0; i < path.poses.size(); i++) {
        const auto& pose = path.poses[i];
        distance = std::sqrt(std::pow(pose.pose.position.x, 2) + std::pow(pose.pose.position.y, 2));
        if (distance < min_dist) {
            min_dist = distance;
            idx = i;
        }
    }
    return idx;
}

    void  Stanley_Ctrl::calc_stanley_actions(const nav_msgs::msg::Path& pose_vector){


        int min_idx_point = get_idx_min_distance_to_path(pose_vector);
        geometry_msgs::msg::PoseStamped  min_dist_pose = pose_vector.poses[min_idx_point];  

        float cte = sqrt(pow(min_dist_pose.pose.position.x,2)+pow(min_dist_pose.pose.position.y,2));
        int goal = find_goal_point(pose_vector, 1);
        geometry_msgs::msg::PoseStamped  look_ahead_pose = pose_vector.poses[goal];

        float yaw_error= tf2::getYaw(look_ahead_pose.pose.orientation) ;
        std::cout<<"yaw error"<< yaw_error <<std::endl;
	    float distance_to_look_ahead= sqrt(pow(look_ahead_pose.pose.position.x,2)+pow(look_ahead_pose.pose.position.y,2));
        //std::cout<<"dist"<< distance_to_look_ahead <<std::endl;
        std::cout<<"linear_error"<< look_ahead_pose.pose.position.y <<std::endl;
        std::cout<<"goal "<< goal <<std::endl;


        geometry_msgs::msg::PoseStamped last_objective = pose_vector.poses.back();
        float distance_to_last_obj = sqrt(pow(last_objective.pose.position.x,2)+pow(last_objective.pose.position.y,2));
        /*if (distance_to_last_obj < 5.0 ){
           this->stanley_output.linear_sp = (5.0 - distance_to_last_obj);
            }
        else   this->stanley_output.linear_sp = this->forward_speed ;*/


        float angular_speed = this->K_position *look_ahead_pose.pose.position.y;
        /*if (fabs(angular_speed) > this->max_angular_sp){
            if (angular_speed > 0) angular_speed = this->max_angular_sp;
            else angular_speed = -this->max_angular_sp;
        }*/
        this->stanley_output.angular_sp = angular_speed;
        
        this->stanley_output.goal_idx = goal;
        this->stanley_output.dist_last_obj = distance_to_last_obj;
        this->stanley_output.look_ahead_pose = look_ahead_pose;
        this->stanley_output.cte = cte;

    }
