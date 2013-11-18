add_executable(FeatureExtractTest FeatureExtractTest.cpp)

TARGET_LINK_LIBRARIES(FeatureExtractTest ${ITK_LIBRARIES} FeatureExtractLib)

INCLUDE_DIRECTORIES(
   ../src
)

ADD_TEST(FeatureExtractTest_ImageIntegral ${EXECUTABLE_OUTPUT_PATH}/FeatureExtractTest ${CMAKE_CURRENT_SOURCE_DIR}/testcase/testcase ${CMAKE_CURRENT_SOURCE_DIR}/testcase/testImg.vtk ImageIntegral
  )

ADD_TEST(FeatureExtractTest_RandomBox ${EXECUTABLE_OUTPUT_PATH}/FeatureExtractTest ${CMAKE_CURRENT_SOURCE_DIR}/testcase/testcase ${CMAKE_CURRENT_SOURCE_DIR}/testcase/testImg.vtk RandomBox
  )

ADD_TEST(FeatureExtractTest_Pixel ${EXECUTABLE_OUTPUT_PATH}/FeatureExtractTest ${CMAKE_CURRENT_SOURCE_DIR}/testcase/testcase ${CMAKE_CURRENT_SOURCE_DIR}/testcase/testImg.vtk Pixel
  )

ADD_TEST(FeatureExtractTest_BoxIntegral ${EXECUTABLE_OUTPUT_PATH}/FeatureExtractTest ${CMAKE_CURRENT_SOURCE_DIR}/testcase/testcase ${CMAKE_CURRENT_SOURCE_DIR}/testcase/testImg.vtk BoxIntegral
  )

ADD_TEST(FeatureExtractTest_MRI ${EXECUTABLE_OUTPUT_PATH}/FeatureExtractTest ${CMAKE_CURRENT_SOURCE_DIR}/testcase/testcase ${CMAKE_CURRENT_SOURCE_DIR}/testcase/testImg.vtk MRI
  )

ADD_TEST(FeatureExtractTest_Spacing ${EXECUTABLE_OUTPUT_PATH}/FeatureExtractTest ${CMAKE_CURRENT_SOURCE_DIR}/testcase/testcase ${CMAKE_CURRENT_SOURCE_DIR}/testcase/testImg.vtk Spacing
  )

#SET_TESTS_PROPERTIES( FeatureExtractTest PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR;WARNING" )
