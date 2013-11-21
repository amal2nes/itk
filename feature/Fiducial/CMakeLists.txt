add_executable(FiducialTest FiducialTest.cpp)

TARGET_LINK_LIBRARIES(FiducialTest ${ITK_LIBRARIES} FeatureExtractLib)

INCLUDE_DIRECTORIES(
   ../src
)

ADD_TEST(FiducialTest ${EXECUTABLE_OUTPUT_PATH}/FiducialTest
  )