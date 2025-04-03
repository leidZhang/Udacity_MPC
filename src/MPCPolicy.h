#ifndef MPC_POLICY_H
#define MPC_POLICY_H
#include "MPC.h"
#include <vector>

class MPCPolicy {
private:
    MPC mpc; 
    std::vector<double> action;

public:
    MPCPolicy(); 
    void reset();     
    void setDt(double dt); 
    double getDt(); 
    double getLf();
    std::vector<double> step(std::vector<std::vector<double>> &nextWaypoints, 
                             std::vector<double> &currentState);
};

#endif