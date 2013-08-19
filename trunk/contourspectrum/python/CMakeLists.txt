# Modified from:	http://preney.ca/paul/archives/107

SET(MODULE_NAME	contourspectrum)
PROJECT(${MODULE_NAME}_py)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
IF(NOT CMAKE_BUILD_TYPE)
  # SET(CMAKE_BUILD_TYPE "DEBUG")
  SET(CMAKE_BUILD_TYPE "RELEASE")
  #SET(CMAKE_BUILD_TYPE "RELWITHDEBINFO")
  #SET(CMAKE_BUILD_TYPE "MINSIZEREL")
ENDIF()

FIND_PACKAGE(ThirdPartyLib)

set(PYTHON_LIBRARY		"C:/Python27/libs/python27.lib"	CACHE FILEPATH "The file path to the python library.")
set(PYTHON_INCLUDE_DIR	"C:/Python27/include"			CACHE PATH "The path to the headers of Python.")
FIND_PACKAGE(PythonLibs)
IF(PYTHONLIBS_FOUND)
  INCLUDE_DIRECTORIES("${PYTHON_INCLUDE_DIRS}")
ELSE()
  MESSAGE(FATAL_ERROR "Unable to find PythonLibs.")
ENDIF()

set(BOOST_ROOT			"~/boost"				CACHE 	PATH "The path of Boost.")
set(BOOST_INCLUDEDIR	"~/boost/include"		CACHE 	PATH "The path to the headers of Boost.")
set(BOOST_LIBRARYDIR	"~/boost/stage/lib"		CACHE 	PATH "The path to the libraries of Boost.")
set(BOOST_VERSION		"1.54"					CACHE	STRING "The version of Boost")

FIND_PACKAGE(Boost)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}")
  SET(Boost_USE_STATIC_LIBS 	OFF)
  SET(Boost_USE_MULTITHREADED	ON)
  SET(Boost_USE_STATIC_RUNTIME 	OFF)
  FIND_PACKAGE(Boost ${BOOST_VERSION} COMPONENTS python)

ELSEIF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "Unable to find correct Boost version. Did you set BOOST_ROOT?")
ENDIF()

ADD_LIBRARY(${PROJECT_NAME} MODULE  
	contourspectrum_py.cpp
	)

# How to set .dll to .pyd?
# Ref: http://www.cmake.org/pipermail/cmake/2006-January/007844.html
# Ref: http://www.mail-archive.com/cmake@cmake.org/msg14835.html
set_target_properties(${PROJECT_NAME}
	PROPERTIES 
	SUFFIX ".pyd")
	
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
