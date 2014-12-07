CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#The name of the project
PROJECT(liboptpp)

add_subdirectory(liboptpp)

include_directories(.)
add_subdirectory(example)
add_dependencies(liboptpp_example liboptpp_src)
