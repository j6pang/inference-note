add_executable(infer_test hello_test.cpp)
target_link_libraries(infer_test glog::glog GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(infer_test)
