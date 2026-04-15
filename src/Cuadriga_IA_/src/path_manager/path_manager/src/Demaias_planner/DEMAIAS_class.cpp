#include "path_manager/DEMAIAS_class.hpp"


Planner::Planner() : INFINITO(std::numeric_limits<REAL_TYPE>::infinity()),
                     OBSTACLE(std::numeric_limits<REAL_TYPE>::infinity()),
                     EPSILON(1e-10) {
    __suffix_path = new_path_node(INFINITO);
    __prefix_path = new_path_node(0.0);
}

Planner::~Planner() {
    delete __suffix_path;
    delete __prefix_path;
}

std::string Planner::__nodeKey(const prodNode* node) {
    return std::to_string(node->pose.x + node->pose.y * __dimX + node->Qstate * __dimX * __dimY);
}

Planner::__PATH* Planner::new_path_node(REAL_TYPE ini_cost) {
    __PATH* node = new __PATH;
    node->id = 0;
    node->total_cost = ini_cost;
    node->iterations = 0;
    return node;
}

Planner::prodNode* Planner::new_prod_graph_node(__SYS_STATE pose, int Qstate) {
    prodNode* node = new prodNode;
    node->pose = pose;
    node->Qstate = Qstate;
    node->g = 0.0;
    node->h = 0.0;
    node->f = 0.0;
    node->parent = nullptr;
    return node;
}

REAL_TYPE Planner::euclidean(std::vector<REAL_TYPE> pA, std::vector<REAL_TYPE> pB) {
    REAL_TYPE dist = 0.0;
    for (FOR_TYPE i = 0; i < (FOR_TYPE)pA.size(); i++) dist += std::pow(pB[i] - pA[i], 2);
    return std::sqrt(dist);
}

Planner::__SYS_STATE Planner::_utm_to_xy(const std::vector<REAL_TYPE>& utm) {
    __SYS_STATE result;
    result.x = static_cast<REAL_TYPE>(round((utm[0] - __minX) / __DEM_resolution));
    result.y = static_cast<REAL_TYPE>(round((utm[1] - __minY) / __DEM_resolution));
    return result;
}

KEY_TYPE Planner::__keyGrid(const __SYS_STATE& point) {
    return KEY_TYPE(point.x + point.y * __dimX);
}

void Planner::create_automaton_from_ltl(const char* command) {
    // Implementation for creating automaton from LTL formula
    // This is a placeholder and should be replaced with actual implementation
    std::system(command);
}

Planner::arguments* Planner::initialize(const std::string& dem_filename, const std::string& proposition_map_filename,
                                        const std::string& mission_filename, const std::string& orthophoto_filename) {
    // Implementation of the initialize method
    std::cout << "Initializing with DEM: " << dem_filename << ", Mapping: " << proposition_map_filename << ", LTL: " << mission_filename << ", Orthophoto: " << orthophoto_filename << std::endl;

    auto args = new arguments();

    // Read DEM file
    std::ifstream dem_file(dem_filename);
    if (!dem_file.is_open()) {
        throw std::runtime_error("Failed to open DEM file: " + dem_filename);
    }
    std::string line;
    while (std::getline(dem_file, line)) {
        // Process DEM line
    }
    dem_file.close();

    // Read proposition map file
    std::ifstream map_file(proposition_map_filename);
    if (!map_file.is_open()) {
        throw std::runtime_error("Failed to open mapping file: " + proposition_map_filename);
    }
    while (std::getline(map_file, line)) {
        std::cout << "Reading Mapping line: " << line << std::endl;
        // Process mapping line
    }
    map_file.close();

    // Read LTL mission file
    std::ifstream ltl_file(mission_filename);
    if (!ltl_file.is_open()) {
        throw std::runtime_error("Failed to open LTL file: " + mission_filename);
    }
    while (std::getline(ltl_file, line)) {
        std::cout << "Reading LTL line: " << line << std::endl;
        // Process LTL line
    }
    ltl_file.close();

    // Read orthophoto file if needed
    if (!orthophoto_filename.empty()) {
        std::ifstream ortho_file(orthophoto_filename);
        if (!ortho_file.is_open()) {
            throw std::runtime_error("Failed to open orthophoto file: " + orthophoto_filename);
        }
        while (std::getline(ortho_file, line)) {
            // Process orthophoto line
        }
        ortho_file.close();
    }
    std::cout << "Init done "  << std::endl;

    return args;
}

double Planner::distance(const geometry_msgs::msg::Point& p1, const geometry_msgs::msg::Point& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2) + std::pow(p1.z - p2.z, 2));
}
void Planner::compute_accepting_product_automaton_states() {
    // Implementation of the function
}

std::deque<Planner::__PATH*> Planner::compute_suffix_prefix_paths(bool cycle) {
    // Implementation of the function
}

Planner::__PATH* Planner::path_search(prodNode* srcNode, std::vector<prodNode*> destNodes, REAL_TYPE cost) {
    // Implementation of the function
}

bool Planner::isEqual(prodNode* nodeA, prodNode* nodeB) {
    return __nodeKey(nodeA) == __nodeKey(nodeB);
}

#ifdef VERBOSE_MATLAB
void Planner::write_MATfile(std::vector<REAL_TYPE>) {
    // Implementation for writing MATLAB files
}
#endif
void Planner::dumb_fnc(void){
         std::cout << "dumb: " ;

}

std::pair<nav_msgs::msg::Path, nav_msgs::msg::Path> Planner::plan(const geometry_msgs::msg::Point& start,
                                                                  const geometry_msgs::msg::Point& goal) {


    nav_msgs::msg::Path path;
    geometry_msgs::msg::PoseStamped pose;
    pose.pose.orientation.w = 1.0;  // Orientation set to 0 (quaternion w = 1)

    double total_distance = distance(start, goal);
    int num_points = static_cast<int>(total_distance);

    double dx = (goal.x - start.x) / total_distance;
    double dy = (goal.y - start.y) / total_distance;
    double dz = (goal.z - start.z) / total_distance;

    for (int i = 0; i <= num_points; ++i) {
        pose.pose.position.x = start.x + i * dx;
        pose.pose.position.y = start.y + i * dy;
        pose.pose.position.z = start.z + i * dz;
        path.poses.push_back(pose);
    }

    return std::make_pair(path, nav_msgs::msg::Path());
}
