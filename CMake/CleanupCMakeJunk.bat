echo Cleaning up CMake junk.. If you want something done right you've got to do it yourself I guess

set OUTPUT_DIR=%~1

cd ../../

for /D %%D in (*) do (
	if /I not "%%D"=="%OUTPUT_DIR%" (
		if exist "%%D\cmake_install.cmake" (
			del "%%D\cmake_install.cmake"
		)
		if exist "%%D\CMakeFiles" (
			rmdir /s /q "%%D\CMakeFiles"
		)
	)
)