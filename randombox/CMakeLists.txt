cmake_minimum_required(VERSION 2.4)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()

project(RandomBox)

# Find ITK.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(RandomBox RandomBox.hxx RandomBox.h TestRandomBox.cxx)

target_link_libraries(RandomBox ${ITK_LIBRARIES})
