@echo off
setlocal

cd ..
mkdir build
cd build

cmake ..
cmake --build . --config Release

