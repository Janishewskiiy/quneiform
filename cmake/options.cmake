if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    option(TESTS "Build tests" ON)
    option(OCRTK "Build OCRTk" ON)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    option(TESTS "Build tests" OFF)
    option(OCRTK "Build OCRTk" OFF)
endif()

option(API "Build cuneiform language bindings" ON)
if(API)
    set(CF_API True)
endif()

option(WITH_THEME_MAC "Build icon theme mac" OFF)
option(WITH_THEME_GNOME "Build icon theme gnome" OFF)
option(WITH_THEME_HUMAN "Build icon theme human" OFF)
option(WITH_THEME_FAENZA "Build icon theme faenza" OFF)
option(WITH_THEME_SNOWISH "Build icon theme snowish" OFF)
option(WITH_COVERAGE "Build with coverage analysis support" OFF)

option(WITH_SANE "Build with SANE scanner support" ON)

option(FOR_MACOSX "Add MacOSX specific flags" OFF)
