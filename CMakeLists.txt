cmake_minimum_required(VERSION 3.16)
project(main VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Debug)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)  # Use C++20, you can change this to 11, 14, 20, etc.
set(CMAKE_CXX_STANDARD_REQUIRED True)  # Ensure the compiler enforces the standard
if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
endif()
set(CMAKE_CXX_EXTENSIONS OFF)  # Disable compiler-specific extensions

# Explicitly enable exceptions for GCC/Clang
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
endif()

# Find Boost
set(BOOST_ROOT "/opt/homebrew/Cellar/boost/1.85.0")
find_package(Boost 1.85.0 REQUIRED COMPONENTS system filesystem)

if(Boost_FOUND)
    message(STATUS "Boost version: ${Boost_VERSION}")
    message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost library dirs: ${Boost_LIBRARY_DIRS}")
else()
    message(FATAL_ERROR "Could not find Boost")
endif()

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(main ${SOURCES})

# Include Boost headers
target_include_directories(main PRIVATE ${Boost_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/generated/sbe")

file(GLOB SBE_GENERATED ${CMAKE_SOURCE_DIR}/generated/sbe/*.h)

add_library(sbe_generated INTERFACE) 

# Link Boost libraries
target_link_libraries(main PRIVATE ${Boost_LIBRARIES} GTest::GTest GTest::Main pthread sbe_generated)

# Add this at the end of your CMakeLists.txt
# enable_testing()
# add_subdirectory(tests)

# add_subdirectory(benchmarks)
