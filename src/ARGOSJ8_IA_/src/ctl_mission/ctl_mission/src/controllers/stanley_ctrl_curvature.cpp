#include "ctl_mission/Stanley_Curvature_Ctrl.h"


Stanley_Curvature_Ctrl::Stanley_Curvature_Ctrl(){
     look_ahead_distance = 1.0;
     K_angle = 1.0;
     K_position = 0.2;
     forward_speed = 1.0;
     points_in_window = 20;

}

    void Stanley_Curvature_Ctrl::params_init( float look_ahead_distance, float K_angle, float K_position, float forward_speed, int points_in_window){

     this->look_ahead_distance = look_ahead_distance;
     this->K_angle = K_angle;
     this->K_position = K_position;
     this->forward_speed = forward_speed;
     this->points_in_window = points_in_window;

    }

    void Stanley_Curvature_Ctrl::control_init(){

     this->min_goal_idx = 0;
    }

    float  Stanley_Curvature_Ctrl::get_points_radius(std::vector <geometry_msgs::msg::PoseStamped> pose_vector, int min_point_idx){

        int lower_limit = min_point_idx - this->points_in_window;
        if (lower_limit < 0) lower_limit = 0; 

        int upper_limit = min_point_idx + this->points_in_window;
        if (upper_limit >= pose_vector.size()) upper_limit = pose_vector.size(); 
        std::vector<cv::Point2d> points;
        cv::Point2d point;

        for(unsigned int i = lower_limit; i < upper_limit; i = i+1){
            point.x = double(pose_vector[i].pose.position.x);
            point.y = double(pose_vector[i].pose.position.y);

            points.push_back(point);

        }

        float radius = fit_circle(points);

        return radius;
    }

float Stanley_Curvature_Ctrl::fit_circle(const std::vector<cv::Point2d> &pnts)
{
    int cols = 3;
    cv::Mat X( static_cast<int>(pnts.size()), cols, CV_64F );
    cv::Mat Y( static_cast<int>(pnts.size()), 1, CV_64F );
    cv::Mat C;

    if (int(pnts.size()) >= 3 )
    {
        for (size_t i = 0; i < pnts.size(); i++)
        {
            X.at<double>(static_cast<int>(i),0) = 2 * pnts[i].x;
            X.at<double>(static_cast<int>(i),1) = 2 * pnts[i].y;
            X.at<double>(static_cast<int>(i),2) = -1.0;
            Y.at<double>(static_cast<int>(i),0) = (pnts[i].x * pnts[i].x + pnts[i].y * pnts[i].y);
        }
        cv::solve(X,Y,C,cv::DECOMP_SVD);
    }
    std::vector<double> coefs;
    C.col(0).copyTo(coefs);

    float radius = sqrt ( coefs[0] * coefs[0] + coefs[1] * coefs[1] - coefs[2] );
    return radius;
}


    int Stanley_Curvature_Ctrl::find_goal_point_n(std::vector <geometry_msgs::msg::PoseStamped> pose_vector,  int min_dist_idx, float look_ahead){
        geometry_msgs::msg::PoseStamped pose ;
        float distance;
        float  min_diff = std::numeric_limits<int>::max();
        int idx = -1;
        float diff;
        int limit = min_dist_idx + this->points_in_window;
        if (limit >= pose_vector.size()) limit = pose_vector.size(); 
        for(unsigned int i = min_dist_idx; i < limit; i = i+1){
 
            pose = pose_vector[i];
            distance = sqrt(pow(pose.pose.position.x,2)+pow(pose.pose.position.y,2));

             diff = fabs(distance-look_ahead);
            if (diff < min_diff){
                min_diff = diff;
                idx = i;
            }
        }
        if (idx == -1) {
            idx = min_dist_idx; 
            std::cout<<"Not found point at lookahead distance"<< std::endl;
        }
    return idx;
  }

    int Stanley_Curvature_Ctrl::find_goal_point(std::vector <geometry_msgs::msg::PoseStamped> pose_vector,  int min_dist_idx ){
        geometry_msgs::msg::PoseStamped pose ;
        float distance;
        float  min_diff = std::numeric_limits<int>::max();
        int idx = -1;
        float diff;
        int limit = min_dist_idx + this->points_in_window;
        if (limit >= pose_vector.size()) limit = pose_vector.size(); 
        for(unsigned int i = min_dist_idx; i < limit; i = i+1){
 
            pose = pose_vector[i];
            distance = sqrt(pow(pose.pose.position.x,2)+pow(pose.pose.position.y,2));

             diff = fabs(distance-this->look_ahead_distance);
            if (diff < min_diff){
                min_diff = diff;
                idx = i;
            }
        }
        if (idx == -1) {
            idx = min_dist_idx; 
            std::cout<<"Not found point at lookahead distance"<< std::endl;
        }
    return idx;
  }

   float Stanley_Curvature_Ctrl::calc_l_ahead(float radius){
        float l_ahead_dist = radius*0.5;
        float l_ahead_max = 10.0;
        float l_ahead_min = 2.0;

        if (l_ahead_dist > l_ahead_max) l_ahead_dist = l_ahead_max;
        if (l_ahead_dist < l_ahead_min) l_ahead_dist = l_ahead_min;
        return l_ahead_dist;
    }
    float Stanley_Curvature_Ctrl::calculate_curvature(const std::vector<geometry_msgs::msg::PoseStamped>& pose_vector, int index) {
        const float EPSILON = 1e-6;

        // Ensure we have enough points to calculate derivatives
        if (index < 1 || index >= pose_vector.size() - 1) {
            return 0.0;  // Not enough points
        }

        // Extract relevant points
        const auto& p0 = pose_vector[index - 1].pose.position;
        const auto& p1 = pose_vector[index].pose.position;
        const auto& p2 = pose_vector[index + 1].pose.position;

        // Calculate first derivatives
        float x_prime = (p2.x - p0.x) / 2.0;
        float y_prime = (p2.y - p0.y) / 2.0;

        // Calculate second derivatives
        float x_double_prime = p2.x - 2.0 * p1.x + p0.x;
        float y_double_prime = p2.y - 2.0 * p1.y + p0.y;

        // Calculate curvature
        float curvature_numerator = x_prime * y_double_prime - x_double_prime * y_prime;
        float curvature_denominator = std::pow(x_prime * x_prime + y_prime * y_prime, 1.5);

        // Avoid division by zero
        if (std::fabs(curvature_denominator) < EPSILON) {
            return 0.0;
        }

        return curvature_numerator / curvature_denominator;
    }


 float Stanley_Curvature_Ctrl::calculate_average_curvature(const std::vector<geometry_msgs::msg::PoseStamped>& pose_vector) {
    float total_curvature = 0.0;

    // Ensure min_goal_idx is valid
    if (this->min_goal_idx < 0 || this->min_goal_idx + this->points_in_window >= pose_vector.size()) {
        return 0.0;  // Invalid range
    }

    // Loop over the specified range and accumulate curvatures
    for (int i = this->min_goal_idx; i < this->min_goal_idx + this->points_in_window; ++i) {
        total_curvature += calculate_curvature(pose_vector, i);
    }

    // Calculate average curvature
    return total_curvature / static_cast<float>(this->points_in_window);
}

    int Stanley_Curvature_Ctrl::get_idx_min_distance_to_path(std::vector <geometry_msgs::msg::PoseStamped> pose_vector, int min_idx_point){

        float  min_dist = std::numeric_limits<int>::max();
        int idx = -1;
        geometry_msgs::msg::PoseStamped pose ;
        float distance;
        for(unsigned int i = min_idx_point; i < pose_vector.size(); i = i+1){
            pose = pose_vector[i];
            distance = sqrt(pow(pose.pose.position.x,2)+pow(pose.pose.position.y,2));
            if (distance < min_dist){
                min_dist = distance;
                idx = i;
         }
        }
        return idx;
    }

    void  Stanley_Curvature_Ctrl::calc_stanley_actions(std::vector <geometry_msgs::msg::PoseStamped> pose_vector, std::vector <geometry_msgs::msg::PoseStamped> utm_vector){

        int min_idx_point = get_idx_min_distance_to_path(pose_vector,this->min_goal_idx);
        geometry_msgs::msg::PoseStamped  min_dist_pose = pose_vector[min_idx_point];       
        float cte = sqrt(pow(min_dist_pose.pose.position.x,2)+pow(min_dist_pose.pose.position.y,2));
        int goal = find_goal_point(pose_vector, 0);
        float radius = get_points_radius(utm_vector, this->min_goal_idx);
        float new_la = calc_l_ahead(radius);
         //std::cout << "radius " << radius<< std::endl;  
         //std::cout << "curvature " << calculate_average_curvature(utm_vector)<< std::endl;  
         //std::cout << "min _idx " <<  this->min_goal_idx<< std::endl;  

        if (goal > this->min_goal_idx) this->min_goal_idx = goal;
        else goal = this->min_goal_idx;
        geometry_msgs::msg::PoseStamped  look_ahead_pose = pose_vector[goal];
        int new_goal = find_goal_point_n(pose_vector, min_idx_point, new_la);

        float yaw_error= tf2::getYaw(look_ahead_pose.pose.orientation);
        geometry_msgs::msg::PoseStamped last_objective = pose_vector.back();
        float distance_to_last_obj = sqrt(pow(last_objective.pose.position.x,2)+pow(last_objective.pose.position.y,2));
        float angular_speed = this->K_angle*yaw_error+this->K_position *look_ahead_pose.pose.position.y;
        this->stanley_output.linear_sp = this->forward_speed;
        this->stanley_output.angular_sp = angular_speed;
        this->stanley_output.goal_idx = 0;
        this->stanley_output.dist_last_obj = distance_to_last_obj;
        this->stanley_output.look_ahead_pose = look_ahead_pose;
        this->stanley_output.cte = cte;
    }
