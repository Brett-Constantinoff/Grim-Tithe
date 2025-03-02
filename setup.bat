@echo off
echo Setting up Grim Tithe...

cd /d "%~dp0"

python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Python is not installed! Please install Python and ensure it is added to the system PATH.
    pause
    exit /b
)

echo Running Vulkan SDK setup...
python .\scripts\installVulkan.py

pause
