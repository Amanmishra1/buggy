## Prerequisites:
Minimum CMake Version 3.10
C++17 is supported (or you can downgrade this project to C++11 by updating CMakeLists.txt)

## Assumptions
* If user presses 'Enter' before entering a word, it is ignored
* There is no size limit to the input length of each word

## Steps to build the project:
Install **cmake ** (details on the https://cmake.org/)
* cd 'project directory' (location where this project is present)
* mkdir build
* cd build
* cmake ..
* make

## Steps to run the code:
* cd build
* ./WordMatcher