project (YakuEngine LANGUAGES CXX)

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

# Project Settings
include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonSettings.cmake)

