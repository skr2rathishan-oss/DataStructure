@echo off
setlocal

set HTTPLIB_URL=https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h
set JSON_URL=https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
set POWERSHELL_EXE=%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe
set PWSH_EXE=%ProgramFiles%\PowerShell\7\pwsh.exe

echo Downloading cpp-httplib...
call :download "%HTTPLIB_URL%" "httplib.h"
if errorlevel 1 goto failed

echo Downloading nlohmann/json...
call :download "%JSON_URL%" "json.hpp"
if errorlevel 1 goto failed

if not exist httplib.h goto failed
if not exist json.hpp goto failed

echo Dependencies downloaded successfully.
pause
exit /b 0

:failed
echo Failed to download dependencies.
echo Check your internet connection, then run this script again.
pause
exit /b 1

:download
set URL=%~1
set OUT=%~2

if exist "%POWERSHELL_EXE%" (
    "%POWERSHELL_EXE%" -NoProfile -ExecutionPolicy Bypass -Command "[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -UseBasicParsing -Uri '%URL%' -OutFile '%OUT%'"
    exit /b %errorlevel%
)

if exist "%PWSH_EXE%" (
    "%PWSH_EXE%" -NoProfile -ExecutionPolicy Bypass -Command "Invoke-WebRequest -Uri '%URL%' -OutFile '%OUT%'"
    exit /b %errorlevel%
)

where curl >nul 2>nul
if not errorlevel 1 (
    curl -L "%URL%" -o "%OUT%"
    exit /b %errorlevel%
)

where certutil >nul 2>nul
if not errorlevel 1 (
    certutil -urlcache -split -f "%URL%" "%OUT%"
    exit /b %errorlevel%
)

echo No downloader found. Tried Windows PowerShell, PowerShell 7, curl, and certutil.
exit /b 1
