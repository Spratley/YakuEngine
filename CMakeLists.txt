# Create the static library
add_library(YakuEngine STATIC)

# Gather all code files
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.inl
)

# Generate filters for folders
foreach(source_file ${ENGINE_SOURCES})
	file(RELATIVE_PATH relative_path "${CMAKE_CURRENT_SOURCE_DIR}" "${source_file}")
	get_filename_component(source_path "${relative_path}" PATH)
	string(REPLACE "/" "\\" filter_path "${source_path}")
	source_group("${filter_path}" FILES "${source_file}")
endforeach()

target_sources(YakuEngine PRIVATE ${ENGINE_SOURCES})

# Set public include directory
target_include_directories(YakuEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Setup Precompiled Headers
set(PCH_HEADER "Yaku_PCH.h")
set(PCH_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/PCH/Yaku_PCH.cpp")
set(PCH_OUTPUT "${CMAKE_BINARY_DIR}/PCH/Yaku.pch")
set_source_files_properties(${PCH_SOURCE} PROPERTIES COMPILE_FLAGS "/Yc${PCH_HEADER} /Fp${PCH_OUTPUT}")
target_compile_options(YakuEngine PRIVATE "/YuPCH/${PCH_HEADER}" "/Fp${PCH_OUTPUT}")

# Disable PCH for external code
file(GLOB_RECURSE NO_PCH_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/*cpp)
foreach(file ${NO_PCH_SOURCES})
	set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "/Y-")
endforeach()

# Project Settings
include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonSettings.cmake)

