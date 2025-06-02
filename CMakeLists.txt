include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonFunctions.cmake)
yaku_lib(YakuEngine STATIC)
yaku_pch_exclude(YakuEngine "Libraries")