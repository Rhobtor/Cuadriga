#include "ctl_mission/Dynamic_PP.h"

DynamicPP::DynamicPP(){
     look_ahead_distance = 2.0;
     max_speed = 3.0;
     max_ang_acc = 0.4;
     max_ang_dec = 2.0;
     max_lin_acc = 0.52;
     max_lin_dec = 2.0;
     last_wz_sp = 0.0;
     last_vx_sp = 0.0;
     dt = 0.02;
     min_speed = 0.5;
}

    void DynamicPP::params_init( double look_ahead_distance, double max_speed, double max_ang_acc, double max_ang_dec, 
                                double max_lin_acc, double max_lin_dec){

     this->look_ahead_distance = look_ahead_distance;
     this->max_speed = max_speed;
     this->max_ang_acc = max_ang_acc;
     this->max_ang_dec = max_ang_dec;
     this->max_lin_acc = max_lin_acc;
     this->max_lin_dec = max_lin_dec;
    }

    void DynamicPP::control_init(double look_ahead_distance, double max_speed, double max_ang_acc, double max_ang_dec, 
                                double max_lin_acc, double max_lin_dec, double dt, double min_speed){

     this->last_wz_sp = 0;
     this->last_vx_sp = 0;
     this->look_ahead_distance = look_ahead_distance;
     this->max_speed = max_speed;
     this->max_ang_acc = max_ang_acc;
     this->max_ang_dec = max_ang_dec;
     this->max_lin_acc = max_lin_acc;
     this->max_lin_dec = max_lin_dec;
     this->dt = dt;
     this->min_speed = min_speed;
    }

    int DynamicPP::find_goal_point(const nav_msgs::msg::Path& pose_vector ){
        geometry_msgs::msg::PoseStamped pose ;
        float distance;
        float  min_diff = std::numeric_limits<int>::max();
        int idx = -1;
        float diff;
        for(unsigned int i = 0; i < pose_vector.poses.size(); i = i+1){
            pose = pose_vector.poses[i];
            distance = sqrt(pow(pose.pose.position.x,2)+pow(pose.pose.position.y,2));
            diff = fabs(distance - this->look_ahead_distance );
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

    double DynamicPP::calculateTotal2DDistance(const nav_msgs::msg::Path& path) {
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

    double DynamicPP::calculateCrossTrackError(const nav_msgs::msg::Path& path) {
        if (path.poses.empty()) {
            return 0.0; // Return zero if path is empty
        }

        double closest_x_distance = std::numeric_limits<double>::max();
        double cross_track_error = 0.0;
        bool found = false;

        // Find the point on the path that is closest ahead of the robot
        for (const auto& pose : path.poses) {
            double x = pose.pose.position.x;
            double y = pose.pose.position.y;
            // Check if the point is ahead of the robot and closer than previously found points
            if (x >= 0 && x < closest_x_distance) {
                closest_x_distance = x;
                cross_track_error = y;
                found = true;
            }
        }

        if (!found) {
            // If no point is found ahead of the robot, consider the CTE as zero or as the y-value of the last point
            // depending on specific navigation needs or scenarios.
            return 0.0;
        }

        return cross_track_error;
    }

    void  DynamicPP::calc_dynamic_pp_actions(const nav_msgs::msg::Path& pose_vector){

        int goal = find_goal_point(pose_vector);
        double max_ang_dec = 2.0; //ToDO: CHECK THIS VAR
        geometry_msgs::msg::PoseStamped  look_ahead_pose = pose_vector.poses[goal];
        float yaw_error= tf2::getYaw(look_ahead_pose.pose.orientation);
        //float w_z_sp = - this->K_angle*yaw_error - this->K_position *look_ahead_pose.pose.position.y;
        geometry_msgs::msg::PoseStamped last_objective = pose_vector.poses[pose_vector.poses.size() - 10];
        float distance_to_last_obj = sqrt(pow(last_objective.pose.position.x,2)+pow(last_objective.pose.position.y,2));
        bool accelerate = true, keep = false, brake = false;
        double v_obj = last_vx_sp + this->max_lin_acc * this->dt; //Acelero
        if (v_obj > this->max_speed) v_obj = this->max_speed;
        float w_z_sp = -look_ahead_pose.pose.position.y*2/(this->look_ahead_distance*this->look_ahead_distance) * v_obj; //Calculo w_obj para esa velocidad
        double v_command, w_command;
        double w_obj;
        std::cout<<"w_z_sp "<< w_z_sp<<std::endl;
        std::cout<<"last_wz_sp "<<last_wz_sp<<std::endl;
        std::cout<<"resta "<<w_z_sp - last_wz_sp<<std::endl;
        std::cout<<"delts "<< this->max_ang_acc * this->dt<<std::endl;
        std::cout<<"v_obj "<< v_obj<<std::endl;
        //std::cout<<"comp  "<< (fabs(w_z_sp - last_wz_sp) > this->max_ang_acc * this->dt)  <<std::endl ;

        if ((fabs(w_z_sp - last_wz_sp) > this->max_ang_acc * this->dt) && (fabs(w_z_sp) > fabs(last_wz_sp)) )
        {
            //No cumplo para v_obj si acelero, intento mantener

             keep = true;
        }
        else{

                v_command = v_obj;
                w_command = w_z_sp;

        }

        if (keep){
             v_obj = last_vx_sp ; //mantengo
             w_z_sp = -look_ahead_pose.pose.position.y*2/(this->look_ahead_distance * this->look_ahead_distance) * v_obj;
             w_obj = w_z_sp;
 
            if ((fabs(w_z_sp - last_wz_sp) > this->max_ang_acc * this->dt) && (fabs(w_z_sp) > fabs(last_wz_sp)) )
            {
                //No cumplo para v_obj mantengo, tengo que decelerar
                brake = true;
            }
            else{

                v_command = v_obj;
                w_command = w_z_sp;
            }

        }
        if (brake){

            v_obj = last_vx_sp - this->max_lin_dec * this->dt; //tengo que frenar
            w_z_sp = -look_ahead_pose.pose.position.y*2/(this->look_ahead_distance * this->look_ahead_distance) * v_obj;
             
            if ((fabs(w_z_sp - last_wz_sp) > this->max_ang_acc * this->dt) )
            {
                  if ((w_z_sp - last_wz_sp) > 0 && (fabs(w_z_sp) > fabs(last_wz_sp))){
                        w_obj = last_wz_sp + this->dt * this->max_ang_acc;
                    }             
                    else  {
                        w_obj  = last_wz_sp - this->dt * this->max_ang_acc;
                    }

            }
            v_command = v_obj;
            w_command = w_obj;


        }
        if (v_command<0.5) v_command = 0.5;


        double stopping_acc = 0.8;
        double safetyFactor = 2.0;  // Example safety factor
        double stopDistance = safetyFactor * (v_command * v_command) / (2 * stopping_acc) + 1.0; //Add 1.0, becuase the robot stops a 1.0 meters of last waypoint
        double minSpeed = 0.5;
        double remainingDistance  = calculateTotal2DDistance(pose_vector);

        bool start_decelerating, keep_decelerating;
        //if (calculateTotal2DDistance(pose_vector) < stopDistance ) {
        //    std::cout<<"STOPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPmm " <<std::endl;

        //    v_command = last_vx_sp - stopping_acc * this->dt;
        //    if (v_command < 0.5) v_command = 0.5;        }
        last_wz_sp = w_command; 
        last_vx_sp = v_command;
        this->dynamic_pp_output.angular_sp = w_command;
        this->dynamic_pp_output.linear_sp = v_command; 
        this->dynamic_pp_output.goal_idx = goal;
        this->dynamic_pp_output.dist_last_obj = distance_to_last_obj;
        this->dynamic_pp_output.look_ahead_pose = look_ahead_pose;
        this->dynamic_pp_output.cte = calculateCrossTrackError(pose_vector);


    }