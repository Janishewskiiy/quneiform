set(CF_BUILD_NUMBER -1)
find_program(BZR_CMD NAMES bzr)
execute_process(
    COMMAND ${BZR_CMD} revno ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE CF_BZR_RESULT
    OUTPUT_VARIABLE CF_BUILD_NUMBER
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Build number is: ${CF_BUILD_NUMBER}")

configure_file(${CMAKE_SOURCE_DIR}/src/include/build_number.h.in ${CMAKE_BINARY_DIR}/src/include/build_number.h)