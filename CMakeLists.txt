cmake_minimum_required(VERSION 3.8)
project(_oop_endterm_FtpClient)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(_oop_endterm_FtpClient ${SOURCE_FILES})

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")