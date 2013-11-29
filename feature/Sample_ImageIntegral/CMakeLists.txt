# This is the root ITK CMakeLists file.
cmake_minimum_required(VERSION 2.4)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()

# This project is designed to be built outside the Insight source tree.
project(FeatureExtract)

# Find ITK.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(SampleImageIntegral SampleImageIntegral.cpp)

SET(FeatureExtract_SRCS 
	../src/FeatureExtractSum.cpp ../src/FeatureExtractRandomBox.cpp
)

ADD_LIBRARY(FeatureExtractLib STATIC ${FeatureExtract_SRCS})

TARGET_LINK_LIBRARIES(SampleImageIntegral ${ITK_LIBRARIES} FeatureExtractLib)

INCLUDE_DIRECTORIES(
   ../src
)
