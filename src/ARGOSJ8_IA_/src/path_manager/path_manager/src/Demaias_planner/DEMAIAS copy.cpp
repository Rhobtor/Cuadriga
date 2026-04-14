//#ifdef DEMAIAS_ROS2node
	#include "path_manager/DEMAIAS.hpp"
//#endif

namespace DEMAIAS {

// ====================================================================================================================
// Main function for LTL-based strategic UGV mission planner.
// Input arguments: DEM (.dat or xyz), LTL mission (.dat) and atomic propositions mapping (.dat) specification files
// ====================================================================================================================
#ifdef DEMAIAS_ROS2node
	std::pair <nav_msgs::msg::Path, nav_msgs::msg::Path> planner(char* DEM, char* mapping, char* LTL, char* orthophoto) {
#else
	int planner(char* DEM, char* mapping, char* LTL, char* orthophoto) {
#endif

std::function <bool(prodNode*, prodNode*)> isEqual = [](prodNode* nodeA, prodNode* nodeB) {
	return (__nodeKey(nodeA) == __nodeKey(nodeB));
};

// Main code block of function 'planner'.
{
// Initialization, preprocessing of input arguments:
// DEM, conversing from LTL to Buchi automaton and prunning automaton according with atomic proposition mapping relation.
clock_t t = clock();
__inputs = initialize(DEM, mapping, LTL, orthophoto);
clock_t tPreprocessing = clock() - t;

// Computing the reduced set of accepting product automaton states.
t = clock();
compute_accepting_product_automaton_states();
clock_t tComputingSet = clock() - t;

// Computing suffix paths, there may be more that one,
// i.e., shortest cycles paths that involves a grid point satisfying incomming transition of an automaton accepting state.
t = clock();
#ifdef DEMAIAS_ROS2node
	std::pair <nav_msgs::msg::Path, nav_msgs::msg::Path> out_path;
#endif
bool isInfinityPath = false;
for (FOR_TYPE i = 0; i < (FOR_TYPE) __accepting_Qstates.size(); i++) if (__accepting_Qstates[i] == 0 || i > 0) { isInfinityPath = true; break; }
std::deque <__PATH*> suffix_paths = std::deque <__PATH*>(0);
suffix_paths.push_back(new_path_node(INFINITO));
if (isInfinityPath) suffix_paths = compute_suffix_prefix_paths(true);
clock_t tSuffixPlanning = clock() - t;

// Computing prefix paths (the shortest path towards any product automaton states belonging to the suffix paths).
t = clock();
std::deque <__PATH*> prefix_paths = std::deque <__PATH*>(0);
prefix_paths.push_back(new_path_node(INFINITO));

if (std::isfinite(suffix_paths[0]->total_cost)) {

	for (FOR_TYPE n = 0; n < (FOR_TYPE) suffix_paths.size(); n++) {

		__PATH* suffix_path = suffix_paths[n];
		__PATH* prefix_path_candidate = new_path_node(INFINITO);
		prodNode* ini_st = new_prod_graph_node(__inputs->s0,0);
		std::vector <prodNode*> end_sts = std::vector <prodNode*>(0);
		for (FOR_TYPE k = 0; k < (FOR_TYPE) suffix_path->node.size(); k++) end_sts.push_back(suffix_path->node[k]);
		prefix_path_candidate = path_search(ini_st, end_sts, INFINITO);
		prefix_path_candidate->id = n;
		if (prefix_path_candidate->total_cost == prefix_paths[0]->total_cost) {
			prefix_paths.push_back(prefix_path_candidate);

		}
		else if (prefix_path_candidate->total_cost < prefix_paths[0]->total_cost) {
			prefix_paths.clear();
			prefix_paths.push_back(prefix_path_candidate);

		}
	}
	
}
else {
	prefix_paths = compute_suffix_prefix_paths(false);
}
__prefix_path = prefix_paths[0];
__suffix_path = suffix_paths[0];
if (std::isfinite(suffix_paths[0]->total_cost)) __suffix_path = suffix_paths[__prefix_path->id];
clock_t tPrefixPlanning = clock() - t;

// Sorting the suffix path nodes according to the prefix path.
t = clock();
if (std::isfinite(__prefix_path->total_cost) && std::isfinite(__suffix_path->total_cost)) {
	__suffix_path->node.pop_back(); __suffix_path->waypoint.pop_back(); __suffix_path->refSpeed.pop_back();
	while (!isEqual(__suffix_path->node[0],__prefix_path->node[__prefix_path->node.size()-1])) {
		prodNode* nodeChg = __suffix_path->node[0];
		__suffix_path->node.pop_front();
		__suffix_path->node.push_back(nodeChg);
		__suffix_path->waypoint.insert(__suffix_path->waypoint.end(),__suffix_path->waypoint.begin(),__suffix_path->waypoint.begin()+__suffix_path->node[0]->segment.size());
		__suffix_path->waypoint.erase(__suffix_path->waypoint.begin(),__suffix_path->waypoint.begin()+__suffix_path->node[0]->segment.size());
		__suffix_path->refSpeed.insert(__suffix_path->refSpeed.end(),__suffix_path->refSpeed.begin(),__suffix_path->refSpeed.begin()+__suffix_path->node[0]->segment.size());
		__suffix_path->refSpeed.erase(__suffix_path->refSpeed.begin(),__suffix_path->refSpeed.begin()+__suffix_path->node[0]->segment.size());
	}
	if (!isEqual(__suffix_path->node[0],__suffix_path->node[__suffix_path->node.size()-1])) {
		prodNode* nodeChg = __suffix_path->node[0];
		__suffix_path->node.push_back(nodeChg);
		__SYS_STATE waypointChg = __suffix_path->waypoint[0];
		__suffix_path->waypoint.push_back(waypointChg);
		REAL_TYPE refSpeedChg = __suffix_path->refSpeed[0];
		__suffix_path->refSpeed.push_back(refSpeedChg);
	}
}
clock_t tSuffixSorting = clock() - t;


// Showing the resulting prefix and suffix paths.
#if defined(EIGHT_NEIGHBOURHOOD) && !defined(VERBOSE_NONE)
	std::cout << "\033[1;36mWarning: Eight-neighbourhood criterion applied. Remove compiler directive EIGHT_NEIGHBOURHOOD to apply four-neighbourhood and a faster computing.\033[0m" << std::endl;
#endif
#if defined(NON_ANY_ANGLE) && !defined(VERBOSE_NONE)
	std::cout << "\033[1;36mWarning: Non any-angle (A*) algorithm applied. Remove compiler directive NON_ANY_ANGLE to apply the DEM-AIA algorithm.\033[0m" << std::endl;
#endif
#if defined(NON_ASYMMETRIC) && !defined(VERBOSE_NONE)
	std::cout << "\033[1;36mWarning: Symmetric slope estimation applied, as 3Dana algorithm. Remove compiler directive NON_ASYMMETRIC for assymetric slope-awareness, as DEM-AIA algorithm.\033[0m" << std::endl;
#endif
#if !defined(VERBOSE_NONE)
	if (!std::isfinite(__prefix_path->total_cost)) std::cout << "\033[31m\033[1;202mTrajectory (or path) not found that fulfills the given LTL mission.\033[0m" << std::endl;
	else {
		std::cout << "\033[1;34m\033[1;202mTrajectory (or path) found that fulfills the given LTL mission.\033[0m" << std::endl << std::endl;
		std::cout << "\033[1;202mOptimal prefix path\033[0m [ length = " << __prefix_path->node.size() << " system states and " << __prefix_path->waypoint.size() << " waypoints, total iterations = " << __prefix_path->iterations << ", total cost = " << __prefix_path->total_cost << " s ]" << std::endl;

		#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
		if (__DEMtype == 2) std::cout << "\033[1;33m{ (x,y,refSpeed),... }\033[0m";
		else std::cout << "\033[1;33m{ (x,y,z,refSpeed),... }\033[0m";
		if (__prefix_path->waypoint.size() > 0) std::cout << std::endl << "{ ";
		for (FOR_TYPE n = 0; n < (FOR_TYPE) __prefix_path->waypoint.size(); n++) {
			std::cout << "(";
			if (__DEMtype == 2) std::cout << __prefix_path->waypoint[n].x+1 << "," << __prefix_path->waypoint[n].y+1 << "," << __prefix_path->refSpeed[n];
///			else printf("%.10g,%.10g,%.10g", __prefix_path->waypoint[n].x * __DEM_resolution + __minX, __prefix_path->waypoint[n].y * __DEM_resolution + __minY, __prefix_path->waypoint[n].theta);
			else printf("%.10g,%.10g,%.10g,%.10g", __prefix_path->waypoint[n].x * __DEM_resolution + __minX, __prefix_path->waypoint[n].y * __DEM_resolution + __minY, __prefix_path->waypoint[n].z * __DEM_resolution, __prefix_path->refSpeed[n]);
			std::cout << "),";
		}
		if (__prefix_path->waypoint.size() > 0) std::cout << "\b }";
		#endif
		std::cout << std::endl;

		if (std::isfinite(__suffix_path->total_cost)) {
			std::cout << std::endl << "\033[1;202mOptimal suffix path\033[0m [ length = " << __suffix_path->node.size() << " system states and " << __suffix_path->waypoint.size() << " waypoints, total iterations = " << __suffix_path->iterations << ", total cost = " << __suffix_path->total_cost << " s ]" << std::endl;


			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
#endif

// Building the resulting prefix and suffix paths as ROS2 service output.
#ifdef DEMAIAS_ROS2node
	geometry_msgs::msg::PoseStamped new_pose;
	if (std::isfinite(__prefix_path->total_cost)) {
		for (FOR_TYPE n = 0; n < (FOR_TYPE) __prefix_path->waypoint.size(); n++) {
			if (__DEMtype == 2) {
				new_pose.pose.position.x = __prefix_path->waypoint[n].x+1;
				new_pose.pose.position.y = __prefix_path->waypoint[n].y+1;
				// new_pose.pose.speed = __prefix_path->refSpeed[n];
				out_path.first.poses.push_back(new_pose);
			}
			else {
				new_pose.pose.position.x = __prefix_path->waypoint[n].x * __DEM_resolution + __minX;
				new_pose.pose.position.y = __prefix_path->waypoint[n].y * __DEM_resolution + __minY;
				new_pose.pose.position.z = __prefix_path->waypoint[n].z * __DEM_resolution;
				// new_pose.pose.speed = __prefix_path->refSpeed[n];
				out_path.first.poses.push_back(new_pose);
			}
		}
		if (std::isfinite(__suffix_path->total_cost)) {
			for (FOR_TYPE n = 0; n < (FOR_TYPE) __suffix_path->waypoint.size(); n++) {
				if (__DEMtype == 2) {
					new_pose.pose.position.x = __suffix_path->waypoint[n].x+1;
					new_pose.pose.position.y = __suffix_path->waypoint[n].y+1;
					// new_pose.pose.speed = __suffix_path->refSpeed[n];
					out_path.second.poses.push_back(new_pose);
				}
				else {
					new_pose.pose.position.x = __suffix_path->waypoint[n].x * __DEM_resolution + __minX;
					new_pose.pose.position.y = __suffix_path->waypoint[n].y * __DEM_resolution + __minY;
					new_pose.pose.position.z = __suffix_path->waypoint[n].z * __DEM_resolution;
					// new_pose.pose.speed = __suffix_path->refSpeed[n];
					out_path.second.poses.push_back(new_pose);
				}
			}
		}
	}
	return out_path;
#else
	return;
#endif
}

}


// ====================================================================================================================
arguments* initialize(char* dem_filename, char* proposition_map_filename, char* mission_filename, char* orthophoto_filename) {

// --------------------------------------------------------------------------------------------------------------------
std::function <void(char[])> create_automaton_from_ltl = [](char ltl_mission[]) {

// Executing command line passed as function's input argument
//  to obtain automaton equivalent to LTL formula by LTL2TGBA
//  (command line tool into spot-2.11.6).
FILE* fp;
if ((fp = popen(ltl_mission, "r")) == NULL) {
	std::cout << "\033[1;31mError: Failed to execute the command specified. Check the LTL formula in the UGV mission file.\033[0m" << std::endl << std::endl;
	return;
}

// Copying the automaton returned by LTL2TGBA (as a text) to a local variable 'table_text'.
std::string table_text = "";
const int BUFFER_SIZE = 1024;
char buff[BUFFER_SIZE];
if (fgets(buff, BUFFER_SIZE, fp) != NULL) table_text = table_text + buff;

rewind(fp);
table_text = "";
while (fgets(buff, BUFFER_SIZE, fp) != NULL) table_text = table_text + buff;


// Closing the LTL-mission file.
if (pclose(fp)) {
	std::cout << "\033[1;31mError: Check the content of the UGV mission file:" << std::endl << " " << ltl_mission << "\033[0m" << std::endl << std::endl;
	return;
}

// Processing the automaton (contained into 'table_text').
// -------------------------------------------------------
// Identifiying index to the first automaton state into 'table_text'.
int i = 0;  // local variable as index for array/table element queries
while (table_text[i] != 'T' && table_text[i] != 'a') i++;
    
// Adaptation of the automaton (contained into 'table_text') to a local variable 'table' (internal data structure).
std::string tmp;  // local variable as string for a state or logical operator identified
std::unordered_map <std::string, int> state;  // local variable as key-value pair map of automaton states
std::vector <std::string> state_list;  // local variable as list of automaton states with a number (key) associated with each one
std::vector <std::string> table;
while (i < table_text.size()) {
	// making a string to identify state or logical operator into a local variable 'tmp'
	tmp = "";
	while (i < table_text.size() && table_text[i] != ':') tmp += table_text[i++];
	// identifying end of the specification automaton into 'table_text'
	if (tmp[0] == '}') break;
	// identifying automaton states...
	// if needed, insert a new automaton state into local variables 'state' and 'state_list'
	if (state.find(tmp) == state.end()) {
		state[tmp] = state_list.size();
		state_list.push_back(tmp);
	}
	while (i < table_text.size() && tmp != "fi;" && tmp != "od;") {
		tmp = "";
		while (i < table_text.size() && table_text[i] != '\t' && table_text[i] != ' ' && table_text[i] != '\n') {
			if (table_text[i] != '(' && table_text[i] != ')') tmp += table_text[i];
			i++;
		}
		if (tmp != " " && tmp != "" && tmp != "if" && tmp != "do" && tmp != "::" && tmp != "->" && tmp != "goto" && tmp != "fi;" && tmp != "od;") table.push_back(tmp);
		i++;
	}
}
__Qstates_cnt = state.size();

// Identifying automaton accepting states.
__accepting_Qstates = std::vector <int>(0);
for (FOR_TYPE x = 0; x < (FOR_TYPE) state_list.size(); x++) if (state_list[x][0] == 'a' && state_list[x][1] == 'c' && state_list[x][2] == 'c') __accepting_Qstates.push_back(x);

// Save the automaton (contained into 'table_text') returned by LTL2TGBA to the file 'LTL-mission-full-automaton.dat'
// Creating a file 'LTL-mission-full-automaton.dat' that will contain transition system information: automaton corresponding with LTL-mission.
__Qtransitions_cnt = 0;
std::vector <std::vector <int>> transition_condn_list;
std::vector <int> transition_condn;
i = 1;
int automaton_state = 0;
std::ofstream outfile;
outfile.open("LTL-mission-full-automaton.dat");
// identifying...
while (i < table.size()) {
	// new automaton state while quering the data structure 'table'
	if (table[i][0] == ':') automaton_state++;
	// 'or' logical operator
	else if (table[i] == "||") {
		transition_condn_list.push_back(transition_condn);
		transition_condn = std::vector <int>(0);
	}
	// 'and' logical operator
	else if (table[i]=="&&") {}
	// automaton state identification
	else if (state.find(table[i]) != state.end()) {
		transition_condn_list.push_back(transition_condn);
		for (FOR_TYPE x = 0; x < (FOR_TYPE) transition_condn_list.size(); x++) {
			__Qtransitions_cnt++;
			outfile << automaton_state << " " << state[table[i]] << " " << transition_condn_list[x].size() << " ";
			for (FOR_TYPE y = 0; y < (FOR_TYPE) transition_condn_list[x].size(); y++) outfile << transition_condn_list[x][y] << " ";
			outfile << "\n";
		}
		transition_condn_list = std::vector <std::vector <int>>(0);
		transition_condn = std::vector <int>(0);
	}
	// 'true' logical constant
	else if (table[i] == "true") transition_condn.push_back(0);
	// 'not' logical operator
	else if (table[i][0] == '!') {
		tmp = "";
		for (FOR_TYPE x = 1; x < (FOR_TYPE) table[i].size(); x++) tmp += table[i][x];
		if (__bool_prop.find(tmp) == __bool_prop.end()) {
			__bool_prop[tmp] = __bool_prop.size() + 1;  // to avoid assigning zero
			__id_prop[__bool_prop.size()] = tmp;
		}
		transition_condn.push_back(-1*__bool_prop[tmp]);
	}
	// special automaton state identification 'skip'
	else if (table[i] == "skip") {
		__Qtransitions_cnt++;
		outfile << automaton_state << " " << automaton_state << " 1 0\n";
	}
	// atomic proposition
	else if (table[i] != "}") {
		tmp = "";
		for (FOR_TYPE x = 0; x < (FOR_TYPE) table[i].size(); x++) tmp += table[i][x];
		if (__bool_prop.find(tmp) == __bool_prop.end()) {
			__bool_prop[tmp] = __bool_prop.size() + 1;  // to avoid assigning zero
			__id_prop[__bool_prop.size()] = tmp;
		}
		transition_condn.push_back(__bool_prop[tmp]);
	}
	i++;
}
// closing file
outfile.close();



#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
	std::cout << "Number of complete Buchi automaton states for LTL mission = " << __Qstates_cnt << std::endl;
	std::cout << "Number of complete Buchi automaton accepting states for LTL mission = " << __accepting_Qstates.size() << " { ";
	for (FOR_TYPE i = 0; i < (FOR_TYPE) __accepting_Qstates.size(); i++) std::cout << __accepting_Qstates[i] << " ";
	std::cout << "}" << std::endl;
	std::cout << "Number of complete Buchi automaton transitions (logical disjunctions are extended in individual logical conjunctions) = " << __Qtransitions_cnt << std::endl;
	std::cout << "Number of atomic propositions in transition conditions = " << __bool_prop.size() << std::endl;
	std::cout << std::endl;
#endif

};

// --------------------------------------------------------------------------------------------------------------------
std::function <std::vector <std::vector <REAL_TYPE>>()> create_LUT = []() {


// UGV parameters for real-world experiments (as ARGOS J8)
	REAL_TYPE				nominalSpeed	= 3;
	std::vector <REAL_TYPE>	kv_anisotropic	= {150,50};				// (120/150,90/50) down- and up-slope anisotropic coefficients, respectively; (300,100) in DEMAIA paper for real-world DEM
	std::vector <REAL_TYPE>	COG				= {0.00, 0.3, 0.97};	// equivalent to military UGV (Argos J8)
	REAL_TYPE				L				= 2.025;
	REAL_TYPE				W				= 1.21;
	REAL_TYPE				rho_tol			= 0.20;					// 0.7; 0.8 in DEMAIA paper



// Absolute pitch (tilt) thresholds.
std::vector <REAL_TYPE> Dx = std::vector <REAL_TYPE>(2);
Dx[0] = (REAL_TYPE) (COG[0] + (0.5 * W * (1 - rho_tol)));
Dx[1] = (REAL_TYPE) (COG[0] - (0.5 * W * (1 - rho_tol)));
if (Dx[1] < Dx[0]) { REAL_TYPE aux = Dx[1]; Dx[1] = Dx[0]; Dx[0] = aux; }
std::vector <REAL_TYPE> Dy = std::vector <REAL_TYPE>(2);
Dy[0] = COG[1] + (0.5 * (L - W * rho_tol));
Dy[1] = COG[1] - (0.5 * (L - W * rho_tol));
if (Dy[1] < Dy[0]) { REAL_TYPE aux = Dy[1]; Dy[1] = Dy[0]; Dy[0] = aux; }
std::vector <REAL_TYPE> thetaThreshold = {std::atan(Dy[0] / COG[2]), std::atan(Dy[1] / COG[2])};
#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
	std::cout << "pitch thresholds = {" 
	          << thetaThreshold[0] << " rad (" << thetaThreshold[0]*180.0/M_PI << " degree)" << ", "
	          << thetaThreshold[1] << " rad (" << thetaThreshold[1]*180.0/M_PI << " degree)}" << std::endl;
#endif

// roll thresholds for every pitch angle
std::deque <REAL_TYPE> angles;
for (FOR_TYPE i = 0; i < M_PI /(2 * __step); i++ ) angles.push_front((REAL_TYPE) (-1.0 * i * __step));
angles.push_front((REAL_TYPE) ( angles[0] - __step));
angles.pop_back();
for (FOR_TYPE i = 0; i < M_PI /(2 * __step); i++ ) angles.push_back((REAL_TYPE) (i * __step));
angles.push_back((REAL_TYPE) ( angles[angles.size()-1] + __step));
std::vector <std::vector <REAL_TYPE>> result = std::vector <std::vector <REAL_TYPE>>(0);
result.resize(angles.size(), std::vector<REAL_TYPE>(angles.size()));
std::vector <REAL_TYPE> phiThreshold = {0, -INFINITO, INFINITO, 0};		// negative largest, negative shortest, positive largest, positive shortest
__symmetricSlope = 0.0;
for (FOR_TYPE i = 0; i < (FOR_TYPE) angles.size(); i++) {
	for (FOR_TYPE j = 0; j < (FOR_TYPE) angles.size(); j++) {
		REAL_TYPE theta = angles[i], phi = angles[j];
		if (theta > thetaThreshold[0] && theta < thetaThreshold[1]) {
			REAL_TYPE DDy;
			if (theta < 0 ) DDy = Dy[0]; else DDy = Dy[1];
			std::vector <REAL_TYPE> phiThreshold_tmp = std::vector <REAL_TYPE>(2);
			phiThreshold_tmp[0] = std::atan(-Dx[0] / (DDy * sin(theta) + COG[2] * cos(theta)));
			phiThreshold_tmp[1] = std::atan(-Dx[1] / (DDy * sin(theta) + COG[2] * cos(theta)));
			if (phiThreshold_tmp[1] < phiThreshold_tmp[0]) { REAL_TYPE aux = phiThreshold_tmp[1]; phiThreshold_tmp[1] = phiThreshold_tmp[0]; phiThreshold_tmp[0] = aux; }

			phiThreshold[0] = (phiThreshold_tmp[0] <= 0 && phiThreshold_tmp[0] < phiThreshold[0]) ? phiThreshold_tmp[0] : phiThreshold[0];
			phiThreshold[0] = (phiThreshold_tmp[1] <= 0 && phiThreshold_tmp[1] < phiThreshold[0]) ? phiThreshold_tmp[1] : phiThreshold[0];
			phiThreshold[1] = (phiThreshold_tmp[0] <= 0 && phiThreshold_tmp[0] > phiThreshold[1]) ? phiThreshold_tmp[0] : phiThreshold[1];
			phiThreshold[1] = (phiThreshold_tmp[1] <= 0 && phiThreshold_tmp[1] > phiThreshold[1]) ? phiThreshold_tmp[1] : phiThreshold[1];
			phiThreshold[2] = (phiThreshold_tmp[0] >= 0 && phiThreshold_tmp[0] < phiThreshold[2]) ? phiThreshold_tmp[0] : phiThreshold[2];
			phiThreshold[2] = (phiThreshold_tmp[1] >= 0 && phiThreshold_tmp[1] < phiThreshold[2]) ? phiThreshold_tmp[1] : phiThreshold[2];
			phiThreshold[3] = (phiThreshold_tmp[0] >= 0 && phiThreshold_tmp[0] > phiThreshold[3]) ? phiThreshold_tmp[0] : phiThreshold[3];
			phiThreshold[3] = (phiThreshold_tmp[1] >= 0 && phiThreshold_tmp[1] > phiThreshold[3]) ? phiThreshold_tmp[1] : phiThreshold[3];
 
			__symmetricSlope = (std::abs(theta) <= std::abs(phiThreshold_tmp[0]) && std::abs(theta) <= std::abs(phiThreshold_tmp[1])) ? std::abs(phiThreshold_tmp[1]) : __symmetricSlope;
			if (phi > phiThreshold_tmp[0] && phi < phiThreshold_tmp[1]) {
				if (theta >= 0) result[i][j] = nominalSpeed * std::pow(cos(theta), kv_anisotropic[1]);
				else result[i][j] = nominalSpeed * std::pow(cos(theta), kv_anisotropic[0]);
			}
		}
	}
}

#ifdef NON_ASYMMETRIC
	for (FOR_TYPE i = 0; i < (FOR_TYPE) angles.size(); i++) {
		for (FOR_TYPE j = 0; j < (FOR_TYPE) angles.size(); j++) {
			REAL_TYPE theta = angles[i], phi = angles[j];
			if (theta > thetaThreshold[0] && theta < thetaThreshold[1]) {
				REAL_TYPE DDy;
				if (theta < 0 ) DDy = Dy[0]; else DDy = Dy[1];
				std::vector <REAL_TYPE> phiThreshold_tmp = std::vector <REAL_TYPE>(2);
				phiThreshold_tmp[0] = std::atan(-Dx[0] / (DDy * sin(theta) + COG[2] * cos(theta)));
				phiThreshold_tmp[1] = std::atan(-Dx[1] / (DDy * sin(theta) + COG[2] * cos(theta)));
				if (phiThreshold_tmp[1] < phiThreshold_tmp[0]) { REAL_TYPE aux = phiThreshold_tmp[1]; phiThreshold_tmp[1] = phiThreshold_tmp[0]; phiThreshold_tmp[0] = aux; }
				if (!(theta > -__symmetricSlope && theta < __symmetricSlope && phi > -__symmetricSlope && phi < __symmetricSlope)) result[i][j] = 0.0;
			}
		}
	}
#endif


return result;

};

// Main code block of function 'initialize'.
{
arguments* inputs = new arguments;

#ifndef VERBOSE_NONE
	printf("%s\n", std::string(MAX_STRING,'-').c_str());
	std::cout << "DEM (or DBM) specification file                                         : " << dem_filename << std::endl;
	std::cout << "Atomic proposition mapping file (in case of DBM, not apply)             : " << proposition_map_filename << std::endl;
	std::cout << "UGV mission specification file                                          : " << mission_filename << std::endl;
	std::cout << "Orthophoto specification file (in case of DEM with RGB information)     : " << orthophoto_filename << std::endl;
	printf("%s\n\n", std::string(MAX_STRING,'-').c_str());
#endif

// Reading the file with LTL formula that specifies the required mission.
// The content of the file must be '<LTL_formula>'.
// The filename of the LTL mission specification must be passed as the third (last) input argument to the programm.
clock_t t = clock();
const int BUFFER_SIZE = 4096;
// char buff[BUFFER_SIZE+20] = "ltl2tgba --spin --complete --state-based-acceptance '";  // this command generates a completed Buchi automaton with an additional rejected state (non-necessary)
char buff[BUFFER_SIZE+20] = "ltl2tgba --spin '";
char tmp[BUFFER_SIZE];
FILE* ft;
if ((ft = fopen(mission_filename, "r")) == NULL) {
	std::cout << "\033[1;31mError: The UGV mission specification file not found.\033[0m" << std::endl << std::endl;
	return inputs;
}
if (fgets(tmp, BUFFER_SIZE, (FILE*)ft) == NULL) {}
if (fgets(tmp, BUFFER_SIZE, (FILE*)ft) == NULL) {
	std::cout << "\033[1;31mError: Failed to execute the command specified. Check the LTL formula in the UGV mission file.\033[0m" << std::endl << std::endl;
	return inputs;
}
bool isLTLformula = !isdigit(tmp[0]);
#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
	if (isLTLformula) std::cout << "Original UGV mission specification (LTL formula), SPIN or Spot syntax: " << std::endl << " " << tmp << std::endl;
	else {
		std::cout << "The original UGV mission specification is expressed as a Buchi automaton with accepting states." << std::endl;
		#if defined(VERBOSE_COMPLETE) || defined(VERBOSE_DEBUG)
			std::cout << " (->) is the initial state and (*) are accepting states." << std::endl;
		#endif
		std::cout << std::endl;
	}
#endif

// Creating the complete Buchi automaton correspond with the LTL mission.
if (isLTLformula) {
	fclose(ft);
	strcat(buff,tmp);
	strcat(buff,"'");
	create_automaton_from_ltl(buff);
	#if defined(VERBOSE_COMPLETE) || defined(VERBOSE_DEBUG)
		printf("Converted from LTL formula to Buchi automaton                           : %9.4f milliseconds\n", (REAL_TYPE)1000 * (clock() - t) / CLOCKS_PER_SEC);
	#endif
}
else {
	// Save the automaton (contained into the mission specification file) to the file 'LTL-mission-full-automaton.dat'
	// Creating a file 'LTL-mission-full-automaton.dat' that will contain transition system information: automaton corresponding with LTL-mission.
	std::vector <std::vector <int>> transition_condn_list;
	std::vector <int> transition_condn;
	int automaton_state = 0;
	std::ofstream outfile;
	outfile.open("LTL-mission-full-automaton.dat");

	// Identifying automaton accepting states.
	std::stringstream stream(tmp); int item;
	while (stream >> item) __accepting_Qstates.push_back(item);



	// identifying...
	//unordered_map <string, int> state;  // local variable as key-value pair map of automaton states
	std::vector <int> state_list;  // local variable as list of automaton states with a number (key) associated with each one
	while (fgets(tmp, BUFFER_SIZE, (FILE*)ft) != NULL) {
		__Qtransitions_cnt++;
		std::stringstream stream(tmp); int Qstate_out, Qstate_in;
		stream >> Qstate_out;
		stream >> Qstate_in;
		__Qstates_cnt = (__Qstates_cnt > Qstate_out) ? __Qstates_cnt : Qstate_out;
		__Qstates_cnt = (__Qstates_cnt > Qstate_in)  ? __Qstates_cnt : Qstate_in;
		stream >> item;
		for (FOR_TYPE x = 0; x < item; x++) {
			std::string prop; stream >> prop; std::string temp = "";
			int offset = (prop.substr(0,1) == "!") ? 1 : 0;
			for (FOR_TYPE x = offset; x < (FOR_TYPE) prop.size(); x++) temp += prop[x];
			if (__bool_prop.find(temp) == __bool_prop.end()) {
				__bool_prop[temp] = __bool_prop.size() + 1;  // to avoid assigning zero
				__id_prop[__bool_prop.size()] = temp;
			}
			if (prop.substr(0,1) == "!") transition_condn.push_back(-1*__bool_prop[temp]);
			else transition_condn.push_back(__bool_prop[temp]);
		}
		outfile << Qstate_out << " " << Qstate_in << " " << item;
		for (FOR_TYPE x = 0; x < (FOR_TYPE) transition_condn.size(); x++) outfile << " " << transition_condn[x];
		outfile << std::endl;
		transition_condn = std::vector <int>(0);


	}
	// closing files
	outfile.close();
	fclose(ft);



	__Qstates_cnt++;

	#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
		std::cout << "Number of complete Buchi automaton states for LTL mission = " << __Qstates_cnt << std::endl;
		std::cout << "Number of complete Buchi automaton accepting states for LTL mission = " << __accepting_Qstates.size() << " { ";
		for (FOR_TYPE i = 0; i < (FOR_TYPE) __accepting_Qstates.size(); i++) std::cout << __accepting_Qstates[i] << " ";
		std::cout << "}" << std::endl;
		std::cout << "Number of complete Buchi automaton transitions (logical disjunctions are extended in individual logical conjunctions) = " << __Qtransitions_cnt << std::endl;
		std::cout << "Number of atomic propositions in transition conditions = " << __bool_prop.size() << std::endl;
		std::cout << std::endl;
	#endif
	#ifndef VERBOSE_NONE
		printf("Read from Buchi automaton specification file                            : %9.4f milliseconds\n", (REAL_TYPE)1000 * (clock() - t) / CLOCKS_PER_SEC);
	#endif
}


// Opening DEM specification file.
// The filename of the DEM specification is passed as the first input argument to the programm.
t = clock();
std::ifstream dem_file;
dem_file.open(dem_filename);

// Determining number of numeric values into the DEM specification file: two (2D), three (2.5D) or six (2.5D and RGB color).
std::string line;
std::stringstream linestr;
REAL_TYPE value;
__DEMtype = 0;
getline(dem_file,line);
linestr << line;
while (!linestr.eof()) {
	linestr >> line;
	if (std::stringstream(line) >> value) __DEMtype++;
}
if (__DEMtype == 1) __DEMtype++;  // 2D file specification format corresponds with the original 'Tstar' planner implementation
#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
	std::string prompt = "Environment ";
	printf("\n%s %s %s\n",std::string((MAX_STRING-prompt.size()-1)/2,'=').c_str(), prompt.c_str(), std::string((MAX_STRING-prompt.size()-1)/2,'=').c_str());
	printf("%s\n",std::string(MAX_STRING,'=').c_str());
	std::cout << "Type of environment: ";
	if (__DEMtype == 2) std::cout << "2D with Binary Occupancy Model (i.e. explicit obstacle and free grid cells)";
	else if (__DEMtype == 3) std::cout << "2.5D with Digital Elevation Model (DEM) and UGV constraints";
	else std::cout << "2.5D with Digital Elevation Model (DEM), RGB information and UGV constraints";
	std::cout << std::endl;
#endif
// going to start position of the file
dem_file.clear();
dem_file.seekg(std::ios::beg);

// Creating vectors (internal data structure) as environment DEM from the specification file.
// computing parameters (__minX,__minY) and DEM resolution, for integer indices, and vector size
if (__DEMtype != 2) {
	std::vector <REAL_TYPE> x_double_coordinates;
	std::vector <REAL_TYPE> y_double_coordinates;
	while (!dem_file.eof()) {
		// storing point coordinates into '__XYZgrid' (internal data structure)
		dem_file >> value;  // X point coordinate
		x_double_coordinates.push_back(value);
		dem_file >> value;  // Y point coordinate
		y_double_coordinates.push_back(value);
		// ignoring elevation and RGB color values
		dem_file >> value;  // elevation or Z point coordinate
		if (__DEMtype == 6) for (FOR_TYPE i = 0; i < 3; i++) dem_file >> value;
	}
	// computing the minimum XY coordinates of DEM grid
	__minX = *min_element(x_double_coordinates.begin(), x_double_coordinates.end());
	__minY = *min_element(y_double_coordinates.begin(), y_double_coordinates.end());
	REAL_TYPE maxx = *max_element(x_double_coordinates.begin(), x_double_coordinates.end());
	REAL_TYPE maxy = *max_element(y_double_coordinates.begin(), y_double_coordinates.end());
	// computing the DEM grid resolution
	__DEM_resolution = DBL_MAX;
	REAL_TYPE diff; 
	for (FOR_TYPE i = 1; i < (FOR_TYPE) x_double_coordinates.size(); i++) {
		diff = std::abs(x_double_coordinates[i-1] - x_double_coordinates[i]);
		diff = (diff == 0) ? __DEM_resolution : diff;
		__DEM_resolution = (diff < __DEM_resolution ) ? diff : __DEM_resolution;
	}
	// computing the DEM grid size
	__dimX = (int) ((maxx - __minX + 1) / __DEM_resolution);
	__dimY = (int) ((maxy - __minY + 1) / __DEM_resolution);
	// going to start position of the file
	dem_file.clear();
	dem_file.seekg(std::ios::beg);
	#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
		printf("DEM specification XY or UTM min = (%.10g,%.10g)\n", __minX, __minY);
		printf("DEM specification XY or UTM max = (%.10g,%.10g)\n", maxx, maxy);
		printf("DEM resolution = %.10g m\n",__DEM_resolution);
	#endif
}
else __minX = __minY = __DEM_resolution = 1;

// saving obstacles (2D) or elevation Z values (2.5D) into vector 'XYZgrid'
int idx = 0;
int num_obstacles = INT_MAX;
__SYS_STATE Sstate;
while (!dem_file.eof() && (__DEMtype != 2 || __DEMtype == 2 && idx < num_obstacles)) {
	switch (__DEMtype) {
		case 2:
			if (!idx) {
				dem_file >> __dimX;
				dem_file >> __dimY;
				dem_file >> num_obstacles;
			}
			// storing obstacle coordinates into '__XYZgrid' (internal data structure)
			dem_file >> value;  // X obstacle coordinate
			Sstate.x = value - 1;
			dem_file >> value;  // Y obstacle coordinate
			Sstate.y = value - 1;
			inputs->XYZgrid[__keyGrid(Sstate)] = OBSTACLE;
			break;
		case 3:
		case 6:
			// storing 2.D point coordinates into '__XYZgrid' (internal data structure)
			std::vector <REAL_TYPE> utm_coordinate = std::vector <REAL_TYPE>(3);
			dem_file >> utm_coordinate[0];  // X (longitude) UTM coordinate
			dem_file >> utm_coordinate[1];  // Y (latitude) UTM coordinate
			Sstate = _utm_to_xy(utm_coordinate);
			dem_file >> value;  // elevation or Z point coordinate
			inputs->XYZgrid[__keyGrid(Sstate)] = (REAL_TYPE) value / __DEM_resolution;	// normalized altitude, i.e., Z divide by __DEM_resolution
			break;
	}
	idx++;
}





// Reading the grid point as a UGV initial position for path planning
// opening LTL mission specification file again
t = clock();
std::ifstream mission_file;
mission_file.open(mission_filename);
// determining XY coordinates for initial system state
if (__DEMtype == 2) {
	mission_file >> value;
	inputs->s0.x = value - 1;  // X point coordinate
	mission_file >> value;
	inputs->s0.y = value - 1;  // Y point coordinate
}
else {
	std::vector <REAL_TYPE> utm_coordinate = std::vector <REAL_TYPE>(3);
	mission_file >> utm_coordinate[0];  // X (longitude) UTM coordinate
	mission_file >> utm_coordinate[1];  // Y (latitude) UTM coordinate
	inputs->s0 = _utm_to_xy(utm_coordinate);
}
// closing file
mission_file.close();
#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
	if (__DEMtype == 2) std::cout << std::endl << "Grid point as UGV initial system state for path planning = (" << inputs->s0.x + 1 << "," << inputs->s0.y + 1 << ")" << std::endl << std::endl;
	else {
		std::vector <REAL_TYPE> utm_coordinate = std::vector <REAL_TYPE>(3);
		utm_coordinate[0] = (REAL_TYPE) (inputs->s0.x) * __DEM_resolution + __minX;
		utm_coordinate[1] = (REAL_TYPE) (inputs->s0.y) * __DEM_resolution + __minY;
		printf("\nGrid point as UGV initial position for path planning = (%.10g,%.10g) equivalent to normalized coordinates (%.10g,%.10g)\n\n", utm_coordinate[0], utm_coordinate[1], inputs->s0.x+1, inputs->s0.y+1);
	}
#endif
#ifndef VERBOSE_NONE
	printf("Read UGV initial state from mission specification file                  : %9.4f milliseconds\n", (REAL_TYPE)1000 * (clock() - t) / CLOCKS_PER_SEC);
#endif

// Reading atomic propositions mapping (included into DEM specification file or an additional specification file).
t = clock();
int propositions_cnt;
KEY_TYPE idx_key;
int proposition;
std::string sprop;
__SstatesByProp.resize(__bool_prop.size() + 1);
switch (__DEMtype) {
	case 2:
		// if the environment is two-dimensional, atomic propositions mapping is included into DEM specification file
		dem_file >> propositions_cnt;
		// reading coordinates with associated true atomic proposition
		Sstate = __SYS_STATE();
		for (FOR_TYPE i = 0; i < propositions_cnt; i++) {
			dem_file >> value;
			Sstate.x = value - 1;
			dem_file >> value;
			Sstate.y = value - 1;
			dem_file >> sprop;
			proposition = 0;
			if (__bool_prop.find(sprop) != __bool_prop.end()) proposition = __bool_prop[sprop];
			// storing atomic proposition identificator into '__propByKey' (internal data structure) related with a grid point
			idx_key = __key(Sstate);
			__propByKey[idx_key].push_back(proposition);
			// storing grid point into '__SstatesByProp' (internal data structure) related with an atomic proposition identificator
			__SstatesByProp[proposition].push_back(Sstate);
			// storing the list of states with a proposition true at it
			if (__propByKey[idx_key].size() == 1) __SstatesWithProp.push_back(Sstate);
		}
		break;
	case 3:
	case 6:
		// opening atomic propositions mapping specification file
		// the filename of the specification is passed as the third input argument to the programm
		std::ifstream prop_file;
		prop_file.open(proposition_map_filename);
		idx = 0;
		while (!prop_file.eof()) {
			std::vector <REAL_TYPE> utm_coordinate = std::vector <REAL_TYPE>(3);
			prop_file >> utm_coordinate[0];  // X (longitude) UTM coordinate
			prop_file >> utm_coordinate[1];  // Y (latitude) UTM coordinate
			Sstate = _utm_to_xy(utm_coordinate);
			prop_file >> value;
//			Sstate.theta = value;  // theta, orientation angle
			prop_file >> sprop;
			proposition = 0;
			if (__bool_prop.find(sprop) != __bool_prop.end()) proposition = __bool_prop[sprop];
			// storing atomic proposition identificator into '__propByKey' (internal data structure) related with a grid point
			idx_key = __key(Sstate);
			__propByKey[idx_key].push_back(proposition);
			// storing grid point into '__SstatesByProp' (internal data structure) related with an atomic proposition identificator
			__SstatesByProp[proposition].push_back(Sstate);
			// storing the list of states with a proposition true at it
			if (__propByKey[idx_key].size() == 1) __SstatesWithProp.push_back(Sstate);
			idx++;	
		}
		propositions_cnt = idx;
		// closing file
		prop_file.close();
		break;
}


// closing file
dem_file.close();
#ifndef VERBOSE_NONE
	printf("Read atomic proposition mapping relation from specification file        : %9.4f milliseconds\n", (REAL_TYPE)1000 * (clock() - t) / CLOCKS_PER_SEC);
#endif

// Pruning the completed Buchi automaton (the unreachable states removing is not included, because in practice it does not affect the computational cost).
t = clock();
std::ifstream ltl_mission_file;
ltl_mission_file.open("LTL-mission-full-automaton.dat");
std::ofstream outfile;
outfile.open("LTL-mission-automaton.dat");
#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
	printf("\n%s\n", std::string(MAX_STRING,'-').c_str());
	std::cout << "Pruning the Buchi automaton according to atomic propositions mapping over environment (unreachable states removing is not included)" << std::endl;
#endif

// Creating vectors (internal data structure) as simplified automaton transitions table from the LTL mission automaton specification file.
// reading automata transitions
std::vector <int> automata_states;
std::vector <int> transition_condn;
bool transition_satisfied = false;
FOR_TYPE Qtransitions_cnt = 0;
for (FOR_TYPE i = 0; i < __Qtransitions_cnt; i++) {
	automata_states = std::vector <int>(2);
	ltl_mission_file >> automata_states[0];
	ltl_mission_file >> automata_states[1];
	int trans_condn_len = 0, neg_literals = 0;
	ltl_mission_file >> trans_condn_len;
	// composing the transition conditions as a conjunction ('and' logical operator) of atomics propositions
	transition_condn = std::vector <int>(0);
	for (FOR_TYPE j = 0; j < trans_condn_len; j++) {
		ltl_mission_file >> proposition;
		transition_condn.push_back(proposition);
		// identifying transition condition negative (with all negative atomic propositions)
		if (proposition <= 0) neg_literals++;
	}
	// evaluating whether the transition condition is a conjunction of negative literals
	if (neg_literals == trans_condn_len) {
		#ifdef VERBOSE_DEBUG
			if (i < 9) std::cout << "transition #   "; else if (i < 99) std::cout << "transition #  "; else if (i < 999) std::cout << "transition # "; else std::cout << "transition #  ";
			std::cout << i+1;
			std::cout << "     Negative transition condition, ";
		#endif
		transition_satisfied = true;
	}
	else {
		for (FOR_TYPE n = 0; n < (FOR_TYPE) __SstatesWithProp.size(); n++) {
			int OK_cnt = 0;
			int tmp;
			#ifdef VERBOSE_DEBUG
				if (n == 0) {
					if (i < 9) std::cout << "transition #   "; else if (i < 99) std::cout << "transition #  "; else if (i < 999) std::cout << "transition # "; else std::cout << "transition #  ";
					std::cout << i+1;
				}
				std::cout << "     [ ";
			#endif
			for (FOR_TYPE j = 0; j < trans_condn_len; j++) {
				#ifdef VERBOSE_DEBUG
					std::cout << std::showpos << transition_condn[j] << std::noshowpos << " ";
				#endif
				if (transition_condn[j] >= 0) {
					tmp = 0;
					for (FOR_TYPE k = 0; k < (FOR_TYPE) __propByKey[__key(__SstatesWithProp[n])].size(); k++)
						if (transition_condn[j] == __propByKey[__key(__SstatesWithProp[n])][k]) { tmp = 1; break; }
					OK_cnt += tmp;
				}
				else {
					tmp = 1;
					for (FOR_TYPE k = 0; k < (FOR_TYPE) __propByKey[__key(__SstatesWithProp[n])].size(); k++)
						if (-1 * transition_condn[j] == __propByKey[__key(__SstatesWithProp[n])][k]) { tmp = 0; break; }
					OK_cnt += tmp;
				}
				if (tmp == 0) break;
			}
			transition_satisfied = (OK_cnt == trans_condn_len);
			if (transition_satisfied) {
				#ifdef VERBOSE_DEBUG
					std::cout << "] ";
				#endif
				break;
			}
		}
	}
	if (transition_satisfied) {
		#ifdef VERBOSE_DEBUG
			std::cout << "satisfied, so included it" << std::endl;
		#endif
		Qtransitions_cnt++;
		outfile << automata_states[0] << " " << automata_states[1] << " " << trans_condn_len << " ";
		for (FOR_TYPE j = 0; j < trans_condn_len; j++) outfile << transition_condn[j] << " ";
		outfile << "\n";
	}
	#ifdef VERBOSE_DEBUG
		else std::cout << "... not satisfied, so removing transition condition" << std::endl;
	#endif
}

// closing files
ltl_mission_file.close();
outfile.close();
#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
	// Reading the file 'LTL-mission-automaton.dat' to show it to the user (optional)
	FILE* fp;
	std::string table_text = "";
	if ((fp = fopen("LTL-mission-automaton.dat", "r")) == NULL)
		std::cout << "\033[1;31mError: Failed to open the file auto-generated.\033[0m" << std::endl;
	else {
		table_text = "";
		while (fgets(buff, BUFFER_SIZE, fp) != NULL) table_text = table_text + buff;
		fclose(fp);
		std::cout << std::endl;
		prompt = "File 'LTL-mission-automaton' with simplified Buchi automaton specification for LTL mission";
		printf("%s %s %s\n",std::string((MAX_STRING-prompt.size()-1)/2,'=').c_str(), prompt.c_str(), std::string((MAX_STRING-prompt.size()-1)/2,'=').c_str());
		printf("%s\n",std::string(MAX_STRING,'=').c_str());
	}
#endif
#if defined(VERBOSE_COMPLETE) || defined(VERBOSE_DEBUG)
	std::cout << table_text << std::endl;
#endif

__Qtransitions_cnt = Qtransitions_cnt;

#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
	std::cout << "Number of simplified Buchi automaton states for LTL mission = " << __Qstates_cnt << std::endl;
	std::cout << "Number of simplified Buchi automaton accepting states for LTL mission = " << __accepting_Qstates.size() << " { ";
	for (FOR_TYPE i = 0; i < (FOR_TYPE) __accepting_Qstates.size(); i++) std::cout << __accepting_Qstates[i] << " ";
	std::cout << "}" << std::endl;
	std::cout << "Number of \ied Buchi automaton transitions (logical disjunctions are extended in individual logical conjunctions) = " << __Qtransitions_cnt << std::endl;
	std::cout << "Number of atomic propositions in transition conditions = " << __bool_prop.size() << std::endl;
	std::cout << std::endl;
#endif
#ifndef VERBOSE_NONE
	printf("Pruned Buchi automaton according with mapping relation                  : %9.4f milliseconds\n", (REAL_TYPE)1000 * (clock() - t) / CLOCKS_PER_SEC);
#endif

// Opening automaton specification file 'LTL-mission-automaton.dat'.
t = clock();
ltl_mission_file.open("LTL-mission-automaton.dat");

// Creating vectors (internal data structure) as automaton transitions table from the LTL mission automaton specification file.
// Automaton transitions are saved as a conjunction of atomics propositions (positive or negative literals)
// The transition conditions are categorized as normal, negative or strictly negative.
__Qtransitions = std::vector< std::vector< std::vector< std::vector<int>>>>(__Qstates_cnt, std::vector< std::vector< std::vector<int>>>(__Qstates_cnt));
__negativeQtransitions = std::vector< std::vector< std::vector< std::vector<int>>>>(__Qstates_cnt, std::vector< std::vector< std::vector<int>>>(__Qstates_cnt));
__positiveQtransitions = std::vector< std::vector< std::vector< std::vector<int>>>>(__Qstates_cnt, std::vector< std::vector< std::vector<int>>>(__Qstates_cnt));
__negativeQtransitions_nonLoop = std::vector <int>(__Qstates_cnt);
// reading automata transitions
for (FOR_TYPE i = 0; i < __Qtransitions_cnt; i++) {
	automata_states = std::vector <int>(2);
	ltl_mission_file >> automata_states[0];
	ltl_mission_file >> automata_states[1];
	int trans_condn_len = 0, neg_literals = 0, strict_neg_literals = 0;
	ltl_mission_file >> trans_condn_len;
	// composing the transition conditions as a conjunction ('and' logical operator) of atomics propositions
	transition_condn = std::vector <int>(0);
	for (FOR_TYPE j = 0; j < trans_condn_len; j++) {
		ltl_mission_file >> proposition;
		transition_condn.push_back(proposition);
		// identifying transition condition negative or strictly negative (with all negative atomic propositions)
		if (proposition <= 0) neg_literals++;
		if (proposition < 0) strict_neg_literals++;
	}
	// saving transition conditions into vector '__Qtransitions'
	__Qtransitions[automata_states[0]][automata_states[1]].push_back(transition_condn);
	// evaluating whether the transition condition is a conjunction of negative literals
	if (neg_literals == trans_condn_len) {
		// saving negative transition conditions into vector '__negativeQtransitions'
		__negativeQtransitions[automata_states[0]][automata_states[1]].push_back(transition_condn);
		// evaluating whether the transition condition is a conjunction of strictly negative literals without cycle
		// marking as special negative transition conditions into vector '__negativeQtransitions_nonLoop'
		if (automata_states[0] != automata_states[1] && strict_neg_literals == trans_condn_len) __negativeQtransitions_nonLoop[automata_states[0]]++;
	}
	else {
		// saving positive transition conditions into vector '__positiveQtransitions'
		__positiveQtransitions[automata_states[0]][automata_states[1]].push_back(transition_condn);
	}
}
// closing file
ltl_mission_file.close();
#ifndef VERBOSE_NONE
	printf("Computed reduced Buchi automaton, obtaining internal data               : %9.4f milliseconds\n", (REAL_TYPE)1000 * (clock() - t) / CLOCKS_PER_SEC);
#endif

// optionally cancel the automaton prunning
#ifdef NON_AUTOMATON_PRUNING
	std::ifstream  src("./LTL-mission-full-automaton.dat", std::ios::binary);
	std::ofstream  dst("./LTL-mission-automaton.dat",   std::ios::binary);
	dst << src.rdbuf();
	#ifndef VERBOSE_NONE
		std::cout << "\033[1;36mWarning: Canceling Buchi automaton prunning. Remove compiler directive NON_AUTOMATON_PRUNING to apply a reduced pruned automaton.\033[0m" << std::endl;
	#endif
#endif

// Creating look-up table for speed associated with a pair of slope-tilt segment inclination
t = clock();
inputs->vRef = create_LUT();
#ifndef VERBOSE_NONE
	printf("Computed LUT with slope-aware UGV reference velocity (step=%5.3f rad)   : %9.4f milliseconds\n", __step, (REAL_TYPE)1000 * (clock() - t) / CLOCKS_PER_SEC);
#endif

return inputs;
}

}


// ====================================================================================================================
void compute_accepting_product_automaton_states() {

// Memory allocation for internal data structures (vectors).
// automaton states that satisfy an incoming transition to an automaton state
_qsystate = std::vector< std::vector<int>>(__Qstates_cnt);
// automaton states that satisfy an automaton transition
_qsytrans = std::vector< std::vector< std::vector<int>>>(__Qstates_cnt, std::vector< std::vector<int>>(__Qstates_cnt));

// Evaluating automaton transitions in each grid point where any atomic proposition is satisfied.
#ifdef VERBOSE_DEBUG
	std::cout << std::endl;
#endif
for (FOR_TYPE nstate = 0; nstate < __Qstates_cnt; nstate++)
	for (FOR_TYPE j = 0; j < __Qstates_cnt; j++)
		for (FOR_TYPE z = 0; z < (FOR_TYPE) __Qtransitions[nstate][j].size(); z++) {
			// checking transition condition (non-negative) associated with each pair of automaton states
			std::vector <int> transition_req = __Qtransitions[nstate][j][z];
			for (FOR_TYPE i = 0; i < (FOR_TYPE) __SstatesWithProp.size(); i++) {
				int OK_cnt = 0;
				int tmp;
				__SYS_STATE Sstate = __SstatesWithProp[i];
				std::vector <int> current_prop = __propByKey[__key(Sstate)];
				for (FOR_TYPE k = 0; k < (FOR_TYPE) transition_req.size(); k++) {
					if (transition_req[k] >= 0) {
						tmp = 0;
						for (FOR_TYPE l = 0; l < (FOR_TYPE) current_prop.size(); l++)
							if (transition_req[k] == current_prop[l]) { tmp = 1; break; }
						OK_cnt += tmp;
					}
					else {
						tmp = 1;
						for (FOR_TYPE l = 0; l < (FOR_TYPE) current_prop.size(); l++)
							if (-1 * transition_req[k] == current_prop[l]) { tmp = 0; break; }
						OK_cnt += tmp;
					}
					if (tmp == 0) break;
				}

				// storing grid point index (to use only with __SstatesWithProp) where the checking transition condition is satisfied
				if (OK_cnt == transition_req.size()) {
					_qsystate[j].push_back(i);
					_qsytrans[nstate][j].push_back(i);

				}
			}
		}

}


// ====================================================================================================================
std::deque <__PATH*> compute_suffix_prefix_paths(bool cycle) {

// If 'cycle' is true, this function compute the shortest cycle paths (suffices) from accepting product automaton states.
// If 'cycle' is false, this function compute the shortest path (prefix) towards any accepting product automaton state.
#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
	std::cout << std::endl;
	std::cout << "\033[1;202m\033[4mComputing ";
	if (cycle) std::cout << "suffix (cycle)"; else std::cout << "prefix (non-cycle)";
	std::cout << " paths\033[0m" << std::endl;
	std::cout << std::endl << "Number of system states where some automaton transition is satisfied = " << __SstatesWithProp.size();
	if (__SstatesWithProp.size()) std::cout << ":" << std::endl << " ";
	for (FOR_TYPE i = 0; i < (FOR_TYPE) __SstatesWithProp.size(); i++)
		if (__DEMtype == 2) std::cout << "(" << __SstatesWithProp[i].x+1 << "," << __SstatesWithProp[i].y+1 << ") ";
///		else printf("(%.10g,%.10g,%.10g) ", __SstatesWithProp[i].x * __DEM_resolution + __minX, __SstatesWithProp[i].y * __DEM_resolution + __minY, __SstatesWithProp[i].theta);
		else printf("(%.10g,%.10g) ", __SstatesWithProp[i].x * __DEM_resolution + __minX, __SstatesWithProp[i].y * __DEM_resolution + __minY);
	std::cout << std::endl;
#endif

// for every accepting state of LTL automaton
std::deque <__PATH*> paths;
paths.push_back(new_path_node(INFINITO));
int idPath = 1;
if (cycle) {
	// NOTE: the following for-loop could be parallelized and select the suffices with the lowest total cost.
	for (FOR_TYPE fs = 0; fs < (FOR_TYPE) __accepting_Qstates.size(); fs++) {
		// evaluating all points where some incoming transition is satisfied for a given accepting state
		#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
			std::cout << std::endl << "Number of incoming transition conditions that are satisfied for the ";
			if (__accepting_Qstates.size() > 1) std::cout << fs + 1 << "-th ";
			std::cout << "accepting state = " << _qsystate[__accepting_Qstates[fs]].size();
			if (_qsystate[__accepting_Qstates[fs]].size()) {
				std::cout << ", from the system state/-s: " << std::endl << " ";
				std::vector <bool> OK(__SstatesWithProp.size());
				for (FOR_TYPE i = 0; i < (FOR_TYPE) _qsystate[__accepting_Qstates[fs]].size(); i++) {
					KEY_TYPE systate = _qsystate[__accepting_Qstates[fs]][i];
					if (OK[systate]) continue;
					else OK[systate] = true;
					std::cout << "(";
					if (__DEMtype == 2) std::cout << __SstatesWithProp[systate].x+1 << "," << __SstatesWithProp[systate].y+1;
///					else printf("%.10g,%.10g,%.10g", __SstatesWithProp[systate].x * __DEM_resolution + __minX, __SstatesWithProp[systate].y * __DEM_resolution + __minY, __SstatesWithProp[systate].theta);
					else printf("%.10g,%.10g", __SstatesWithProp[systate].x * __DEM_resolution + __minX, __SstatesWithProp[systate].y * __DEM_resolution + __minY);
					std::cout << ") ";
				}
			}
			std::cout << std::endl;
		#endif
		std::vector <bool> OK(__SstatesWithProp.size());
		for (FOR_TYPE i = 0; i < (FOR_TYPE) _qsystate[__accepting_Qstates[fs]].size(); i++) {
			// determining a point index of the point vector related to the accepting state evaluated
			KEY_TYPE systate = _qsystate[__accepting_Qstates[fs]][i];
			if (OK[systate]) continue;
			else OK[systate] = true;
			#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
				std::cout << std::endl << "Candidate suffix path #" << idPath << " starting cycle in ";
				std::cout << "\033[4m(";
				if (__DEMtype == 2) std::cout << __SstatesWithProp[systate].x+1 << "," << __SstatesWithProp[systate].y+1;
///				else printf("%.10g,%.10g,%.10g", __SstatesWithProp[systate].x * __DEM_resolution + __minX, __SstatesWithProp[systate].y * __DEM_resolution + __minY, __SstatesWithProp[systate].theta);
				else printf("%.10g,%.10g", __SstatesWithProp[systate].x * __DEM_resolution + __minX, __SstatesWithProp[systate].y * __DEM_resolution + __minY);
				std::cout << ")\033[0m " << std::endl;
			#endif
			// searching for path towards each involved grid point into the product automaton graph (as cycle from initial state)
			__PATH* path_candidate;
			prodNode* ini_st = new_prod_graph_node(__SstatesWithProp[systate],__accepting_Qstates[fs]);
			prodNode* end_st = new_prod_graph_node(__SstatesWithProp[systate],__accepting_Qstates[fs]);
			#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
				std::cout << "Computing cycle path... " << std::endl;
			#endif
			path_candidate = path_search(ini_st, {end_st}, paths[0]->total_cost);
			path_candidate->id = idPath++;
			if (std::isfinite(path_candidate->total_cost))
				if (std::abs(path_candidate->total_cost - paths[0]->total_cost) < EPSILON) paths.push_back(path_candidate);
				else
					if (path_candidate->total_cost < paths[0]->total_cost) {
						paths.clear();
						paths.push_back(path_candidate);
					}
		}
	}
}
else {
	std::vector <prodNode*> end_sts = std::vector <prodNode*>(0);
	for (FOR_TYPE fs = 0; fs < (FOR_TYPE) __accepting_Qstates.size(); fs++) {
		// evaluating all points where some incoming transition is satisfied for a given accepting state
		#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
			std::cout << std::endl << "Number of incoming transition conditions that are satisfied for the ";
			if (__accepting_Qstates.size() > 1) std::cout << fs + 1 << "-th ";
			std::cout << "accepting state = " << _qsystate[__accepting_Qstates[fs]].size();
			if (_qsystate[__accepting_Qstates[fs]].size()) {
				std::cout << ", from the system state/-s: " << std::endl << " ";
				std::vector <bool> OK(__SstatesWithProp.size());
				for (FOR_TYPE i = 0; i < (FOR_TYPE) _qsystate[__accepting_Qstates[fs]].size(); i++) {
					KEY_TYPE systate = _qsystate[__accepting_Qstates[fs]][i];
					if (OK[systate]) continue;
					else OK[systate] = true;
					std::cout << "(";
					if (__DEMtype == 2) std::cout << __SstatesWithProp[systate].x+1 << "," << __SstatesWithProp[systate].y+1;
///					else printf("%.10g,%.10g,%.10g", __SstatesWithProp[systate].x * __DEM_resolution + __minX, __SstatesWithProp[systate].y * __DEM_resolution + __minY, __SstatesWithProp[systate].theta);
					else printf("%.10g,%.10g", __SstatesWithProp[systate].x * __DEM_resolution + __minX, __SstatesWithProp[systate].y * __DEM_resolution + __minY);
					std::cout << ") ";
				}
			}
			std::cout << std::endl << std::endl;
		#endif
		std::vector <bool> OK(__SstatesWithProp.size());
		for (FOR_TYPE i = 0; i < (FOR_TYPE) _qsystate[__accepting_Qstates[fs]].size(); i++) {
			// determining a point index of the point vector related to the accepting state evaluated
			KEY_TYPE systate = _qsystate[__accepting_Qstates[fs]][i];
			if (OK[systate]) continue;
			else {
				OK[systate] = true;
				end_sts.push_back(new_prod_graph_node(__SstatesWithProp[systate],__accepting_Qstates[fs]));
			}
		}
	}
	// searching for path towards any involved grid point into the product automaton graph
	__PATH* path_candidate;
	prodNode* ini_st = new_prod_graph_node(__inputs->s0,0);
	#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
		std::cout << "Computing non-cycle path..." << std::endl;
	#endif
	path_candidate = path_search(ini_st, end_sts, paths[0]->total_cost);
	path_candidate->id = idPath++;
	if (std::isfinite(path_candidate->total_cost))
		if (std::abs(path_candidate->total_cost - paths[0]->total_cost) < EPSILON) paths.push_back(path_candidate);
		else
			if (path_candidate->total_cost < paths[0]->total_cost) {
				paths.clear();
				paths.push_back(path_candidate);
			}
}

#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
	if (!std::isfinite(paths[0]->total_cost)) std::cout << std::endl << "\033[1;202mOptimal path not found\033[0m";
	else {
		std::cout << std::endl << "\033[1;202m" << paths.size() << " optimal ";
		if (cycle) std::cout << "suffix "; else std::cout << "prefix ";
		std::cout << "path/-s found\033[0m";
		if (std::isfinite(paths[0]->total_cost)) std::cout << " [ total cost = " << paths[0]->total_cost << " s ]" << std::endl;
	}
#endif
#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME) && !defined(VERBOSE_COMPLETE) && !defined(VERBOSE_DEBUG)
	std::cout << std::endl;
#endif

return paths;

}


// ====================================================================================================================
__PATH* path_search(prodNode* srcNode, std::vector <prodNode*> destNodes, REAL_TYPE minCost_found) {

// --------------------------------------------------------------------------------------------------------------------
std::function <std::vector <__SYS_STATE>(__SYS_STATE)> UGVmotion = [](__SYS_STATE pose) {

    // ----------------------------------------------------------------------------------------------------------------
	std::function <bool(__SYS_STATE)> valid = [](__SYS_STATE pose) {
		bool OK_XYZgrid;
		if (__DEMtype == 2) OK_XYZgrid = (__inputs->XYZgrid.find(__keyGrid(pose)) == __inputs->XYZgrid.end());
		else OK_XYZgrid = (__inputs->XYZgrid.find(__keyGrid(pose)) != __inputs->XYZgrid.end() && __inputs->XYZgrid[__keyGrid(pose)] != OBSTACLE);
		return (pose.x >= 0 && pose.x < __dimX && pose.y >= 0 && pose.y < __dimY && OK_XYZgrid);
	};

// Main code block of inner function 'UGVmotion'.
{
#ifdef EIGHT_NEIGHBOURHOOD
	const std::vector <std::vector <int>> neighbour_index  {{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};
#else
	const std::vector <std::vector <int>> neighbour_index {{1,0},{0,1},{-1,0},{0,-1}};
#endif
std::vector <__SYS_STATE> result = std::vector <__SYS_STATE>(0);
__SYS_STATE sl = __SYS_STATE();
// In the for-loop, if n is initialized with 0, UGV motions include to the own origin node with {0,0} index
for (FOR_TYPE n = 0; n < (FOR_TYPE) neighbour_index.size(); n++) {
	sl.x = pose.x + neighbour_index[n][0];
	sl.y = pose.y + neighbour_index[n][1];
	// checking that neighbour system state candidate is valid, i.e. belong to environment
	if (!valid(sl)) continue;
	result.push_back(sl);
}
return result;
}

};

// --------------------------------------------------------------------------------------------------------------------
std::function <std::vector <std::tuple <prodNode*, std::vector <segmentItem>, REAL_TYPE, int, bool>>(prodNode*, __SYS_STATE&)> updateCost = [](prodNode* nodeA, __SYS_STATE& stateB) {

	#ifndef NON_ANY_ANGLE
    // ----------------------------------------------------------------------------------------------------------------
	std::function <std::vector <std::vector <REAL_TYPE>>(prodNode&, __SYS_STATE&)> compute_EntryExitPoints = [](prodNode& nodeA, __SYS_STATE& stateB) {

		// ------------------------------------------------------------------------------------------------------------
		std::function <bool(std::vector <REAL_TYPE>, std::vector <REAL_TYPE>)> isEqualPoint = [](std::vector <REAL_TYPE> pointA, std::vector <REAL_TYPE> pointB) {
			return (std::abs(pointA[0]-pointB[0]) < EPSILON && std::abs(pointA[1]-pointB[1]) < EPSILON);
		};

	// Main code block of inner function 'compute_EntryExitPoints'.
	{
	std::vector <std::vector <REAL_TYPE>> result = std::vector <std::vector <REAL_TYPE>>(0);
	result.push_back({nodeA.pose.x, nodeA.pose.y});
	REAL_TYPE stepX = 0, stepY = 0;
	if (stateB.x > nodeA.pose.x) stepX = 1; else if (stateB.x < nodeA.pose.x) stepX = -1;
	if (stateB.y > nodeA.pose.y) stepY = 1; else if (stateB.y < nodeA.pose.y) stepY = -1;
	REAL_TYPE m = (stateB.y - nodeA.pose.y) / (stateB.x - nodeA.pose.x);
	std::vector <REAL_TYPE> pA {nodeA.pose.x + stepX, nodeA.pose.y + m * stepX};
	std::vector <REAL_TYPE> pB {nodeA.pose.x + stepY / m, nodeA.pose.y + stepY};
	if (m == 0)
		if (stepX > 0) pB[0] =  INFINITO;
		else           pB[0] = -INFINITO;
	else if (!std::isfinite(m))
		if (stepY > 0) pA[1] =  INFINITO;
		else           pA[1] = -INFINITO;
	if (m == 1) pB = {stateB.x, stateB.y};
	FOR_TYPE i = 0;
	while (!(pA[0] == stateB.x && pB[1] == stateB.y)) {
		i++;
		if ((pA[0] <= pB[0] && stepX > 0) || (pA[0] >= pB[0] && stepX < 0)) {
			if (!isEqualPoint(pA,result.back())) result.push_back(pA);
			pA[0] += stepX;
			if (std::isfinite(m)) pA[1] = nodeA.pose.y + m * (pA[0] - nodeA.pose.x);
		}
		else {
			if (!isEqualPoint(pB,result.back())) result.push_back(pB);
			pB[1] += stepY;
			if (m != 0) pB[0] = nodeA.pose.x + (pB[1] - nodeA.pose.y) / m;
		}
	}
	result.push_back({stateB.x, stateB.y});
	return result;
	}

	};
	#endif

    // ----------------------------------------------------------------------------------------------------------------
	std::function <std::vector <segmentItem>(std::vector <REAL_TYPE>,std::vector <REAL_TYPE>)> compute_Segments = [](std::vector <REAL_TYPE> point1, std::vector <REAL_TYPE> point2) {

		std::function <REAL_TYPE(std::vector<REAL_TYPE>, std::vector<REAL_TYPE>, std::vector<REAL_TYPE>)> computeZ_Line3D = [](std::vector <REAL_TYPE> point, std::vector <REAL_TYPE> pA, std::vector<REAL_TYPE> pB) {
			std::vector <REAL_TYPE> pointA(pA.begin(), pA.begin() + 1);
			std::vector <REAL_TYPE> pointB(pB.begin(), pB.begin() + 1);
			return (REAL_TYPE) (pA[2]+((pB[1]-pA[1])*euclidean(pointA, point)/euclidean(pointA, pointB)));
		};

		std::function <bool(std::vector <REAL_TYPE>, std::vector <REAL_TYPE>)> isEqualPoint = [](std::vector <REAL_TYPE> pointA, std::vector <REAL_TYPE> pointB) {
			return (std::abs(pointA[0]-pointB[0]) < EPSILON && std::abs(pointA[1]-pointB[1]) < EPSILON);
		};

		std::function <std::vector <REAL_TYPE>(std::vector <REAL_TYPE>)> floorVector = [](std::vector <REAL_TYPE> point) {
			std::vector <REAL_TYPE> result = std::vector <REAL_TYPE>(point.size(),0);
			for (FOR_TYPE i = 0; i < (FOR_TYPE) point.size(); i++) result[i] = std::floor(point[i]);
			return result;
		};

		std::function <std::vector <REAL_TYPE>(std::vector <REAL_TYPE>, std::vector <REAL_TYPE>)> line2D = [](std::vector <REAL_TYPE> pointA, std::vector <REAL_TYPE> pointB) {
			std::vector <REAL_TYPE> result = {(pointB[1]-pointA[1]) / (pointB[0]-pointA[0]), pointA[1] - pointA[0]*(pointB[1]-pointA[1]) / (pointB[0]-pointA[0])};
			return result;
		};

		std::function <std::vector <REAL_TYPE>(std::vector <REAL_TYPE>, std::vector <REAL_TYPE>)> intersection = [](std::vector <REAL_TYPE> lineA, std::vector <REAL_TYPE> lineB) {
			std::vector <REAL_TYPE> result = {(lineB[1]-lineA[1]) / (lineA[0]-lineB[0]), lineA[1] + lineA[0]*(lineB[1]-lineA[1]) / (lineA[0]-lineB[0])};
			return result;
		};

		std::function <bool(std::vector <REAL_TYPE>)> existPoint = [](std::vector <REAL_TYPE> point) {
			return (std::isfinite(point[0]));
		};

		std::function <REAL_TYPE(std::vector <REAL_TYPE>, __SYS_STATE, __SYS_STATE)> estimateZ = [](std::vector <REAL_TYPE> point, __SYS_STATE pA, __SYS_STATE pB) {
			return (REAL_TYPE) pA.z + ((pB.z - pA.z) * euclidean({pA.x, pA.y},point) / euclidean({pA.x, pA.y}, {pB.x, pB.y}));
		};

		std::function <segmentItem(__SYS_STATE, __SYS_STATE, std::vector <__SYS_STATE>)> segment = [](__SYS_STATE pA, __SYS_STATE pB, std::vector <__SYS_STATE> perp) {
			segmentItem item;
			item.point1 = pA;
			item.point2 = pB;
			item.perp = perp;
			return item;
		};

	// Main code block of inner function 'compute_Segments'.
	{
	// Compute cell and its center.
	std::vector <segmentItem> result = std::vector <segmentItem>(0);
	std::vector <REAL_TYPE> cell = std::vector <REAL_TYPE>(point1.size(),0);
	for (FOR_TYPE i = 0; i < (FOR_TYPE) point1.size(); i++) cell[i] = std::floor((point1[i] + point2[i]) / 2);
	std::vector <REAL_TYPE> pointC = {cell[0]+0.5, cell[1]+0.5};
	__SYS_STATE pC = __SYS_STATE(pointC[0], pointC[1]); pC.z = 0;
	const std::vector <std::vector <int>> index {{0,0},{1,0},{1,1},{0,1}};
	std::vector <REAL_TYPE> axeZ = std::vector <REAL_TYPE>(4,INFINITO);
	for (FOR_TYPE i = 0; i < 4; i++) {
		__SYS_STATE sys_state = __SYS_STATE(cell[0]+index[i][0],cell[1]+index[i][1]);
		if (__inputs->XYZgrid.find(__keyGrid(sys_state)) != __inputs->XYZgrid.end()) { axeZ[i] = __inputs->XYZgrid[__keyGrid(sys_state)]; pC.z += axeZ[i]; }
	}
	if (pC.z != OBSTACLE) pC.z /= 4;

	// whether both entry/exit points correspond to nodes
	__SYS_STATE p1 = __SYS_STATE(point1[0], point1[1]);
	__SYS_STATE p2 = __SYS_STATE(point2[0], point2[1]);
	if (isEqualPoint(point1, floorVector(point1)) && isEqualPoint(point2, floorVector(point2))) {
		if (__inputs->XYZgrid.find(__keyGrid(p1)) != __inputs->XYZgrid.end()) p1.z = __inputs->XYZgrid[__keyGrid(p1)];
		if (__inputs->XYZgrid.find(__keyGrid(p2)) != __inputs->XYZgrid.end()) p2.z = __inputs->XYZgrid[__keyGrid(p2)];
		if (point1[0] == point2[0]) {
			std::vector <REAL_TYPE> pointP = {cell[0]-1, cell[1]};
			__SYS_STATE pP = __SYS_STATE(pointP[0]+0.5, pointP[1]+0.5); pP.z = 0;
			for (FOR_TYPE i = 0; i < 4; i++) {
				__SYS_STATE sys_state = __SYS_STATE(pointP[0]+index[i][0],pointP[1]+index[i][1]);
				if (__inputs->XYZgrid.find(__keyGrid(sys_state)) != __inputs->XYZgrid.end()) pP.z += __inputs->XYZgrid[__keyGrid(sys_state)];
			}
			if (pP.z != OBSTACLE) pP.z /= 4;
			result = {segment(p1,p2,{pC,pP})};

		}
		else if (point1[1] == point2[1]) {
			std::vector <REAL_TYPE> pointP = {cell[0], cell[1]-1};
			__SYS_STATE pP = __SYS_STATE(pointP[0]+0.5, pointP[1]+0.5); pP.z = 0;
			for (FOR_TYPE i = 0; i < 4; i++) {
				__SYS_STATE sys_state = __SYS_STATE(pointP[0]+index[i][0],pointP[1]+index[i][1]);
				if (__inputs->XYZgrid.find(__keyGrid(sys_state)) != __inputs->XYZgrid.end()) pP.z += __inputs->XYZgrid[__keyGrid(sys_state)];
			}
			if (pP.z != OBSTACLE) pP.z /= 4;
			result = {segment(p1,p2,{pC,pP})};

		}
		else {
			__SYS_STATE pP1, pP2;
			if (isEqualPoint(point1,cell)) { pP1 = __SYS_STATE(cell[0]+1, cell[1]); pP1.z = axeZ[1]; pP2 = __SYS_STATE(cell[0], cell[1]+1); pP2.z = axeZ[3]; }
			else if (isEqualPoint(point1,{cell[0]+1, cell[1]})) { pP1 = __SYS_STATE(cell[0]+1, cell[1]+1); pP1.z = axeZ[2]; pP2 = __SYS_STATE(cell[0], cell[1]); pP2.z = axeZ[0]; }
			else if (isEqualPoint(point1,{cell[0]+1, cell[1]+1})) { pP1 = __SYS_STATE(cell[0], cell[1]+1); pP1.z = axeZ[3]; pP2 = __SYS_STATE(cell[0]+1, cell[1]); pP2.z = axeZ[1]; }
			else { pP1 = __SYS_STATE(cell[0], cell[1]); pP1.z = axeZ[0]; pP2 = __SYS_STATE(cell[0]+1, cell[1]+1); pP2.z = axeZ[2]; }
			result = {segment(p1,pC,{pP1,pP2}), segment(pC,p2,{pP1,pP2})};

		}
	}

	#ifndef NON_ANY_ANGLE
	// ... or any of entry/exit points does not correspond to node
	else {
		// Look up or estimate altitude corresponding to both entry/exit points.
		__SYS_STATE pE1 = __SYS_STATE(std::floor(point1[0]), std::floor(point1[1]));
		if (__inputs->XYZgrid.find(__keyGrid(pE1)) != __inputs->XYZgrid.end()) pE1.z = __inputs->XYZgrid[__keyGrid(pE1)];
		__SYS_STATE pE2 = __SYS_STATE(std::floor(point2[0]), std::floor(point2[1]));
		if (__inputs->XYZgrid.find(__keyGrid(pE2)) != __inputs->XYZgrid.end()) pE2.z = __inputs->XYZgrid[__keyGrid(pE2)];
		if (isEqualPoint(point1, floorVector(point1))) {
			if (__inputs->XYZgrid.find(__keyGrid(p1)) != __inputs->XYZgrid.end()) p1.z = __inputs->XYZgrid[__keyGrid(p1)];
		}
		else {
			__SYS_STATE pE1p;
			if (point1[0] == std::floor(point1[0])) {
				pE1p = __SYS_STATE(std::floor(point1[0]), std::floor(point1[1]+1));
				if (__inputs->XYZgrid.find(__keyGrid(pE1p)) != __inputs->XYZgrid.end()) pE1p.z = __inputs->XYZgrid[__keyGrid(pE1p)];
			}
			else {
				pE1p = __SYS_STATE(std::floor(point1[0]+1), std::floor(point1[1]));
				if (__inputs->XYZgrid.find(__keyGrid(pE1p)) != __inputs->XYZgrid.end()) pE1p.z = __inputs->XYZgrid[__keyGrid(pE1p)];
			}
			p1.z = estimateZ(point1, pE1, pE1p);
		}
		if (isEqualPoint(point2, floorVector(point2))) {
			if (__inputs->XYZgrid.find(__keyGrid(p2)) != __inputs->XYZgrid.end()) p2.z = __inputs->XYZgrid[__keyGrid(p2)];
		}
		else {
			__SYS_STATE pE2p;
			if (point2[0] == std::floor(point2[0])) {
				pE2p = __SYS_STATE(std::floor(point2[0]), std::floor(point2[1]+1));
				if (__inputs->XYZgrid.find(__keyGrid(pE2p)) != __inputs->XYZgrid.end()) pE2p.z = __inputs->XYZgrid[__keyGrid(pE2p)];
			}
			else {
				pE2p = __SYS_STATE(std::floor(point2[0]+1), std::floor(point2[1]));
				if (__inputs->XYZgrid.find(__keyGrid(pE2p)) != __inputs->XYZgrid.end()) pE2p.z = __inputs->XYZgrid[__keyGrid(pE2p)];
			}
			p2.z = estimateZ(point2, pE2, pE2p);
		}

		// Compute intersections of line segment 'pointA-pointB' with cell diagonals.
		std::vector <REAL_TYPE> pointA = intersection(line2D(point1, point2), line2D(cell, {cell[0]+1, cell[1]+1}));
		__SYS_STATE pA = __SYS_STATE(INFINITO,INFINITO);
		std::vector <REAL_TYPE> pointB = intersection(line2D(point1, point2), line2D({cell[0], cell[1]+1}, {cell[0]+1, cell[1]}));

		if (isEqualPoint(floorVector(pointA),cell) && !isEqualPoint(pointA, point1) && !isEqualPoint(pointA, point2)) {
			__SYS_STATE pEA;
			if (pointA[0] < cell[0]+0.5) {
				pEA = __SYS_STATE(cell[0], cell[1]);
				if (__inputs->XYZgrid.find(__keyGrid(pEA)) != __inputs->XYZgrid.end()) pEA.z = __inputs->XYZgrid[__keyGrid(pEA)];
			}
			else {
				pEA = __SYS_STATE(cell[0]+1, cell[1]+1);
				if (__inputs->XYZgrid.find(__keyGrid(pEA)) != __inputs->XYZgrid.end()) pEA.z = __inputs->XYZgrid[__keyGrid(pEA)];
			}
			pA = __SYS_STATE(pointA[0], pointA[1]);
			pA.z = estimateZ(pointA, pEA, pC);

			if (isEqualPoint(pointA,pointC)) {
				__SYS_STATE pP1, pP2;
				if (isEqualPoint(point1,cell)) { pP1 = __SYS_STATE(cell[0]+1, cell[1]); pP1.z = axeZ[1]; pP2 = __SYS_STATE(cell[0], cell[1]+1); pP2.z = axeZ[3]; }
				else if (isEqualPoint(point1,{cell[0]+1, cell[1]})) { pP1 = __SYS_STATE(cell[0]+1, cell[1]+1); pP1.z = axeZ[2]; pP2 = __SYS_STATE(cell[0], cell[1]); pP2.z = axeZ[0]; }
				else if (isEqualPoint(point1,{cell[0]+1, cell[1]+1})) { pP1 = __SYS_STATE(cell[0], cell[1]+1); pP1.z = axeZ[3]; pP2 = __SYS_STATE(cell[0]+1, cell[1]); pP2.z = axeZ[1]; }
				else { pP1 = __SYS_STATE(cell[0], cell[1]); pP1.z = axeZ[0]; pP2 = __SYS_STATE(cell[0]+1, cell[1]+1); pP2.z = axeZ[2]; }
				result = {segment(p1,pC,{pP1,pP2}), segment(pC,p2,{pP1,pP2})};

			}
			else if (!existPoint(pointB) || !isEqualPoint(floorVector(pointB),cell) || isEqualPoint(pointB,point1) || isEqualPoint(pointB,point2) || isEqualPoint(pointA, pointB)) {
				result = {segment(p1,pA,{pC}), segment(pA,p2,{pC})};

			}
		}
		if (isEqualPoint(floorVector(pointB),cell) && !isEqualPoint(pointA, pointB) && !isEqualPoint(pointB, point1) && !isEqualPoint(pointB, point2)) {
			__SYS_STATE pEB;
			if (pointB[0] < cell[0]+0.5) {
				pEB = __SYS_STATE(cell[0], cell[1]+1);
				if (__inputs->XYZgrid.find(__keyGrid(pEB)) != __inputs->XYZgrid.end()) pEB.z = __inputs->XYZgrid[__keyGrid(pEB)];
			}
			else {
				pEB = __SYS_STATE(cell[0]+1, cell[1]);
				if (__inputs->XYZgrid.find(__keyGrid(pEB)) != __inputs->XYZgrid.end()) pEB.z = __inputs->XYZgrid[__keyGrid(pEB)];
			}
			__SYS_STATE pB = __SYS_STATE(pointB[0], pointB[1]);
			pB.z = estimateZ(pointB, pEB, pC);
			if (existPoint(pointA) && !isEqualPoint(floorVector(pointA),pointA) && isEqualPoint(floorVector(pointA),cell)) {
				if (std::pow(pointA[0]-point1[0], 2) + std::pow(pointA[1]-point1[1], 2) <= std::pow(pointB[0]-point1[0], 2) + std::pow(pointB[1]-point1[1], 2)) {
					result = {segment(p1,pA,{pC}), segment(pA,pB,{pC}), segment(pB,p2,{pC})};

				}
				else {
					result = {segment(p1,pB,{pC}), segment(pB,pA,{pC}), segment(pA,p2,{pC})};

				}
			}
			else {
				result = {segment(p1,pB,{pC}), segment(pB,p2,{pC})};

			}
		}
	}
	#endif
	return result;
	}

	};

    // ----------------------------------------------------------------------------------------------------------------
	std::function <std::tuple <REAL_TYPE, REAL_TYPE, REAL_TYPE, REAL_TYPE, REAL_TYPE, std::vector <int>>(segmentItem, std::vector <int>, bool, bool, bool, bool)> compute_SegmentCost = [](segmentItem item, std::vector <int> Qstate_ini, bool firstCell, bool entryCell, bool exitCell, bool lastCell) {

		std::function <bool(std::vector <REAL_TYPE>, std::vector <REAL_TYPE>)> isEqualPoint = [](std::vector <REAL_TYPE> pointA, std::vector <REAL_TYPE> pointB) {
			return (std::abs(pointA[0]-pointB[0]) < EPSILON && std::abs(pointA[1]-pointB[1]) < EPSILON);
		};

		std::function <std::vector <REAL_TYPE>(std::vector <REAL_TYPE>)> floorVector = [](std::vector <REAL_TYPE> point) {
			std::vector <REAL_TYPE> result = std::vector <REAL_TYPE>(point.size(),0);
			for (FOR_TYPE i = 0; i < (FOR_TYPE) point.size(); i++) result[i] = std::floor(point[i]);
			return result;
		};

	// Main code block of inner function 'compute_SegmentCost'.
	{
	__SYS_STATE point1 = item.point1;
	__SYS_STATE point2 = item.point2;
	std::tuple <REAL_TYPE, REAL_TYPE, REAL_TYPE, REAL_TYPE, REAL_TYPE, std::vector <int>> result = {0.0, 0.0, 0.0, 0.0, 0.0, {NON_QSTATE}};
	REAL_TYPE distSquare;
	REAL_TYPE coefC, coefB = std::pow(__DEM_resolution, 2) * ((std::pow(point2.x - point1.x, 2) + std::pow(point2.y - point1.y, 2)));
	item.refSpeed		= 1.0;
	item.proposedSlope	= 0.0;
	item.slope			= 0.0;
	item.tilt			= 0.0;
	if (__DEMtype != 2) {
		// Computing actual slope for motion constraint, as DEMAIA (the inclination of path segment).
		REAL_TYPE coefA = __DEM_resolution * (point2.z - point1.z);
		if (coefB != 0) item.slope = std::atan(coefA / std::sqrt(coefB));
		distSquare = std::pow(coefA, 2) + coefB;
		// Computing actual tilt for motion constraint.
		for (FOR_TYPE i = 0; i < (FOR_TYPE) item.perp.size(); i++) {
			__SYS_STATE p = item.perp[i];
			REAL_TYPE coefCx = (p.y - point1.y) * (point2.z - point1.z) - (p.z - point1.z) * (point2.y - point1.y);
			REAL_TYPE coefCy = (p.x - point1.x) * (point2.z - point1.z) - (p.z - point1.z) * (point2.x - point1.x);
			REAL_TYPE coefCz = (p.x - point1.x) * (point2.y - point1.y) - (p.y - point1.y) * (point2.x - point1.x);
			coefC  = std::pow(coefCx, 2) + std::pow(coefCy, 2) + std::pow(coefCz, 2);
			REAL_TYPE actualCosineTilt = std::abs(coefCz) * std::sqrt(distSquare / (coefB * coefC));
			item.tilt += std::acos(actualCosineTilt);
			item.tilt /= item.perp.size();
		}
		#if defined(NON_ASYMMETRIC) || defined(VERBOSE_MATLAB)
			// Computing estimated slope for motion constraint, as 3Dana (the highest inclination of cell traingle traversed by path segment).
			std::vector <REAL_TYPE> tmp = {0.0, 0.0};
			std::vector <REAL_TYPE> a = {point2.x-point1.x, point2.y-point1.y, point2.z-point1.z};
			for (FOR_TYPE i = 0; i < (FOR_TYPE) item.perp.size(); i++) {
				std::vector <REAL_TYPE> b = {item.perp[i].x-point1.x, item.perp[i].y-point1.y, item.perp[i].z-point1.z};
				std::vector <REAL_TYPE> cross = {a[1]*b[2]-a[2]*b[1], -(a[0]*b[2]-a[2]*b[0]), a[0]*b[1]-a[1]*b[0]};
				REAL_TYPE sum_cross = std::pow(cross[0], 2) + std::pow(cross[1], 2) + std::pow(cross[2], 2);
				if (std::isnan(cross[0])) tmp[i] = INFINITO;
				else item.proposedSlope += std::acos(std::abs(cross[2]) / std::sqrt( sum_cross ));
			}
			item.proposedSlope /= item.perp.size();
		#endif

		// Computing the segment speed that corresponds to pitch and rool angles (slope and tilt, theta and phi)
		int offset = ((__inputs->vRef.size()-1)/2);
		int i, j = offset;
		#ifdef NON_ASYMMETRIC
			i = (int) (offset + item.proposedSlope / __step);
		#else
			i = (int) (offset + item.slope / __step);
			j = (int) (offset + item.tilt / __step);
		#endif
		item.refSpeed = (i <= __inputs->vRef.size() && j <= __inputs->vRef.size()) ? __inputs->vRef[i][j] : 0.0;
	}
	else {
		if (point1.z != OBSTACLE && point2.z != OBSTACLE) distSquare = coefB;
		else distSquare = INFINITO;
	}
	if (item.refSpeed == 0) std::get<0>(result) = INFINITO; else std::get<0>(result) = (REAL_TYPE) std::sqrt(distSquare) / item.refSpeed;
	std::get<1>(result) = (REAL_TYPE) item.refSpeed;
	std::get<2>(result) = (REAL_TYPE) item.proposedSlope;
	std::get<3>(result) = (REAL_TYPE) item.slope;
	std::get<4>(result) = (REAL_TYPE) item.tilt;


	// Verifying UGV mission specification, as LTL fullfilment
	// Computing the next Qstate from the initial state and the UGV mission automaton for path segment
	if (std::isfinite(std::get<0>(result))) {
		bool anyTransition_satisfied = false;
		std::vector <int> ql = std::vector <int>(0);
		std::vector <REAL_TYPE> p1 = {point1.x, point1.y};
		std::vector <REAL_TYPE> p2 = {point2.x, point2.y};
		if (!(isEqualPoint(p1,p2)) || firstCell && lastCell) {
			__SYS_STATE sl;
			if (exitCell) sl = __SYS_STATE(std::floor(point2.x),std::floor(point2.y));
			else sl = __SYS_STATE(std::floor((point1.x + point2.x) / 2),std::floor((point1.y + point2.y) / 2));

			std::vector <bool> checkedQstate = std::vector <bool>(__Qstates_cnt,false);
			for (FOR_TYPE n = 0; n < (FOR_TYPE) Qstate_ini.size(); n++) {
				// Querying atomic propositions satisfied in the current system state
				std::vector <int> current_prop = {0};
				if (__propByKey.find(__key(sl)) != __propByKey.end()) current_prop = __propByKey[__key(sl)];
				// Determining automaton transitions (non-deterministic automaton) that can be satisfied in the current system state, i.e. the next Qstates
				for (FOR_TYPE i = 0; i < __Qstates_cnt; i++) {
					for (FOR_TYPE j = 0; j < (FOR_TYPE) __Qtransitions[Qstate_ini[n]][i].size(); j++) {
						int OK_cnt = 0;
						bool transition_satisfied = false;
						int tmp;
						for (FOR_TYPE k = 0; k < (FOR_TYPE) __Qtransitions[Qstate_ini[n]][i][j].size(); k++) {
							if (__Qtransitions[Qstate_ini[n]][i][j][k] >= 0) {
								tmp = 0;
								for (FOR_TYPE l = 0; l < (FOR_TYPE) current_prop.size(); l++)
									if (__Qtransitions[Qstate_ini[n]][i][j][k] == current_prop[l]) { tmp = 1; break; }
								OK_cnt += tmp;
							}
							else {
								tmp = 1;
								for (FOR_TYPE l = 0; l < (FOR_TYPE) current_prop.size(); l++)
									if (-1 * __Qtransitions[Qstate_ini[n]][i][j][k] == current_prop[l]) { tmp = 0; break; }
								OK_cnt += tmp;
							}
							if (tmp == 0) break;
						}
						transition_satisfied = (OK_cnt == __Qtransitions[Qstate_ini[n]][i][j].size());
						anyTransition_satisfied = (anyTransition_satisfied || transition_satisfied);
						if (transition_satisfied)
							if (!checkedQstate[i]) { ql.push_back(i); checkedQstate[i] = true; }
					}
					/// if (anyTransition_satisfied) break;  // this command sequence MUST BE commented for applying over NON-DETERMINISTIC automata, evaluating the rest of automaton transitions
				}
			}
		}
		if (firstCell && !exitCell && isEqualPoint(p1,floorVector(p2))) std::get<5>(result) = Qstate_ini;
		else if (anyTransition_satisfied) std::get<5>(result) = ql; else std::get<0>(result) = INFINITO;
	}

	if (!(std::isfinite(std::get<0>(result)))) std::get<5>(result) = {NON_QSTATE};
	return result;
	}

	};

// Main code block of inner function 'updateCost'.
{
std::vector <std::tuple <prodNode*, std::vector <segmentItem>, REAL_TYPE, int, bool>> lowestCost = std::vector <std::tuple <prodNode*, std::vector <segmentItem>, REAL_TYPE, int, bool>>{0};
std::tuple <REAL_TYPE, REAL_TYPE, REAL_TYPE, REAL_TYPE, REAL_TYPE, std::vector <int>> segment_tmp;
bool firstCell, entryCell, exitCell, lastCell;
#ifndef NON_ANY_ANGLE
	REAL_TYPE g1 = INFINITO;
	std::vector <int> g1Qstate = {NON_QSTATE};
	std::vector <segmentItem> ipG1 = std::vector <segmentItem>(0);
	if (nodeA->parent != NULL) {
		g1 = nodeA->parent->g;
		g1Qstate = {nodeA->parent->Qstate};
		#ifdef VERBOSE_DEBUG
			std::cout << "      any-angle: ";
			std::cout << "(" << nodeA->parent->pose.x+1 << "," << nodeA->parent->pose.y+1 << ")-(" << stateB.x+1 << "," << stateB.y+1 << ") ";
			std::cout << "g1{Qstate}=" << g1 << "{" << g1Qstate[0] << "}" << ", ";
		#endif
		std::vector <std::vector <REAL_TYPE>> io_points = compute_EntryExitPoints(*nodeA->parent, stateB);
		#ifdef VERBOSE_DEBUG
			std::cout << "Traversed cells:" << io_points.size()-1 << ", ";
		#endif
		for (FOR_TYPE i = 1; i < io_points.size() && std::isfinite(g1); i++) {
			#ifdef VERBOSE_DEBUG
				std::cout << "#" << i << "# ";
			#endif
			std::vector <segmentItem> ipG1_tmp = compute_Segments(io_points[i-1],io_points[i]);
			for (FOR_TYPE j = 0; j < ipG1_tmp.size() && std::isfinite(g1); j++) {
				segment_tmp = compute_SegmentCost(ipG1_tmp[j], g1Qstate, (i == 1), (j == 0), (j == ipG1_tmp.size()-1), (i == io_points.size()-1));
				g1 += std::get<0>(segment_tmp);
				ipG1_tmp[j].refSpeed = std::get<1>(segment_tmp);
				ipG1_tmp[j].proposedSlope = std::get<2>(segment_tmp);
				ipG1_tmp[j].slope = std::get<3>(segment_tmp);
				ipG1_tmp[j].tilt = std::get<4>(segment_tmp);
				g1Qstate = std::get<5>(segment_tmp);

			}
			ipG1.insert(ipG1.end(),ipG1_tmp.begin(),ipG1_tmp.end());
		}
		#ifdef VERBOSE_DEBUG
			printf("\b\b.\n");
		#endif
	}
#endif

REAL_TYPE g2 = nodeA->g;
std::vector <int> g2Qstate = {nodeA->Qstate};

std::vector <segmentItem> ipG2 = compute_Segments({nodeA->pose.x, nodeA->pose.y}, {stateB.x, stateB.y});
for (FOR_TYPE j = 0; j < ipG2.size() && std::isfinite(g2); j++) {
	segment_tmp = compute_SegmentCost(ipG2[j], g2Qstate, true, (j == 0), (j == ipG2.size()-1), true);
	g2 += std::get<0>(segment_tmp);
	ipG2[j].refSpeed = std::get<1>(segment_tmp);
	ipG2[j].proposedSlope = std::get<2>(segment_tmp);
	ipG2[j].slope = std::get<3>(segment_tmp);
	ipG2[j].tilt = std::get<4>(segment_tmp);
	g2Qstate = std::get<5>(segment_tmp);

}
#if defined(VERBOSE_DEBUG) && !defined(NON_ANY_ANGLE)
	if (std::isfinite(g1) && g1 > g2) std::cout << "- NOTE: The cost of the any-angle segment path is higher than A*-like approach (due to slope-awareness)." << std::endl;
#endif

prodNode* node = new_prod_graph_node(stateB, NON_QSTATE);
KEY_TYPE key;
REAL_TYPE gB;
std::vector <bool> checkedStatesQ2 = std::vector <bool>(g2Qstate.size(),false);
unsigned aux = 0;
#ifndef NON_ANY_ANGLE
	std::vector <bool> checkedStatesQ1 = std::vector <bool>(g1Qstate.size(),false);
	for (FOR_TYPE i = 0; i < (FOR_TYPE) g1Qstate.size(); i++)
		for (FOR_TYPE j = 0; j < (FOR_TYPE) g2Qstate.size(); j++) {
			if (g1Qstate[i] == g2Qstate[j]) {
				checkedStatesQ1[i] = true; checkedStatesQ2[j] = true;
				node->Qstate = g1Qstate[i]; key = __nodeKey(node);
				if (__discoveredNode.find(key) != __discoveredNode.end()) gB = __discoveredNode[key]->g;
				else gB = INFINITO;
				if (g1Qstate[i] != NON_QSTATE && std::isfinite(g1) && g1 < gB && g1 <= g2) lowestCost.push_back({nodeA->parent, ipG1, g1, g1Qstate[i], true});
				else if (g2Qstate[j] != NON_QSTATE && std::isfinite(g2) && g2 < gB) lowestCost.push_back({nodeA, ipG2, g2, g2Qstate[j], false});
			}
		}
	for (FOR_TYPE i = 0; i < (FOR_TYPE) g1Qstate.size(); i++)
		if (!checkedStatesQ1[i]) {
			node->Qstate = g1Qstate[i]; key = __nodeKey(node);
			if (__discoveredNode.find(key) != __discoveredNode.end()) gB = __discoveredNode[key]->g;
			else gB = INFINITO;
			if (g1Qstate[i] != NON_QSTATE && std::isfinite(g1) && g1 < gB) lowestCost.push_back({nodeA->parent, ipG1, g1, g1Qstate[i], true});
		}
	for (FOR_TYPE j = 0; j < (FOR_TYPE) g2Qstate.size(); j++)
		if (!checkedStatesQ2[j]) {
			node->Qstate = g2Qstate[j]; key = __nodeKey(node);
			if (__discoveredNode.find(key) != __discoveredNode.end()) gB = __discoveredNode[key]->g;
			else gB = INFINITO;
			if (g2Qstate[j] != NON_QSTATE && std::isfinite(g2) && g2 < gB) lowestCost.push_back({nodeA, ipG2, g2, g2Qstate[j], false});
		}
#endif
#ifdef NON_ANY_ANGLE
	for (FOR_TYPE j = 0; j < (FOR_TYPE) g2Qstate.size(); j++) {
		node->Qstate = g2Qstate[j]; key = __nodeKey(node);
		if (__discoveredNode.find(key) != __discoveredNode.end()) gB = __discoveredNode[key]->g;
		else gB = INFINITO;
		if (g2Qstate[j] != NON_QSTATE && std::isfinite(g2) && g2 < gB) lowestCost.push_back({nodeA, ipG2, g2, g2Qstate[j], false});
	}
#endif
delete node;
return lowestCost;
}

};

#if defined(EUCLIDEAN_TIME) || defined(OCTILE_TIME) || defined(OCTILE_2D_TIME)
// --------------------------------------------------------------------------------------------------------------------
std::function <REAL_TYPE(__SYS_STATE&, std::vector <prodNode*>)> heuristic = [](__SYS_STATE& poseA, std::vector <prodNode*> destNodes) {

REAL_TYPE travelTime = INFINITO;
KEY_TYPE idx = __keyGrid(poseA);
if (__DEMtype == 2) poseA.z = 0.0; 
else if (__inputs->XYZgrid.find(idx) != __inputs->XYZgrid.end()) poseA.z = __inputs->XYZgrid[idx];
else poseA.z = INFINITO;
for (FOR_TYPE i = 0; i < (FOR_TYPE) destNodes.size(); i++) {
	idx = __keyGrid(destNodes[i]->pose);
	if (__DEMtype == 2) destNodes[i]->pose.z = 0.0;
	else if (__inputs->XYZgrid.find(idx) != __inputs->XYZgrid.end()) destNodes[i]->pose.z = __inputs->XYZgrid[idx];
	else destNodes[i]->pose.z = INFINITO;
	REAL_TYPE deltaI = destNodes[i]->pose.x - poseA.x, deltaJ = destNodes[i]->pose.y - poseA.y, deltaK = destNodes[i]->pose.z - poseA.z;
	#ifdef EUCLIDEAN_TIME
		REAL_TYPE dist = std::sqrt(std::pow(deltaI, 2) + std::pow(deltaJ, 2) + std::pow(deltaK, 2));
	#endif
	#ifdef OCTILE_TIME
		REAL_TYPE dist = std::sqrt(std::pow(std::abs(deltaI - deltaJ) + std::sqrt(2.0) * std::fmin(deltaI, deltaJ), 2) + std::pow(deltaK, 2));
	#endif
	#ifdef OCTILE_2D_TIME
		REAL_TYPE mx = fmax(std::abs(destNodes[i]->pose.x - poseA.x),std::abs(destNodes[i]->pose.y - poseA.y));
		REAL_TYPE mn = fmin(std::abs(destNodes[i]->pose.x - poseA.x),std::abs(destNodes[i]->pose.y - poseA.y));
		REAL_TYPE dist = (REAL_TYPE) (mn * 1.5 + (mx - mn));
	#endif
	travelTime = std::fmin(travelTime, dist);
}
int offset = ((__inputs->vRef.size()-1)/2);
return (REAL_TYPE)(travelTime/__inputs->vRef[offset][offset]);

};
#endif

// --------------------------------------------------------------------------------------------------------------------
std::function <bool(prodNode*, std::vector <prodNode*>)> belongsTo = [](prodNode* nodeA, std::vector <prodNode*> nodeB) {
	bool result = false;
	for (FOR_TYPE i = 0; i < nodeB.size() && !result; i++) result = (__nodeKey(nodeA) == __nodeKey(nodeB[i]));
	return result;
};

// Main code block of function 'path_search'
{
__PATH* path = new_path_node(INFINITO);
prodNode* node = new_prod_graph_node(srcNode->pose, srcNode->Qstate);
std::priority_queue <prodNode*, std::vector <prodNode*>, prod_graph_comparator> qopen;		// dinamic list with product nodes ordered by cost plus heuristic function
__discoveredNode = std::unordered_map <KEY_TYPE, prodNode*>(0);
KEY_TYPE iteration = 0;
#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
	KEY_TYPE updates_cnt = 0, excludes_cnt = 0;
#endif

// Initialize the heuristic matrix H.
#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME) 
	#if defined(EUCLIDEAN_TIME) || defined(OCTILE_TIME) || defined(OCTILE_2D_TIME)
		std::cout << " Computing heuristic matrix H with ";
		#ifdef EUCLIDEAN_TIME
			std::cout << "Euclidean-time heuristic function... ";
		#endif
		#ifdef OCTILE_TIME
			std::cout << "octile-time heuristic function... ";
		#endif
		#ifdef OCTILE_2D_TIME
			std::cout << "2D octile-time heuristic function... ";
		#endif
	#endif
	#if !defined(EUCLIDEAN_TIME) && !defined(OCTILE_TIME) && !defined(OCTILE_2D_TIME)
		std::cout << " Computing non-heuristic matrix H with zero function (as Dijkstra algorithm)... ";
	#endif
#endif
std::vector <std::vector <REAL_TYPE>> H(__dimX, std::vector <REAL_TYPE>(__dimY,0.0));
#if defined(EUCLIDEAN_TIME) || defined(OCTILE_TIME) || defined(OCTILE_2D_TIME)
	for (FOR_TYPE x = 0; x < __dimX; x++)
		for (FOR_TYPE y = 0; y < __dimY; y++) {
			__SYS_STATE Sstate = __SYS_STATE(x,y);
			H[x][y] = heuristic(Sstate,destNodes);
		}
#endif

bool isInfinityPath = false;
for (FOR_TYPE i = 0; i < (FOR_TYPE) __accepting_Qstates.size(); i++) if (__accepting_Qstates[i] == 0 || i > 0) { isInfinityPath = true; break; }
qopen.push(srcNode);

// Iterative algorithm: path searching
while (!qopen.empty() && (!belongsTo(node,destNodes) || (iteration <= 1 && isInfinityPath))) {
	#ifdef VERBOSE_DEBUG
		std::cout << std::endl;
	#endif
	iteration++;
	node = qopen.top();
	qopen.pop();
	// Checking current cost value (avoiding old values that were updated already)
	if ((__discoveredNode.find(__nodeKey(node)) != __discoveredNode.end()) && node->f != __discoveredNode[__nodeKey(node)]->f) {
		#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
			excludes_cnt++;
		#endif
		continue;
	}
	if ((__discoveredNode.find(__nodeKey(node)) != __discoveredNode.end()) && __discoveredNode[__nodeKey(node)]->g > minCost_found && (iteration >= 3 && isInfinityPath)) {
		#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
			std::cout << std::endl << " \033[1;33m\033[1;202mExcedded a previous minimum-cost found... aborting searching.\033[0m" << std::endl;
		#endif
		break;	// while-break for exceding previous minimum-cost found
	}



	// Selecting automaton outcoming transitions for the current Qstate only
	int Qstate = node->Qstate;


	// Building a new level of path-search tree over the product automaton graph, so evaluating edge cost with DEMAIA alternatives and UGV mission specification
	__SYS_STATE sl = __SYS_STATE();
	std::vector <__SYS_STATE> neighbour_states = UGVmotion(node->pose);
	for (FOR_TYPE n = 0; n < (FOR_TYPE) neighbour_states.size(); n++) {
		sl = neighbour_states[n];
		#ifdef VERBOSE_DEBUG
			std::cout << std::endl << "    candidate to neighbour system state: (";
			std::cout << sl.x+1 << "," << sl.y+1;
			std::cout << ")" << std::endl;
		#endif

		std::vector <std::tuple <prodNode*, std::vector <segmentItem>, REAL_TYPE, int, bool>> minCost = updateCost(node, sl);
		std::vector <prodNode*> neighbour_node = std::vector <prodNode*>(minCost.size());
		KEY_TYPE key;
		for (FOR_TYPE j = 0; j < (FOR_TYPE) minCost.size(); j++) {
			neighbour_node[j] = new_prod_graph_node(sl, std::get<3>(minCost[j]));
			key = __nodeKey(neighbour_node[j]);
			
			#ifdef VERBOSE_DEBUG
				std::cout << std::endl << "      Satisfying transition towards system state [ (";
				std::cout << sl.x+1 << "," << sl.y+1;
				std::cout << "), " << neighbour_node[j]->Qstate << " ]";
			#endif

			#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
				if (__discoveredNode.find(key) != __discoveredNode.end()) updates_cnt++;
			#endif
			if (__discoveredNode.find(key) == __discoveredNode.end())
				__discoveredNode[key] = neighbour_node[j];
			else delete neighbour_node[j];
			// wr(vi(si,qi),vl(sl,ql)) <- cost(si,sl)
			__discoveredNode[key]->parent	= std::get<0>(minCost[j]);
			__discoveredNode[key]->segment	= std::get<1>(minCost[j]);
			__discoveredNode[key]->g		= std::get<2>(minCost[j]);
			__discoveredNode[key]->h		= H[sl.x][sl.y];
			__discoveredNode[key]->f		= __discoveredNode[key]->g + __discoveredNode[key]->h;
			// Ur(vi(si,qi),vl(sl,ql)) <- true
///			actualCostEdgeNode[__nodeKey(neighbour_node[j])] = true;
			// if vl(sl,ql) is not labelled as discovered then
			//   · label vl(sl,ql) as discovered and add it to Gr
			//   · Q.enqueue(vl(sl,ql))
			qopen.push(__discoveredNode[key]);
			#ifdef VERBOSE_DEBUG
				std::string g_type = (std::get<4>(minCost[j])) ? " as any-angle approach" : " as A*-like approach";
				std::cout << " and \033[1;33m\033[1;202mupdating priority queue...\033[0m with g = " << std::get<2>(minCost[j]) << g_type.c_str();
//				if (i < ql.size()-1) std::cout << std::endl;
			#endif
		}
	}
}

#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
	if (iteration) {
		std::cout << std::endl;
		if (qopen.empty() && !belongsTo(node,destNodes)) std::cout << " \033[1;31m\033[1;202mNot found a resulting path\033[0m (QOpen ordered list is empty)." << std::endl;
		if (belongsTo(node,destNodes)) std::cout << " \033[1;34m\033[1;202mFound a resulting path.\033[0m" << std::endl;
		std::cout << "  Number of iterations performed in DEM-LTL search algorithm = " << iteration << std::endl;
		std::cout << "  Number of discovered product automaton nodes = " << __discoveredNode.size() << std::endl;
		std::cout << "  Number of re-updates in the priority queue for a lower cost value = " << updates_cnt << std::endl;
		REAL_TYPE percent = (iteration) ? excludes_cnt * 100.00 / iteration : 0; 
		std::cout << "  Number of pulled nodes from the priority queue that are not reevaluated = " << excludes_cnt << ", i.e. " << percent << " % over total iterations" << std::endl;
	}
#endif

// obtaining the resulting path by backtracking from target node to initial node in the product automaton
path->waypoint = std::deque <__SYS_STATE>(0);
if (belongsTo(node,destNodes)) {
	path->iterations = iteration;
	path->total_cost = node->f;
	prodNode* path_node = node;
	while (path_node->parent != NULL) {
		path->node.push_front(path_node);
		for (FOR_TYPE n = path_node->segment.size()-1; n >= 0; n--) {
			path->waypoint.push_front(path_node->segment[n].point2);
			path->refSpeed.push_front(path_node->segment[n].refSpeed);
			path->proposedSlope.push_front(path_node->segment[n].proposedSlope);
			path->slope.push_front(path_node->segment[n].slope);
			path->tilt.push_front(path_node->segment[n].tilt);
		}
		path_node = path_node->parent;
	}
	path->node.push_front(path_node);
	if (__DEMtype == 2) path_node->pose.z = 0.0;
	else if (__inputs->XYZgrid.find(__keyGrid(path_node->pose)) != __inputs->XYZgrid.end()) path_node->pose.z = __inputs->XYZgrid[__keyGrid(path_node->pose)];
	else path_node->pose.z = INFINITO;
	path->waypoint.push_front(path_node->pose);
	path->refSpeed.push_front(0.0);
	path->proposedSlope.push_front(0.0);
	path->slope.push_front(0.0);
	path->tilt.push_front(0.0);
}

if (path->node.size() == 3 && __keyGrid(path->node[0]->pose) == __keyGrid(path->node[2]->pose))
	path = new_path_node(INFINITO);

#if !defined(VERBOSE_NONE) && !defined(VERBOSE_TIME)
	// showing the resulting node sequence; the resulting waypoints sequence will be shown at the end of strategic trajectory planning
	if (iteration) {
		std::cout << "\033[4mCandidate path\033[0m [ length = " << path->node.size() << " system states and " << path->waypoint.size() << " waypoints, total iterations = " << path->iterations << ", total cost = " << path->total_cost << " s ]" << std::endl;
		if (path->node.size() > 0) std::cout << "{ ";
		for (FOR_TYPE n = 0; n < (FOR_TYPE) path->node.size(); n++) {
			std::cout << "(";
			std::cout << path->node[n]->pose.x+1 << "," << path->node[n]->pose.y+1;
			std::cout << "),";
		}
		if (path->node.size() > 0) std::cout << "\b }" << std::endl;
	}
#endif
return path;
}

}


