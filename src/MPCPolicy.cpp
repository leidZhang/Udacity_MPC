#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include "MPC.h"
#include "helpers.h"
#include "MPCPolicy.h"

using std::string;
using std::vector;

constexpr double pi() {return M_PI; }
double deg2rad(double x) {return x * pi() / 180; }
double rad2deg(double x) {return x * 180 / pi(); }

MPCPolicy::MPCPolicy() : mpc() {}

void MPCPolicy::reset() {this->action = {0, 0}; }

void MPCPolicy::setDt(double dt) {mpc.setDt(dt); }

void MPCPolicy::setLf(double Lf) {mpc.setLf(Lf); }

double MPCPolicy::getDt() {return mpc.dt; }

double MPCPolicy::getLf() {return mpc.Lf; }

vector<double> MPCPolicy::step(vector<vector<double>> &nextWaypoints, vector<double> &currentStates) {
    // Read state information
    double px = currentStates[0]; // x coord?
    double py = currentStates[1]; // y coord?
    double psi = currentStates[2]; // theta?
    double v = currentStates[3]; // velocity?
    // Read last action
    double a = currentStates[5];
    // double a = this->action[0]; // throttle
    double delta = this->action[1]; // steering?

    // Convert to local frame coord system
    const size_t num_of_waypoints = nextWaypoints.size();
    std::cout << num_of_waypoints << std::endl;
    Eigen::VectorXd waypoints_x_carcoord(num_of_waypoints);
    Eigen::VectorXd waypoints_y_carcoord(num_of_waypoints);
    for (size_t i = 0; i < num_of_waypoints; i++) {
        // double diff_x = nextWaypoints[i][0] - px;
        // double diff_y = nextWaypoints[i][1] - py;
        // waypoints_x_carcoord[i] = diff_x * cos(-psi) - diff_y * sin(-psi);
        // waypoints_y_carcoord[i] = diff_y * cos(-psi) + diff_x * sin(-psi);

        waypoints_x_carcoord[i] = nextWaypoints[i][0];
        waypoints_y_carcoord[i] = nextWaypoints[i][1];
    }

    // Fit polynomial to waypoints
    Eigen::VectorXd new_coeffs = polyfit(waypoints_x_carcoord, waypoints_y_carcoord, 3);
    // Calculate current cross-track error (cte) and orientation error (epsi)
    double cte = new_coeffs[0]; // current cte
    double epsi = -atan(new_coeffs[1]); // current heading error

    // Set up the state vector for MPC solver
    double next_px = 0.0 + v * mpc.dt;
    double next_py = 0.0;
    double next_psi = 0.0 - v * delta / mpc.Lf * mpc.dt;
    double next_v = v + a * mpc.dt;
    double next_cte = cte + v * sin(epsi) * mpc.dt;
    double next_epsi = epsi + v * (-delta) / mpc.Lf * mpc.dt;
    Eigen::VectorXd mpc_state(mpc.num_of_states);
    mpc_state << next_px, next_py, next_psi, next_v, next_cte, next_epsi;

    // Solve the MPC problem and get the solution
    vector<double> stateVec(mpc_state.data(), mpc_state.data() + mpc_state.size());
    vector<double> newCoeffsVec(new_coeffs.data(), new_coeffs.data() + new_coeffs.size());
    // vector<double> solution = mpc.Solve(mpc_state, new_coeffs);
    vector<double> solution = mpc.Solve(stateVec, newCoeffsVec);
    double steering = solution[0];
    double throttle = solution[1];

    // Update action and return it
    this->action = {throttle, steering, v};

    // Display the trajectory
    vector<double> mpc_x_vals;
    vector<double> mpc_y_vals;
    for ( size_t i = 2; i < solution.size(); i++ ) {
        if ( i % 2 == 0 ) {
            mpc_x_vals.push_back( solution[i] );
            std::cout << "mpc_x_vals: " << solution[i] << std::endl;
        } else {
            mpc_y_vals.push_back( solution[i] );
            std::cout << "mpc_y_vals: " << solution[i] << std::endl;
        }
    }

    // Display the waypoints/reference line
    vector<double> next_x_vals;
    vector<double> next_y_vals;
    const double poly_step = 5.0;
    const int poly_num = 10;
    for (size_t i = 0; i < poly_num; i++) {
        double x_coord = poly_step * i;
        double y_coord = polyeval(new_coeffs, x_coord);
        next_x_vals.push_back(x_coord);
        next_y_vals.push_back(y_coord);
        std::cout << "x_coord: " << x_coord << " y_coord: " << y_coord << std::endl;
    }

    return this->action;
}
