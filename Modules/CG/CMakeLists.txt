include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonFunctions.cmake)
yaku_lib(CG STATIC)

yaku_pch_exclude(CG "CG/Libraries")
yaku_link_lib(CG CG/Libraries/OpenGL/GLFW/glfw3)
yaku_add_include_dir(CG CG/Libraries/OpenGL)

yaku_link(CG PUBLIC YakuCore)