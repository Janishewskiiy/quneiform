add_definitions(-DTEST_IMG_PATH="${CMAKE_SOURCE_DIR}/images/quneiform")
add_definitions(-DTEST_EXE_PATH="${EXECUTABLE_OUTPUT_PATH}")

set(TEST_PUMA_SRC
    test_puma.cpp
    testlocalrecognitionserver.cpp
    testpuma.cpp
    teststartprocess.cpp
)

if(CF_USE_SHMEM)
    list(APPEND TEST_PUMA_SRC testprocessrecognitionserver.cpp)
endif()

add_executable(test_puma ${TEST_PUMA_SRC})
add_executable(test_puma_crash crash_process.cpp)

add_dependencies(test_puma test_puma_crash)

target_link_libraries(test_puma cuneiform ${CPPUNIT_LIBRARIES})

add_test(Puma ${EXECUTABLE_OUTPUT_PATH}/test_puma)
