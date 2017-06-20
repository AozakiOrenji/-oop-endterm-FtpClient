cmake_minimum_required(VERSION 3.8)

project(_oop_endterm_FtpClient)                                     #Define the name of the project

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)              #let CMake search header files here

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src VAR_USERPROJECT) #let CMake search source files here

add_executable(_oop_endterm_FtpClient ${VAR_USERPROJECT})

#set(SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/src/index.cpp)

#add_executable(_oop_endterm_FtpClient ${SOURCE_FILES})

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")