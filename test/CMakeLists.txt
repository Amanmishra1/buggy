cmake_minimum_required(VERSION 3.10)

set(SOURCES
  BasicRecognizerTest.cpp
)

add_executable(intent_test ${SOURCES})
target_link_libraries(intent_test PUBLIC
  gtest_main
  BasicRecognizer
)

add_test(
  NAME intent_test
  COMMAND intent_test
)
