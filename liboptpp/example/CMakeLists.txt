CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#The name of the project
PROJECT(liboptpp_example)

find_package(mylib)

add_executable(${PROJECT_NAME}
	liboptpp_example.cpp)
	