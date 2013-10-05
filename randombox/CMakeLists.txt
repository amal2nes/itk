cmake_minimum_required(VERSION 2.4)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()

project(RandomBox)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_VERBOSE_MAKEFILE ON)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

add_executable(RandomBox RandomBox.hxx RandomBox.h TestRandomBox.cxx)
