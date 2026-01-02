include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonFunctions.cmake)
yaku_lib(CG STATIC)

yaku_pch_exclude(CG "CG/Libraries")

yaku_link(CG PUBLIC YakuCore)