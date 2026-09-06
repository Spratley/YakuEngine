# -=-=-= Required YakuEn Functions =-=-=-

# yaku_clang_copy
# Ensures the user has YakuEn's .clang-format in their project root

function(yaku_clang_copy)
	# Delete existing .clang-format 
	file(GLOB CLANG_FILES ${CMAKE_SOURCE_DIR}/.clang-format)
	file(REMOVE ${CMAKE_SOURCE_DIR}/.clang-format)

	# Copy .clang-format from YakuEngine to project root
	file(COPY ${CMAKE_SOURCE_DIR}/YakuEngine/.clang-format DESTINATION ${CMAKE_SOURCE_DIR})
endfunction()


# yaku_set_output_dirs
# Initializes the standard output directories
# TARGET_NAME	-> The project to modify

function(yaku_set_output_dirs TARGET_NAME)
	set_target_properties(${TARGET_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	)
endfunction()


# yaku_project_body
# Sets the C++ standard for the project (Which is almost immediately overwritten),
# then gathers all source files and adds them, conditionally marking platform-specific files as non-compilable, 
# and generates virtual filters that matches the folder layout.
# It then initializes the PCH and the output, and imports the project settings file
# TARGET_NAME	-> The project to modify

function(yaku_project_body TARGET_NAME)
	set(CMAKE_CXX_STANDARD 20)
	# Gather all code files
	file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
		${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/*.h
		${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/*.inl
		${CMAKE_CURRENT_SOURCE_DIR}/*.c
	)

	set(PLATFORM_TAG "")
	if (EMSCRIPTEN)
		set(PLATFORM_TAG "WASM")
	elseif(WIN32)
		set(PLATFORM_TAG "Windows")
	endif()

	# Conditionally mark files as non-compilable if they're platform dependant
	foreach(FILE IN LISTS SRC_FILES)
		set(SKIP_COMPILE FALSE)
		file(STRINGS "${FILE}" FIRST_LINE LIMIT_COUNT 1)
		
		# Check for the tag: // @platform: <Name>
		if(FIRST_LINE MATCHES "^//[ \t]*@platform:[ \t]*(.*)")
			set(FILE_PLATFORM "${CMAKE_MATCH_1}")
			string(STRIP "${FILE_PLATFORM}" FILE_PLATFORM)
			
			if(NOT FILE_PLATFORM STREQUAL PLATFORM_TAG)
				set(SKIP_COMPILE TRUE)
			endif()
		endif()

		if(SKIP_COMPILE)
			set_source_files_properties("${FILE}" PROPERTIES HEADER_FILE_ONLY TRUE)
		endif()
	endforeach()

	# Generate project filters
	source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC_FILES})
	target_sources(${TARGET_NAME} PRIVATE ${SRC_FILES})
	
	# Setup PCH
	yaku_pch(${TARGET_NAME} ${TARGET_NAME}_PCH)
	
	#Project Settings
	include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonSettings.cmake)
	
	yaku_set_output_dirs(${TARGET_NAME})
endfunction()

# -=-=-= Project Declarations =-=-=-

# yaku_executable
# Defines a target project as an executable
# TARGET_NAME	-> The project to create
# EXE_NAME	-> The name of the .exe to output

function(yaku_executable TARGET_NAME EXE_NAME)
	set(CMAKE_CXX_STANDARD 23)
	add_executable(${TARGET_NAME})
	set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "${EXE_NAME}")
	yaku_project_body(${TARGET_NAME})
endfunction()


# yaku_lib
# Defines a target project as a library
# TARGET_NAME	-> The project to create
# LIB_TYPE	-> The type of library to output

function(yaku_lib TARGET_NAME LIB_TYPE)
	set(CMAKE_CXX_STANDARD 23)
	add_library(${TARGET_NAME} ${LIB_TYPE})
	yaku_project_body(${TARGET_NAME})

	# Set public include directory
	target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endfunction()


# yaku_solution
# Defines a solution file for Visual Studio
# NAME	-> The name of the solution

function(yaku_solution NAME)
	set(CMAKE_SUPPRESS_REGENERATION ON)
	# Configuration settings
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Retail" CACHE STRING "" FORCE)
	# Shared project settings
	include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonSettings.cmake)
endfunction()

# -=-=-= Include Directories =-=-=-

# yaku_include_path
# Adds the specified include path to a project
# TARGET_NAME		-> The project to modify
# ACCESSOR		-> If the include path should be inherited when this project is incuded by another
# LOCAL_INCLUDE_PATH	-> The root folder the build system should search for includes in

function(yaku_include_path TARGET_NAME ACCESSOR LOCAL_INCLUDE_PATH)
	get_filename_component(ABS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_INCLUDE_PATH}" ABSOLUTE)
	target_include_directories(${TARGET_NAME} ${ACCESSOR} ${ABS_PATH})
	message(STATUS "${TARGET_NAME} - Adding custom include directory: ${ABS_PATH}")
endfunction()

# -=-=-= Precompiled Headers =-=-=-

# yaku_pch
# Sets the precompiled header configuration for a project
# Assumes your PCH lives in the a folder called /PCH/ in the root of the project
# TARGET_NAME	-> The project to modify
# PCH_NAME	-> The name of the precompiled header file to use

function(yaku_pch TARGET_NAME PCH_NAME)
	set(PCH_HEADER "${PCH_NAME}.h")
	set(PCH_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/PCH/${PCH_NAME}.cpp")
	set(PCH_OUTPUT "${CMAKE_BINARY_DIR}/PCH/${PCH_NAME}.pch")
	if (MSVC)
		# Disable precompiled headers for WebAssembly builds, since it's just straight building the content anyway right?
		set_source_files_properties(${PCH_SOURCE} PROPERTIES COMPILE_FLAGS "/Yc${PCH_HEADER} /Fp${PCH_OUTPUT}")
		target_compile_options(${TARGET_NAME} PRIVATE "/YuPCH/${PCH_HEADER}" "/Fp${PCH_OUTPUT}")
	endif()
	#target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/PCH)
endfunction()


# yaku_pch_exclude
# Sets a folder(s) to be excluded from use with the project's PCH
# TARGET_NAME	-> The project to modify
# ARGS		-> A list of folder paths relative to the project's root to exclude

function(yaku_pch_exclude TARGET_NAME)
	if (MSVC) # Disable PCH for WebAssembly builds
	foreach(EXCLUDE_FOLDER IN LISTS ARGN)
		# Normalize to absolute path
		get_filename_component(ABS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${EXCLUDE_FOLDER}" ABSOLUTE)

		if(EXISTS "${ABS_PATH}" AND IS_DIRECTORY "${ABS_PATH}")
			file(GLOB_RECURSE NO_PCH_SOURCES CONFIGURE_DEPENDS "${ABS_PATH}/*.cpp" "${ABS_PATH}/*.c")
			foreach(EXCLUDE_FILE IN LISTS NO_PCH_SOURCES)
				set_source_files_properties(${EXCLUDE_FILE} PROPERTIES COMPILE_FLAGS "/Y-")
			endforeach()
		else()
			message(WARNING "PCH exclusion skipped: Folder '${ABS_PATH}' does not exist.")
		endif()
	endforeach()
	endif()
endfunction()








# -=-=-= UNSORTED =-=-=-



function(yaku_link TARGET_NAME ACCESS_TYPE)
	target_link_libraries(${TARGET_NAME} ${ACCESS_TYPE} ${ARGN})
endfunction()

function(yaku_subproject_dir DIRECTORY)
	add_subdirectory(${DIRECTORY} ${CMAKE_SOURCE_DIR}/${DIRECTORY})
endfunction()

function(yaku_subproject NAME)
	yaku_subproject_dir(${NAME} ${NAME})
endfunction()

function(yaku_startup PROJECT_NAME)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endfunction()

function(apply_configuration_settings TARGET_NAME)
	if(MSVC)
		# Set SubSystem
		foreach(CONFIG_NAME DEBUG RELEASE RETAIL)
			if(CONFIG_NAME STREQUAL "RETAIL")
				set_target_properties(${TARGET_NAME} PROPERTIES
					LINK_FLAGS_${CONFIG_NAME} "/SUBSYSTEM:WINDOWS"
				)
			else()
				set_target_properties(${TARGET_NAME} PROPERTIES
					LINK_FLAGS_${CONFIG_NAME} "/SUBSYSTEM:CONSOLE"
				)
			endif()
		endforeach()

		# Set Debug and Optimizations for Retail
		target_compile_definitions(${TARGET_NAME} PRIVATE
			$<$<CONFIG:Retail>:YAKU_RETAIL;NDEBUG>
		)
		target_compile_options(${TARGET_NAME} PRIVATE
			$<$<CONFIG:Retail>:/O2 /Ob2 /MD>
		)
		target_link_options(${TARGET_NAME} PRIVATE
			$<$<CONFIG:Retail>:/INCREMENTAL:NO /DEBUG:NONE>
		)

		# Set Debug options for Release
		target_compile_options(${TARGET_NAME} PRIVATE
			$<$<CONFIG:Release>:/Zi>
		)
		target_link_options(${TARGET_NAME} PRIVATE
			$<$<CONFIG:Release>:/DEBUG>
		)
	endif()
endfunction()

function(yaku_config_projects)
	foreach(PROJECT_NAME IN LISTS ARGN)
		apply_configuration_settings(${PROJECT_NAME})
	endforeach()
endfunction()

function(yaku_link_lib TARGET_NAME LIB_PATH)
	target_link_libraries(${TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/${LIB_PATH}.lib")
endfunction()

function(yaku_add_include_dir TARGET_NAME INCLUDE_PATH)
	target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH})
endfunction()

function(yaku_post_generation)
endfunction()