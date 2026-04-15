#include "ctl_mission/Pure_Pursuit.h"


PurePursuit::PurePursuit(){
     look_ahead_distance = 2.0;
     forward_speed = 1.0;
     current_speed = forward_speed;
     min_speed = 0.5;
     breaking_acc = 1.0;

}

    void PurePursuit::control_config( float look_ahead_distance, float forward_speed){

     this->look_ahead_distance = look_ahead_distance;
     this->forward_speed = forward_speed;

     }

    void PurePursuit::control_init(double dt, float look_ahead_distance, float forward_speed, float min_speed, float breaking_acc){
        this->current_speed = forward_speed;
        this->dt = dt;
        this->look_ahead_distance = look_ahead_distance;
        this->forward_speed = forward_speed;
        this->min_speed = min_speed;
        this->breaking_acc = breaking_acc;
    }

int PurePursuit::find_goal_point(const nav_msgs::msg::Path& path, int min_dist_idx) {
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

int PurePursuit::get_idx_min_distance_to_path(const nav_msgs::msg::Path& path) {
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

double PurePursuit::calculateTotal2DDistance(const nav_msgs::msg::Path& path) {
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
    void  PurePursuit::calc_pure_pursuit_actions(const nav_msgs::msg::Path& pose_vector, float dist_last_obj){


        int min_idx_point = get_idx_min_distance_to_path(pose_vector);
        geometry_msgs::msg::PoseStamped  min_dist_pose = pose_vector.poses[min_idx_point];  
        float cte = sqrt(pow(min_dist_pose.pose.position.x,2)+pow(min_dist_pose.pose.position.y,2));
        int goal = find_goal_point(pose_vector, 1);
        geometry_msgs::msg::PoseStamped  look_ahead_pose = pose_vector.poses[goal];
        float yaw_error= tf2::getYaw(look_ahead_pose.pose.orientation) ;
	    float distance_to_look_ahead= sqrt(pow(look_ahead_pose.pose.position.x,2)+pow(look_ahead_pose.pose.position.y,2));
        geometry_msgs::msg::PoseStamped last_objective = pose_vector.poses.back();
        float distance_to_last_obj = sqrt(pow(last_objective.pose.position.x,2)+pow(last_objective.pose.position.y,2));
        // Calculate deceleration distance
         double stopping_acc = 2.0;
        double safetyFactor = 1.0;  // Example safety factor
        double stopDistance = safetyFactor * (this->forward_speed * this->forward_speed) / (2 * stopping_acc) + 1.0; //Add 1.0, becuase the robot stops a 1.0 meters of last waypoint
        double minSpeed = 0.5;
        double remainingDistance  = calculateTotal2DDistance(pose_vector) - 1.0; //minus 1, the robot stops at 1 m from last waypoint
        //double stopDistance =  (this->current_speed * this->current_speed) / (2 * this->breaking_acc);
        std::cout<<" stopDistance " << stopDistance<<std::endl;
        double min_speed = 0.5;
        if (dist_last_obj < stopDistance ) {
           std::cout<<"STOPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPmm " <<std::endl;
            double targetSpeed =this->current_speed -  stopping_acc * this->dt;
             
          std::cout<<"targetSpeed " << targetSpeed<<std::endl;
           this->current_speed = targetSpeed;
           if (this->current_speed < this->min_speed) this->current_speed = this->min_speed;        }
        else{
            this->current_speed = this->forward_speed;
        }
        float angular_speed = look_ahead_pose.pose.position.y*2/(this->look_ahead_distance*this->look_ahead_distance) * this->current_speed; //Calculo w_obj para esa velocidad

        this->pure_pursuit_output.angular_sp = angular_speed;
        this->pure_pursuit_output.linear_sp = this->current_speed;
        this->pure_pursuit_output.goal_idx = goal;
        this->pure_pursuit_output.dist_last_obj = distance_to_last_obj;
        this->pure_pursuit_output.look_ahead_pose = look_ahead_pose;
        this->pure_pursuit_output.cte = cte;

    }
