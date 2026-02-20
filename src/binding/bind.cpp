#include "MPC.h"
#include "MPCPolicy.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

// Make sure use the same name in CMakeLists
PYBIND11_MODULE(_mpc, m) {
    py::class_<MPCPolicy>(m, "MPCPolicy")
        .def(py::init<>())  // constructor
        .def("reset", &MPCPolicy::reset)
        .def("set_dt", &MPCPolicy::setDt)
        .def("set_lf", &MPCPolicy::setLf)
        .def("get_dt", &MPCPolicy::getDt)
        .def("get_lf", &MPCPolicy::getLf)
        .def("step", &MPCPolicy::step);
    py::class_<MPC>(m, "MPC")
        .def(py::init<>())
        .def("solve", &MPC::Solve); 
}
