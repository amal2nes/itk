add_executable(RandomBoxPerformance RandomBoxPerformance.cpp)

TARGET_LINK_LIBRARIES(RandomBoxPerformance ${ITK_LIBRARIES} FeatureExtractLib)

INCLUDE_DIRECTORIES(
   ../src
)

ADD_TEST(RandomBoxPerformance
  ${EXECUTABLE_OUTPUT_PATH}/RandomBoxPerformance
  ${CMAKE_CURRENT_SOURCE_DIR}/data/100by100by100.vtk ${CMAKE_CURRENT_SOURCE_DIR}/result 5 10000 10000
  )


#SET_TESTS_PROPERTIES( RandomBoxPerformance PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR;WARNING" )
