@echo off
setlocal

set PYBIND11_DIR=%~dp0..\..\src\pybind11

if exist "%PYBIND11_DIR%" (
    echo pybind11 already exists. Skipping clone.
) else (
    echo Cloning pybind11...
    git clone https://github.com/pybind/pybind11.git "%PYBIND11_DIR%"
    cd ..
    if errorlevel 1 (
        echo ERROR: Failed to clone pybind11.
        exit /b 1
    )
)

echo Done.