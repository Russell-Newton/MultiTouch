cmake_minimum_required(VERSION 3.14)

project(GestureLibrary VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/single)

set(TARGET_LIBRARY GestureLibrary)
file(GLOB_RECURSE LIBRARY_SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)

add_library(${TARGET_LIBRARY} STATIC ${LIBRARY_SOURCES})

# set_target_properties(${TARGET_LIBRARY} PROPERTIES
#                       POSITION_INDEPENDENT_CODE True
#                       )

if (NOT SKIP_TESTS)
enable_testing()

add_subdirectory(test)
endif()
