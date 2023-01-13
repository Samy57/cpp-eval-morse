cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS OFF) 

project ( morse )

add_subdirectory( src )

include_directories( src ) 

add_executable(morse.exe main.cpp) 
target_link_libraries(morse.exe library)