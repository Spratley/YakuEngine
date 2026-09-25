include(${CMAKE_SOURCE_DIR}/YakuEngine/CMake/CommonFunctions.cmake)
yaku_lib(AniMagic STATIC)

yaku_link(AniMagic PUBLIC CG)
yaku_link(AniMagic PUBLIC YakuCore)