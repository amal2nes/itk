add_executable(RandomBoxPerformance RandomBoxPerformance.cpp)

TARGET_LINK_LIBRARIES(RandomBoxPerformance ${ITK_LIBRARIES} FeatureExtractLib)

INCLUDE_DIRECTORIES(
   ../src
)

ADD_TEST(RandomBoxPerformance
  ${EXECUTABLE_OUTPUT_PATH}/RandomBoxPerformance
  )
SET_TESTS_PROPERTIES( RandomBoxPerformance PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR;WARNING" )
