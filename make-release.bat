@echo off
set target_dir="cmake-build-release"
if not exist "%target_dir%" mkdir "%target_dir%"
cmake -B%target_dir%
cmake --build %target_dir% --config Release
if not exist "bin" mkdir "bin"
xcopy cmake-build-release\Release .\bin\Release /E/H/C/I
