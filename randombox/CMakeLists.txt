cmake_minimum_required(VERSION 2.4)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()

project(RandomBox)

# Find ITK.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(RandomBox RandomBox.hxx RandomBox.h TestRandomBox.cxx)

target_link_libraries(RandomBox ${ITK_LIBRARIES})
