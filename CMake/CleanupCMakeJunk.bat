@echo off
setlocal enabledelayedexpansion

echo Cleaning up CMake junk.. If you want something done right you've got to do it yourself I guess

set "HOME_DIR=%CD%"
cd ../../

if "%~1"=="" (
	set OUTPUT_DIR=Generated
) else (
	set OUTPUT_DIR=%~1
)

call :CleanDirectories "%CD%"

cd "%HOME_DIR%"
goto :eof

:CleanDirectories
for /D %%D in ("%~1\*") do (
	set "foldername=%%~nxD"
	call :CheckFolderName

	if "!SKIP_FOLDER!"=="0" (
		if exist "%%~fD\cmake_install.cmake" (
			echo Deleting "%%~fD\cmake_install.cmake"
			del /q "%%~fD\cmake_install.cmake"
		)
		if exist "%%~fD\CMakeFiles" (
			echo Deleting Directory "%%~fD\CMakeFiles"
			rmdir /s /q "%%~fD\CMakeFiles"
		)
		call :CleanDirectories "%%~fD"
	)
)

goto :eof

:CheckFolderName
setlocal enabledelayedexpansion
set "SKIP_FOLDER=0"
if /I "!foldername!"=="%OUTPUT_DIR%" (
	endlocal & set "SKIP_FOLDER=1"
) else (
	endlocal & set "SKIP_FOLDER=0"
)
goto :eof