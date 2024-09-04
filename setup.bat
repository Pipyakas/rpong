@echo off
setlocal

REM Update and install dependencies
REM Note: Ensure you have Chocolatey installed (https://chocolatey.org/install)
choco install -y git cmake mingw

REM Set the working directory to %USERPROFILE%
cd /d %USERPROFILE%

REM Clone Raylib repository
git clone https://github.com/raysan5/raylib.git raylib

REM Build Raylib for native Windows
cd raylib\src
mingw32-make PLATFORM=PLATFORM_DESKTOP -j%NUMBER_OF_PROCESSORS%

REM Configure Git
git config --global user.name "Pipyakas"
git config --global user.email "pipyakas@gmail.com"

endlocal