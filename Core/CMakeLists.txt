include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonFunctions.cmake)
yaku_lib(YakuCore STATIC)

yaku_pch_exclude(YakuCore "YK/Libraries")

if (NOT EMSCRIPTEN)
    yaku_link_lib(YakuCore YK/Libraries/OpenGL/GLFW/glfw3)
    yaku_add_include_dir(YakuCore YK/Libraries/OpenGL)
endif()