CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#The name of the project
PROJECT(liboptpp_example)

# DEL-BY-LEETEN 2015/06/23: find_package(mylib)

add_executable(${PROJECT_NAME}
	liboptpp_example.cpp)
	