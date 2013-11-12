add_executable(FeatureExtractTest FeatureExtractTest.cpp)

TARGET_LINK_LIBRARIES(FeatureExtractTest ${ITK_LIBRARIES} FeatureExtractLib)

ADD_TEST(FeatureExtractTest
  ${EXECUTABLE_OUTPUT_PATH}/FeatureExtractTest
  )
SET_TESTS_PROPERTIES( FeatureExtractTest PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR;WARNING" )
