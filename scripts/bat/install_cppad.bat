@echo off
setlocal

set CPPAD_DIR=%~dp0..\..\src\CppAD

if exist "%CPPAD_DIR%" (
    echo CppAD already exists. Skipping clone.
) else (
    echo Cloning CppAD...
    git clone https://github.com/coin-or/CppAD.git "%CPPAD_DIR%"
    cd "%CPPAD_DIR%"
    git checkout stable/2022
    cmake .
    cmake --build .
    cd ..
    if errorlevel 1 (
        echo ERROR: Failed to clone CppAD.
        exit /b 1
    )
)

echo Done.