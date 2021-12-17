include(googletest)

file(GLOB TEST_CC "${PROJECT_SOURCE_DIR}/test/*_test.cc")

add_executable(tester test/tester.cc ${TEST_CC})
target_link_libraries(tester rdc gtest gmock)

enable_testing()
