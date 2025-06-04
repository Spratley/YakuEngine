include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonFunctions.cmake)
yaku_lib(CG STATIC)
yaku_link(CG PUBLIC YakuEngine)