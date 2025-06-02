function(yaku_pch TARGET_NAME PCH_NAME)
	set(PCH_HEADER "${PCH_NAME}.h")
	set(PCH_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/PCH/${PCH_NAME}.cpp")
	set(PCH_OUTPUT "${CMAKE_BINARY_DIR}/PCH/${PCH_NAME}.pch")
	set_source_files_properties(${PCH_SOURCE} PROPERTIES COMPILE_FLAGS "/Yc${PCH_HEADER} /Fp${PCH_OUTPUT}")
	target_compile_options(${TARGET_NAME} PRIVATE "/YuPCH/${PCH_HEADER}" "/Fp${PCH_OUTPUT}")
endfunction()

function(yaku_pch_exclude TARGET_NAME)
	foreach(EXCLUDE_FOLDER IN LISTS ARGN)
		# Normalize to absolute path
		get_filename_component(ABS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${EXCLUDE_FOLDER}" ABSOLUTE)

		if(EXISTS "${ABS_PATH}" AND IS_DIRECTORY "${ABS_PATH}")
			file(GLOB_RECURSE NO_PCH_SOURCES CONFIGURE_DEPENDS "${ABS_PATH}/*.cpp")
			foreach(EXCLUDE_FILE IN LISTS NO_PCH_SOURCES)
				set_source_files_properties(${EXCLUDE_FILE} PROPERTIES COMPILE_FLAGS "/Y-")
				message(STATUS "Disabling PCH for: ${EXCLUDE_FILE}")
			endforeach()
		else()
			message(WARNING "PCH exclusion skipped: Folder '${ABS_PATH}' does not exist.")
		endif()
	endforeach()
endfunction()


function(yaku_project_body TARGET_NAME)
	# Gather all code files
	file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
		${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/*.h
		${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/*.inl
	)

	# Generate project filters
	source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC_FILES})
	target_sources(${TARGET_NAME} PRIVATE ${SRC_FILES})
	
	# Setup PCH
	yaku_pch(${TARGET_NAME} ${TARGET_NAME}_PCH)
	
	#Project Settings
	include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonSettings.cmake)

endfunction()

function(yaku_executable TARGET_NAME EXE_NAME)
	add_executable(${TARGET_NAME})
	set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "${EXE_NAME}")
	yaku_project_body(${TARGET_NAME})
endfunction()

function(yaku_lib TARGET_NAME LIB_TYPE)
	add_library(${TARGET_NAME} ${LIB_TYPE})
	yaku_project_body(${TARGET_NAME})

	# Set public include directory
	target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endfunction()

function(yaku_link TARGET_NAME ACCESS_TYPE)
	foreach(LIBRARY IN LISTS ARGN)
		target_link_libraries(${TARGET_NAME} ${ACCESS_TYPE} ${LIBRARY})
	endforeach()
endfunction()