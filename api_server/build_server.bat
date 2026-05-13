@echo off
setlocal

if not exist httplib.h (
    echo Missing httplib.h. Run download_deps.bat first.
    exit /b 1
)

if not exist json.hpp (
    echo Missing json.hpp. Run download_deps.bat first.
    exit /b 1
)

echo Building SmartCloudStorage API server...
g++ server.cpp -std=c++17 -Wall -Wextra -I. -o server.exe -lws2_32

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build successful.
echo From the project root, run: .\api_server\server.exe
echo API URL: http://localhost:8080
exit /b 0
