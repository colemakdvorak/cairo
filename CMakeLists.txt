cmake_minimum_required(VERSION 3.14)
project(cairo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories for headers
include_directories(include)

# Define a proper library target (name: cairo_lib)
add_library(cairo_lib
    src/ShufflePrice.cpp
    src/ShuffleBar.cpp
)

# Add GoogleTest subdirectory
add_subdirectory(external/googletest)

# Define the test executable
add_executable(test_shuffle_price tests/test_shuffle_price.cpp)

# Link test with your project library and gtest
target_link_libraries(test_shuffle_price PRIVATE cairo_lib gtest gtest_main)

# Make sure tests can include project headers
target_include_directories(test_shuffle_price PRIVATE include)

# Enable testing
enable_testing()
add_test(NAME ShufflePriceTest COMMAND test_shuffle_price)
