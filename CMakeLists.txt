cmake_minimum_required(VERSION 3.10)

project(WordReader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

enable_testing()
add_subdirectory(googletest)

set(HEADERS
  include/buggy.h
)

set(SOURCES 
  src/buggy.cpp
)

add_library(WordMatcher ${HEADERS} ${SOURCES})

target_include_directories(WordMatcher
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(WordReader main.cpp)

target_link_libraries(WordReader
  WordMatcher
)

add_subdirectory(test)
