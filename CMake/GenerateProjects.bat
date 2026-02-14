@echo off
setlocal

:: Directory Values
set "SCRIPT_DIRECTORY=%~dp0"
:: Come back and rework how we're choosing the build directory.
:: It's not good to hardcode the assumption that we're building two folders up.
set "BUILD_ROOT_DIRECTORY=%SCRIPT_DIRECTORY%..\.."
set "BUILD_DIRECTORY=%BUILD_ROOT_DIRECTORY%\_Generated"

:: Script Constants
set CMAKE_GENERATOR="Visual Studio 17 2022"

:: Script Logic Begin
if exist "%BUILD_DIRECTORY%" (
    echo Cleaning previous build artifacts...
    rd /s /q "%BUILD_DIRECTORY%"
)
mkdir "%BUILD_DIRECTORY%"

pushd "%BUILD_ROOT_DIRECTORY%"
    echo Starting Build...
    cmake -S . -B "%BUILD_DIRECTORY%" -G %CMAKE_GENERATOR%
    if %ERRORLEVEL% neq 0 (echo Build generation failed! & pause & exit /b 1)
:: pushd %BUILD_ROOT_DIRECTORY%
popd

call CleanupCMakeJunk.bat

:: Bat Finished Successfully
endlocal
pause
exit