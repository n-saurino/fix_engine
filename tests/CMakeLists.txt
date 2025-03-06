cmake_minimum_required(VERSION 3.10)

project(FIXMessagesTests)

# Add Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
# Include all source files from the src directory
set(SOURCE_FILES
  ../src/FIXEngine.cpp
  ../src/FIXNetworkHandler.cpp)

# Add the test source files
add_executable(test_FIXMessages test_FIXMessages.cpp ${SOURCE_FILES})
# Add the test source files

# Add header file includes
target_include_directories(test_FIXMessages PRIVATE ${Boost_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/generated/sbe")


# Link the test executable with Google Test
target_link_libraries(test_FIXMessages gtest_main)

include(GoogleTest)
gtest_discover_tests(test_FIXMessages)