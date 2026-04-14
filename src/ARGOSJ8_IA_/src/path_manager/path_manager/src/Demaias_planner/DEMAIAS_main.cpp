#include <bits/stdc++.h>

namespace DEMAIAS
{

int planner(char* DEM, char* mapping, char* LTL);

}


int main(int args, char **argv) {

int ans;

/*
// example of 2D environments, as an 'any-angle' variant of T* planner algorithm
DEMAIAS::planner("exDBM.dat", "", "exLTL-a.dat");
DEMAIAS::planner("exDBM.dat", "", "exLTL-b.dat");
DEMAIAS::planner("2dDBM.dat", "", "2dLTL.dat");

// example of artificial DEM
DEMAIAS::planner("2.5dDEM.xyz", "2.5dDEM-mapping.dat", "2.5dLTL.dat");

// examples of real-world DEM with resolutions 1 and 2 m.
DEMAIAS::planner("DEM1m.xyz", "DEM1m-mapping.dat", "DEM1m-LTL_mission.dat");
DEMAIAS::planner("DEM2m.xyz", "DEM2m-mapping.dat", "DEM2m-LTL_mission.dat");

// examples of real-world DEM with resolutions 1 and 2 m.
DEMAIAS::planner("2.5dDEMrgb.xyz", "2.5dDEMrgb-mapping.dat", "2.5dLTLrgb.dat");
DEMAIAS::planner("2.5dDEMrgb_2.xyz", "2.5dDEMrgb-mapping.dat", "2.5dLTLrgb.dat");

// examples of errors
DEMAIAS::planner("DEM1m.xyz", "DEM1m-mapping.dat", "no_filename.dat");
DEMAIAS::planner("DEM1m.xyz", "DEM1m-mapping.dat", "LTL-mission-full-automaton-free.dat");
*/

if (args > 1) DEMAIAS::planner(argv[1], argv[2], argv[3]);
else std::cout << "\033[1;31mError: Input arguments not found.\033[0m" << std::endl;
return 0;

}