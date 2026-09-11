add_executable(infer_test hello_test.cpp)
target_link_libraries(infer_test glog::glog GTest::gtest_main)

add_executable(alloc_test test_alloc.cpp)
target_link_libraries(alloc_test infer glog::glog GTest::gtest_main)

add_executable(buffer_test test_buffer.cpp)
target_link_libraries(buffer_test infer glog::glog GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(infer_test)
gtest_discover_tests(alloc_test)
gtest_discover_tests(buffer_test)
