if(USE_WIN32_SHMEM)
    set(SUM_IPC "Win32")
elseif(USE_MMAP_SHMEM)
    set(SUM_IPC "MMap")
elseif(USE_SYSTEMV_SHMEM)
    set(SUM_IPC "SystemV")
endif()

if(CMAKE_BUILD_TYPE STREQUAL Release)
    set(QF_CXX_FLAGS ${CMAKE_CXX_FLAGS_RELEASE})
    set(QF_C_FLAGS ${CMAKE_C_FLAGS_RELEASE})
elseif(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(QF_CXX_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})
    set(QF_C_FLAGS ${CMAKE_C_FLAGS_DEBUG})
elseif(CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
    set(QF_CXX_FLAGS ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
    set(QF_C_FLAGS ${CMAKE_C_FLAGS_RELWITHDEBINFO})
else()
    set(QF_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    set(QF_C_FLAGS ${CMAKE_C_FLAGS})
endif()

message("                                                                      ")
message("                  Configure summary:                                  ")
message("                                                                      ")
message("    install directory:         ${CMAKE_INSTALL_PREFIX}                ")
message("    compiler:                  ${CMAKE_CXX_COMPILER}                  ")
message("    build type:                ${CMAKE_BUILD_TYPE}                    ")
message("    compiler c flags:          ${QF_C_FLAGS}")
message("    compiler cxx flags:        ${QF_CXX_FLAGS}")
message("    version:                   ${CF_VERSION}                          ")
message("    build number:              ${CF_BUILD_NUMBER}                     ")
message("    gui:                       ${CF_GUI}                              ")
message("    icon theme mac:            ${WITH_THEME_MAC}                      ")
message("    icon theme faenza:         ${WITH_THEME_FAENZA}                   ")
message("    icon theme gnome:          ${WITH_THEME_GNOME}                    ")
message("    icon theme human:          ${WITH_THEME_HUMAN}                    ")
message("    icon theme snowish:        ${WITH_THEME_SNOWISH}                  ")
message("    IPC type:                  ${SUM_IPC}                             ")
message("    image lib Qt4:             ${CF_USE_IMAGE_LIBRARY_QT}             ")
message("    image lib ImageMagick:     ${CF_USE_IMAGE_LIBRARY_IMAGE_MAGICK}   ")
message("    image lib GraphicsMagick:  ${CF_USE_IMAGE_LIBRARY_GRAPHICS_MAGICK}")
message("    image lib FreeImage:       ${CF_USE_IMAGE_LIBRARY_FREEIMAGE}      ")
message("    PDF support:               ${CF_PDF_SUPPORT}                      ")
message("    Multi TIFF support:        ${CF_TIFF_SUPPORT}                     ")
message("    C API:                     ${CF_API}                              ")
message("    sane support:              ${CF_SANE_SCANNER_SUPPORT}             ")
message("    CppCheck:                  ${CPPCHECK_FOUND}                      ")
message("    Doxygen:                   ${DOXYGEN_FOUND}                       ")
message("    package vendor:            ${CPACK_PACKAGE_VENDOR}                ")
message("    package generator:         ${CPACK_GENERATOR}                     ")
message("                                                                      ")
