cmake_minimum_required(VERSION 3.16)

project(FIXMessagesTests)

# Find Google Test
find_package(GTest REQUIRED)

enable_testing()

# Include all source files from the src directory
set(SOURCE_FILES
  ../src/FIXEngine.cpp
  ../src/FIXNetworkHandler.cpp)

# Add the test source files
add_executable(test_FIXMessages test_FIXMessages.cpp ${SOURCE_FILES})

# Add header file includes
target_include_directories(test_FIXMessages PRIVATE ${Boost_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/generated/sbe")

# Link the test executable with Google Test
target_link_libraries(test_FIXMessages PRIVATE GTest::gtest GTest::gtest_main pthread)

# Register tests
include(GoogleTest)
gtest_discover_tests(test_FIXMessages)

# Ensure the test executable is recognized by CTest
add_test(NAME FIXMessageTests COMMAND test_FIXMessages)