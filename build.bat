@echo off
echo Building Grim Tithe...

cd /d "%~dp0"

.\external\premake\premake5.exe vs2022

pause