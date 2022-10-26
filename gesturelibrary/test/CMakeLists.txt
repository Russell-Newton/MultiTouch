include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

include_directories(${PROJECT_SOURCE_DIR}/test/include)

set(TARGET_TEST testGestureLibrary)
file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/src/*.cpp)

add_executable(${TARGET_TEST} ${TEST_SOURCES})

target_link_libraries(${TARGET_TEST} PUBLIC GTest::gtest_main ${TARGET_LIBRARY})

include(GoogleTest)
gtest_discover_tests(${TARGET_TEST})

add_custom_target(COPY_RESOURCE_FILES ALL
    ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/test/res/ ${PROJECT_BINARY_DIR}/test/res/
)

