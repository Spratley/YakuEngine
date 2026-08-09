include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonFunctions.cmake)
yaku_lib(YakuEngine STATIC)

yaku_pch_exclude(YakuEngine "EN/Libraries")

yaku_link(YakuEngine PUBLIC YakuCore)
yaku_link(YakuEngine PUBLIC CG)

yaku_include_path(YakuEngine PUBLIC "EN/Libraries/HIDra_UserConfig")