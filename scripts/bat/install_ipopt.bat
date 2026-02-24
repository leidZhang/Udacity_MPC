@echo off
setlocal

set "EXIT_ON_ERROR=if errorlevel 1 exit /b 1"

REM == Config verion ==
set "REPO_URL=https://github.com/coin-or/Ipopt/releases/download/releases/3.14.15/Ipopt-3.14.15-win64-msvs2019-md.zip"
set INSTALL_DIR=%~dp0..\..\..\Ipopt

for %%I in ("%~dp0..\..\..\Ipopt") do set "INSTALL_DIR=%%~fI"
set "IPOPT_BIN=%INSTALL_DIR%\bin"
set "IPOPT_DIR=%INSTALL_DIR%"

echo Installing Ipopt %IPOPT_VERSION%...

REM == Create directory ==
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"

REM == Install and decompress Zip ==
set ZIPFILE=%INSTALL_DIR%\ipopt_win64.zip
if exist "%ZIPFILE%" (
    echo Zip already exists. Skipping download.
) else (
    echo Downloading Ipopt binaries...
    curl --ssl-no-revoke -L -o "%ZIPFILE%" "%REPO_URL%"
)

REM == Extract ==
echo Extracting...
set "TEMP_DIR=%INSTALL_DIR%\_tmp"
powershell -Command "Expand-Archive '%ZIPFILE%' '%TEMP_DIR%' -Force"
for /d %%D in ("%TEMP_DIR%\*") do (
    xcopy "%%D\*" "%INSTALL_DIR%\" /E /I /Y
)
rmdir /s /q "%TEMP_DIR%"


REM == Add PATH to the environment variable ==
echo Adding Ipopt to PATH...
powershell -NoProfile -Command ^
"$bin='%IPOPT_BIN%'; ^
$old=[Environment]::GetEnvironmentVariable('Path','User'); ^
if($old -notlike '*'+$bin+'*'){ ^
  [Environment]::SetEnvironmentVariable('Path',$old+';'+$bin,'User') ^
}"

echo Setting '%IPOPT_DIR%' to environment variable...
setx IPOPT_DIR "%IPOPT_DIR%"

echo Done. Please reopen your terminal to start using ipopt.
pause