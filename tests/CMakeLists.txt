cmake_minimum_required(VERSION 3.16)

project(ProjectTests)

# Find Google Test
find_package(GTest REQUIRED)

enable_testing()

# Include all source files from the src directory
set(SOURCE_FILES
  ../src/FIXEngine.cpp
  ../src/FIXNetworkHandler.cpp)

# Create separate test executables for each component
add_executable(test_FIXMessages test_FIXMessages.cpp ${SOURCE_FILES})
add_executable(test_MemoryPool test_MemoryPool.cpp ${SOURCE_FILES})
add_executable(test_LFQueue test_LFQueue.cpp ${SOURCE_FILES})

# Include directories for both test executables
target_include_directories(test_FIXMessages PRIVATE ${Boost_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/generated/sbe")
target_include_directories(test_MemoryPool PRIVATE ${Boost_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/generated/sbe")
target_include_directories(test_LFQueue PRIVATE ${Boost_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/generated/sbe")

# Link test executables with Google Test and pthread
target_link_libraries(test_FIXMessages PRIVATE GTest::gtest GTest::gtest_main pthread)
target_link_libraries(test_MemoryPool PRIVATE GTest::gtest GTest::gtest_main pthread)
target_link_libraries(test_LFQueue PRIVATE GTest::gtest GTest::gtest_main pthread)

# Register tests separately
include(GoogleTest)
gtest_discover_tests(test_FIXMessages)
gtest_discover_tests(test_MemoryPool)
gtest_discover_tests(test_LFQueue)

# Ensure the test executables are recognized by CTest
add_test(NAME FIXMessageTests COMMAND test_FIXMessages)
add_test(NAME MemoryPoolTests COMMAND test_MemoryPool)
add_test(NAME LFQueueTests COMMAND test_LFQueue)