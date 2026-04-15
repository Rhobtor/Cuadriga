#include "ctl_mission/Regulated_Pure_Pursuit.h"


RegulatedPurePursuit::RegulatedPurePursuit(){
     look_ahead_distance = 2.0;
     forward_speed = 1.0;
     current_speed = forward_speed;
     min_speed = 0.5;
     breaking_acc = 1.0;
     r_min = 3.0;
     last_vx_sp = 0.0;

}

    void RegulatedPurePursuit::control_config( float look_ahead_distance, float forward_speed, float r_min){

     this->look_ahead_distance = look_ahead_distance;
     this->forward_speed = forward_speed;
     this->r_min = r_min;

     }

    void RegulatedPurePursuit::params_init(double dt, float look_ahead_distance, float forward_speed, 
                                            float min_speed, float breaking_acc, float r_min){
        this->current_speed = forward_speed;
        this->dt = dt;
        this->look_ahead_distance = look_ahead_distance;
        this->forward_speed = forward_speed;
        this->min_speed = min_speed;
        this->breaking_acc = breaking_acc;
        this->r_min = r_min;
    }

int RegulatedPurePursuit::find_goal_point(const nav_msgs::msg::Path& path, int min_dist_idx) {
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

int RegulatedPurePursuit::get_idx_min_distance_to_path(const nav_msgs::msg::Path& path) {
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
double RegulatedPurePursuit::calculateTotal2DDistance(const nav_msgs::msg::Path& path) {
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
    void  RegulatedPurePursuit::calc_regulated_pure_pursuit_actions(const nav_msgs::msg::Path& pose_vector, float dist_last_obj){


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
        double stopDistance =  1.5 * (this->forward_speed * this->forward_speed) / (2 * stopping_acc) + 1.0;
        std::cout<<" stopDistance " << stopDistance<<std::endl;
        double min_speed = 0.5;
        float curvature = look_ahead_pose.pose.position.y*2/(this->look_ahead_distance  * this->look_ahead_distance ); //Calculo w_obj para esa velocidad
        float radius = fabs(1/curvature);
        float v_forward = this->forward_speed;

        //if (this->current_speed  > dist_last_obj * 0.25 ) {
        //    std::cout<<"STOPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPmm " <<std::endl;

        //    this->current_speed = dist_last_obj * 0.25;
        //           }
        //else{
            if (radius > this->r_min){
                this->current_speed = this->forward_speed;
            }
            else {

                this->current_speed = this->forward_speed * radius / this->r_min ;
            }        
       // }

        if (this->current_speed < this->min_speed) {
                    std::cout<<" minsp " << this->min_speed<<std::endl;
                    std::cout<<" currsp " << this->current_speed <<std::endl;

            this->current_speed = this->min_speed;
                std::cout<<" after " << this->current_speed <<std::endl;

        } 
        float angular_speed = curvature * v_forward;
        std::cout<<" v_forward " << v_forward<<std::endl;
        std::cout<<" angular_speed " << angular_speed<<std::endl;
        std::cout<<" curvature " << curvature<<std::endl;
        std::cout<<" lahead " << this->look_ahead_distance<<std::endl;

        std::cout<<" v_forward this " << this->forward_speed<<std::endl;
        std::cout<<" rmin thid " << this->r_min<<std::endl;
        std::cout<<" lahead " << this->look_ahead_distance<<std::endl;

        double remainingDistance = calculateTotal2DDistance(pose_vector);
        if (dist_last_obj < stopDistance ) {
            std::cout<<"STOPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPmm " <<std::endl;
            this->current_speed = last_vx_sp - stopping_acc * this->dt;
            if (this->current_speed < 0.5) this->current_speed = 0.5;       
         }
        last_vx_sp = this->current_speed;
        this->regulated_pure_pursuit_output.angular_sp = angular_speed;
        this->regulated_pure_pursuit_output.linear_sp = this->current_speed;
        this->regulated_pure_pursuit_output.goal_idx = goal;
        this->regulated_pure_pursuit_output.dist_last_obj = distance_to_last_obj;
        this->regulated_pure_pursuit_output.look_ahead_pose = look_ahead_pose;
        this->regulated_pure_pursuit_output.cte = cte;

    }
