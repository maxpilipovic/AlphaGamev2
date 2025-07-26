@echo off
pushd %~dp0\..\

echo Generating Visual Studio 2022 project files...
premake5.exe vs2022

if %errorlevel% neq 0 (
    echo Error: Failed to generate project files
    popd
    exit /b %errorlevel%
)

echo.
echo Project files generated successfully!
echo You can now open GameProject.sln in Visual Studio 2022
echo.

popd
pause