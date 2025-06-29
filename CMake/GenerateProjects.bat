set "HOME_DIR=%CD%"
set OUTPUT_DIR=Generated
set GENERATOR="Visual Studio 17 2022"

cd ../../

if not exist %OUTPUT_DIR% mkdir %OUTPUT_DIR%
cd %OUTPUT_DIR%
cmake -G %GENERATOR% ..\

timeout /t 5 >nul

cd /d "%HOME_DIR%"
call CleanupCMakeJunk.bat "%OUTPUT_DIR%"

echo Finished generation!
pause