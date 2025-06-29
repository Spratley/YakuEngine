function(yaku_pch TARGET_NAME PCH_NAME)
	set(PCH_HEADER "${PCH_NAME}.h")
	set(PCH_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/PCH/${PCH_NAME}.cpp")
	set(PCH_OUTPUT "${CMAKE_BINARY_DIR}/PCH/${PCH_NAME}.pch")
	set_source_files_properties(${PCH_SOURCE} PROPERTIES COMPILE_FLAGS "/Yc${PCH_HEADER} /Fp${PCH_OUTPUT}")
	target_compile_options(${TARGET_NAME} PRIVATE "/YuPCH/${PCH_HEADER}" "/Fp${PCH_OUTPUT}")
	target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/PCH)
endfunction()

function(yaku_pch_exclude TARGET_NAME)
	foreach(EXCLUDE_FOLDER IN LISTS ARGN)
		# Normalize to absolute path
		get_filename_component(ABS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${EXCLUDE_FOLDER}" ABSOLUTE)

		if(EXISTS "${ABS_PATH}" AND IS_DIRECTORY "${ABS_PATH}")
			file(GLOB_RECURSE NO_PCH_SOURCES CONFIGURE_DEPENDS "${ABS_PATH}/*.cpp" "${ABS_PATH}/*.c")
			foreach(EXCLUDE_FILE IN LISTS NO_PCH_SOURCES)
				set_source_files_properties(${EXCLUDE_FILE} PROPERTIES COMPILE_FLAGS "/Y-")
				message(STATUS "Disabling PCH for: ${EXCLUDE_FILE}")
			endforeach()
		else()
			message(WARNING "PCH exclusion skipped: Folder '${ABS_PATH}' does not exist.")
		endif()
	endforeach()
endfunction()

function(yaku_set_output_dirs TARGET_NAME)
	set_target_properties(${TARGET_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	)
endfunction()

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

	# Generate project filters
	source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC_FILES})
	target_sources(${TARGET_NAME} PRIVATE ${SRC_FILES})
	
	# Setup PCH
	yaku_pch(${TARGET_NAME} ${TARGET_NAME}_PCH)
	
	#Project Settings
	include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonSettings.cmake)
	
	yaku_set_output_dirs(${TARGET_NAME})
endfunction()

function(yaku_executable TARGET_NAME EXE_NAME)
	set(CMAKE_CXX_STANDARD 20)
	add_executable(${TARGET_NAME})
	set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "${EXE_NAME}")
	yaku_project_body(${TARGET_NAME})
endfunction()

function(yaku_lib TARGET_NAME LIB_TYPE)
	set(CMAKE_CXX_STANDARD 20)
	add_library(${TARGET_NAME} ${LIB_TYPE})
	yaku_project_body(${TARGET_NAME})

	# Set public include directory
	target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endfunction()

function(yaku_link TARGET_NAME ACCESS_TYPE)
	target_link_libraries(${TARGET_NAME} ${ACCESS_TYPE} ${ARGN})
endfunction()

function(yaku_solution NAME)
	# Configuration settings
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Retail" CACHE STRING "" FORCE)
	# Shared project settings
	include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonSettings.cmake)
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