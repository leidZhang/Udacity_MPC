project(MPC)

cmake_minimum_required (VERSION 3.5)

# -g allows for gdb debugging
# turn on -03 for best performance
add_definitions(-std=c++11 -O3)
add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 11)

# set(sources src/MPC.cpp src/main.cpp)
set(sources src/MPC.cpp)
# Ensure position-independent code for shared libraries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Install python3.8 and python3.8-dev
set(Python3_EXECUTABLE "/usr/bin/python3.8")
find_package(Python3 3.8 REQUIRED COMPONENTS Interpreter Development)
# Install pybind11 via sudo apt inatll pybind11-dev
set(PYBIND11_PYTHON_VERSION 3.8)
find_package(pybind11 REQUIRED)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)
include_directories(src/Eigen-3.3)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    include_directories(/usr/local/opt/openssl/include)
    link_directories(/usr/local/opt/openssl/lib)
    link_directories(/usr/local/Cellar/libuv/1*/lib)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# pybind11_add_module(mpc ${source})

# Change cppad to 2022000 version
# in solve_callback.cpp add # define HAVE_CSTDDEF and # undef HAVE_CSTDDEF
# add_executable(mpc ${sources})
pybind11_add_module(mpc ${sources})
target_link_libraries(mpc PRIVATE pybind11::module cppad_lib ipopt z ssl uv) # uWS)