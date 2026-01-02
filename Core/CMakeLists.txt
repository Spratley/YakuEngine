include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonFunctions.cmake)
yaku_lib(YakuCore STATIC)

yaku_pch_exclude(YakuCore "YKC/Libraries")

yaku_link_lib(YakuCore YKC/Libraries/OpenGL/GLFW/glfw3)
yaku_add_include_dir(YakuCore YKC/Libraries/OpenGL)