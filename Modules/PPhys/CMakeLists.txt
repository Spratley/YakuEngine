include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonFunctions.cmake)
yaku_lib(PPhys STATIC)

yaku_pch_exclude(PPhys "PP/Libraries")

yaku_link(PPhys PUBLIC YakuCore)

yaku_include_path(PPhys PUBLIC "PP/Libraries")