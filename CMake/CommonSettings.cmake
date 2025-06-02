# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use Folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set output folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Debug Output
set(CMAKE_DEBUG_POSTFIX "_Debug")

# MSVC Flags
if(MSVC)
	add_compile_options(/W4 /WX /MP /permissive-)
	add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS)
endif()

# Configuration settings
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