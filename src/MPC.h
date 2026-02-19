#ifndef MPC_H
#define MPC_H

#include <vector>
#include "Eigen-3.3/Eigen/Core"

class MPC {
	public:
	/**
	 * TODO: Set the timestep length and duration
	 */
	double dt = 0.1;
    double Lf = 2.67;

	const int num_of_states = 6; // px, py, psi, v, cte, epsi
	const int num_of_actutions = 2; // acceleration, steering angle

	// This value assumes the model presented in the classroom is used.
	//
	// It was obtained by measuring the radius formed by running the vehicle in the
	//   simulator around in a circle with a constant steering angle and velocity on
	//   a flat terrain.
	//
	// Lf was tuned until the the radius formed by the simulating the model
	//   presented in the classroom matched the previous radius.
	//
	// This is the length from front to CoG that has a similar radius.

	MPC();

	virtual ~MPC();

	// Solve the model given an initial state and polynomial coefficients.
	// Return the first actuations.
	void setDt(double dt);
        void setLf(double Lf);
	std::vector<double> Solve(const std::vector<double> &state, const std::vector<double> &coeffs);
};

#endif  // MPC_H
