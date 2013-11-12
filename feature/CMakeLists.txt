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

# Directories to search for #include (add more if needed) 
# (ITK is already included through the use of INCLUDE(${ITK_USE_FILE})
INCLUDE_DIRECTORIES(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_BINARY_DIR}
)

SET(FeatureExtract_SRCS 
	FeatureExtractSum.cpp FeatureExtractRandomBox.cpp
)

ADD_LIBRARY(FeatureExtractLib STATIC ${FeatureExtract_SRCS})

target_link_libraries(FeatureExtractLib ${ITK_LIBRARIES})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} )

ENABLE_TESTING()

INCLUDE(CTest)

IF (BUILD_TESTING)
	ADD_SUBDIRECTORY (Test)
ENDIF (BUILD_TESTING)
