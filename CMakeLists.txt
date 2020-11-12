cmake_minimum_required(VERSION 3.10)

project(WordReader)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

enable_testing()
add_subdirectory(googletest)
find_package (Threads)

set(HEADERS
  include/buggy.h
  include/word.h
)

set(SOURCES 
  src/buggy.cpp
)

add_compile_options(
  "-Wall" "-Wpedantic" "-Wextra" "-fexceptions"
  "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
)

add_executable(WordMatcher ${HEADERS} ${SOURCES})

target_include_directories(WordMatcher
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries (WordMatcher ${CMAKE_THREAD_LIBS_INIT})

#add_subdirectory(test)
