project("fswrappertest" CXX C)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(fused_gmock)

set(PRODUCT_SOURCE
        ${CMAKE_SOURCE_DIR}/src/usadamasa/fswrapper/file_finder.cpp
        ${CMAKE_SOURCE_DIR}/src/usadamasa/fswrapper/file_finder.h
        )

set(TEST_SRCS
        fused_gmock/gmock-gtest-all.cc
        test_main.cpp
        usadamasa/fswrapper/file_finder_test.cpp)

add_executable(fwrappertest
        ${TEST_SRCS}
        ${PRODUCT_SOURCE})

target_link_libraries(fwrappertest
        pthread
        dl
        boost_system
        boost_filesystem
        boost_regex
        )

add_test(
        NAME test
        COMMAND $<TARGET_FILE:fwrappertest> --gtest_output=xml:test_result.xml
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
