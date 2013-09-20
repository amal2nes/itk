# This is the root ITK CMakeLists file.
cmake_minimum_required(VERSION 2.4)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()


# This project is designed to be built outside the Insight source tree.
project(TestFilter)

# Find ITK.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(TestFilter TestFilter.cxx SummationImageFilter.h SummationImageFilter.hxx)

target_link_libraries(TestFilter ${ITK_LIBRARIES})
