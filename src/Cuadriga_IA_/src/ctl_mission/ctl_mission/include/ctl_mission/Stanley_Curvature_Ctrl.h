#include <vector>
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include <tf2/utils.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
class Stanley_Curvature_Ctrl
{
private:
    float look_ahead_distance ,K_angle,K_position,forward_speed;
    int points_in_window, min_goal_idx;
public:
    Stanley_Curvature_Ctrl();
    struct {             // controller output structure declaration
        float linear_sp;         
        float angular_sp;
        int goal_idx;
        geometry_msgs::msg::PoseStamped look_ahead_pose;
        float cte;
        float dist_last_obj;   
    } stanley_output;        
    void params_init(float look_ahead_distance, float K_angle, float K_position, float forward_speed, int points_in_window);
    //Stanley_Curvature_Ctrl(float look_ahead_distance, float K_angle, float K_position, float forward_speed, int points_in_window );
    int  get_idx_min_distance_to_path(std::vector <geometry_msgs::msg::PoseStamped> tf2_point_vector, int min_idx_point);
    int find_goal_point(std::vector <geometry_msgs::msg::PoseStamped> tf2_point_vector, int min_dist_idx);
    int find_goal_point_n(std::vector <geometry_msgs::msg::PoseStamped> pose_vector,  int min_dist_idx, float look_ahead);
    float calculate_curvature(const std::vector<geometry_msgs::msg::PoseStamped>& pose_vector, int index);
    float calculate_average_curvature(const std::vector<geometry_msgs::msg::PoseStamped>& pose_vector);
    void calc_stanley_actions(std::vector <geometry_msgs::msg::PoseStamped> pose_vector, std::vector <geometry_msgs::msg::PoseStamped> utm_vector);
    
    float fit_circle(const std::vector<cv::Point2d>& pnts);
    float get_points_radius(std::vector <geometry_msgs::msg::PoseStamped> pose_vector, int min_point_idx);
    float calc_l_ahead(float radius);
    void control_init();
};


