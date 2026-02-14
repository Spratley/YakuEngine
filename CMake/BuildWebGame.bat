@echo off
setlocal

:: Directory Values
set "SCRIPT_DIRECTORY=%~dp0"
:: Come back and rework how we're choosing the build directory.
:: It's not good to hardcode the assumption that we're building two folders up.
set "BUILD_ROOT_DIRECTORY=%SCRIPT_DIRECTORY%..\.."
set "BUILD_DIRECTORY=%BUILD_ROOT_DIRECTORY%\_WebBuild"
:: Also come back to this, EMSDK should be embedded in YakuEn, not dependant on the end-user's personal install
:: For now, since it's just my PC, hardcode the path to my copy
set "EMSDK_PATH=C:\Users\Offic\Documents\Repos\emsdk"

:: Script Constants
set BUILD_CONFIG_TYPE=Retail
set CMAKE_GENERATOR="Ninja"

:: Script Logic Begin
echo Initializing EMDSK Environment...
pushd %EMSDK_PATH%
    call emsdk_env.bat >nul 2>&1
    call emsdk activate latest ninja-git-release-64bit >nul 2>&1
:: pushd %EMSDK_PATH%
popd

if exist "%BUILD_DIRECTORY%" (
    echo Cleaning previous build artifacts...
    rd /s /q "%BUILD_DIRECTORY%"
)
mkdir "%BUILD_DIRECTORY%"

pushd "%BUILD_ROOT_DIRECTORY%"
    echo Starting WebAssembly Build...
    call emcmake cmake -S . -B "%BUILD_DIRECTORY%" -G "%CMAKE_GENERATOR%"
    if %ERRORLEVEL% neq 0 (echo Build generation failed! & pause & exit /b 1)
    
    pushd %BUILD_DIRECTORY%
        call ninja
        if %ERRORLEVEL% neq 0 (echo Building failed! & pause & exit /b 1)
        :: Come back and make this more robust. What if we're generating files that we don't want deleted?
        for %%i in (*) do (
            if not "%%~xi"==".js" (
                if not "%%~xi"==".wasm" (
                    if not "%%~xi"==".html" (
                        del /q "%%i"
                    )
                )
            )
        )
        rd /s /q %BUILD_DIRECTORY%\CMakeFiles
    :: pushd %BUILD_DIRECTORY%
    popd
:: pushd "%BUILD_ROOT_DIRECTORY%"
popd

:: Bat Finished Successfully
endlocal
pause
exit