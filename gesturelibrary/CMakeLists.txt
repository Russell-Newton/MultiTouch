cmake_minimum_required(VERSION 3.14)

project(GestureLibrary VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/single)

set(TARGET_LIBRARY GestureLibrary)
file(GLOB_RECURSE LIBRARY_SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)

add_library(${TARGET_LIBRARY} SHARED ${LIBRARY_SOURCES})

set_target_properties(${TARGET_LIBRARY} PROPERTIES
                      POSITION_INDEPENDENT_CODE True
                      )


enable_testing()

set(TARGET_TEST testGestureLibrary)
file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)

add_executable(${TARGET_TEST} ${TEST_SOURCES})

target_link_libraries(${TARGET_TEST} PUBLIC GTest::gtest_main ${TARGET_LIBRARY})

include(GoogleTest)
gtest_discover_tests(${TARGET_TEST})

add_custom_target(COPY_RESOURCE_FILES ALL
    ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/test/res/ res/
)
