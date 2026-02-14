set "HOME_DIR=%CD%"
set OUTPUT_DIR=Build
set GENERATOR="Visual Studio 17 2022"

cd ../../

if not exist %OUTPUT_DIR% mkdir %OUTPUT_DIR%
cd %OUTPUT_DIR%

cmake -G %GENERATOR% ..\
cmake --build . --config Retail

timeout /t 1 >nul

echo Finished build! Cleaning up intermediate files...

ren "bin/Retail" "Build_Intermediate"
move "bin/Build_Intermediate" "../"
cd ../
rd /s /q "%OUTPUT_DIR%"
ren "Build_Intermediate" "%OUTPUT_DIR%"

cd /d "%HOME_DIR%"
call CleanupCMakeJunk.bat "%OUTPUT_DIR%"

echo Finished building!
pause