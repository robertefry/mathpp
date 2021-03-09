
cmake_minimum_required(VERSION 3.0)

# Define project environment
project("Math Plus Plus" C CXX)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
enable_testing()

# Define code standards
set(CMAKE_C_STANDARD   "99")
set(CMAKE_CXX_STANDARD "20")

# Define compiler/linker flags
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
    ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"
)
    set ("CMAKE_CXX_FLAGS" "-Wall -Wextra -Wpedantic -std=c++20 -ggdb -pthread")
    set ("CMAKE_C_FLAGS"   "-Wall -Wextra -Wpedantic -std=c++20 -ggdb -pthread")
endif()

# Include project directories
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Glob project sources
file(GLOB_RECURSE PROJECT_SOURCES
    "src/*.h" "src/*.hh" "src/*.hpp" "src/*.hxx"
    "src/*.c" "src/*.cc" "src/*.cpp" "src/*.cxx"
)

# Glob project test sources
file(GLOB_RECURSE PROJECT_TEST_SOURCES
    "test/*.h" "test/*.hh" "test/*.hpp" "test/*.hxx"
    "test/*.c" "test/*.cc" "test/*.cpp" "test/*.cxx"
)

# Include googletest
add_subdirectory(googletest)

# Add test executable
set(PROJECT_TESTS_NAME "mathpp_tests")
include_directories(googletest/googletest/include)
include_directories(googletest/googlemock/include)
add_executable(${PROJECT_TESTS_NAME} ${PROJECT_TEST_SOURCES})
target_link_libraries(${PROJECT_TESTS_NAME} PUBLIC gtest_main gmock_main)
add_test(NAME ${PROJECT_TESTS_NAME} COMMAND ${PROJECT_TESTS_NAME})

# Run test executable after successfully building
add_custom_command(TARGET ${PROJECT_TESTS_NAME} POST_BUILD COMMAND ${PROJECT_TESTS_NAME})
