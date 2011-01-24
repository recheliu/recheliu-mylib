#	Required cache entries:
# 		FREEGLUT
#		THIRDPARTY_DIR
#		OPENCV_DIR
#		OPENCV_VER

cmake_minimum_required (VERSION 2.8)
project (mylib)

# The required packages
find_package(CUDA)
find_package(OpenGL)
find_package(GLUT)

# The required variables
# MOD-BY-LEETEN 01/23/2011-FROM:
	# set(THIRDPARTY_DIR "D:/lib" CACHE PATH "The path to the 3rd party libraries" FORCE)
	# set(OPENCV_DIR "C:/OpenCV2.1" CACHE PATH "The installed path of OpenCV" FORCE)
	# set(OPENCV_VER "210" CACHE STRING "The Version of OpenCV" FORCE)
	# set(FREEGLUT ON CACHE BOOL "Flag to decide whether FREEGLUT is used" FORCE)
# MOD-BY-LEETEN 01/23/2011-TO:
set(THIRDPARTY_DIR "D:/lib" CACHE PATH "The path to the 3rd party libraries")
set(OPENCV_DIR "C:/OpenCV2.1" CACHE PATH "The installed path of OpenCV")
set(OPENCV_VER "210" CACHE STRING "The Version of OpenCV")
set(FREEGLUT ON CACHE BOOL "Flag to decide whether FREEGLUT is used")
# MOD-BY-LEETEN 01/23/2011-END
# MOD-BY-LEETEN 01/23/2011-FROM:
	# set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})
# TO:
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
# MOD-BY-LEETEN 01/23/2011-END

# CUDA
set(CUDA_DIR "${CUDA_TOOLKIT_ROOT_DIR}")
set(CUDA_INC_DIR "${CUDA_DIR}/include")
set(CUDA_LIB_DIR "${CUDA_DIR}/lib")

# CUDA SDK
set(CUDASDK_DIR "${CUDA_SDK_ROOT_DIR}")
set(CUDASDK_INC_DIR "${CUDASDK_DIR}/inc")
set(CUDASDK_LIB_DIR "${CUDASDK_DIR}/lib")

# 3rd party lib
set(THIRDPARTY_INC_DIR "${THIRDPARTY_DIR}/include")
set(THIRDPARTY_LIB_DIR "${THIRDPARTY_DIR}/lib")

# OpenCV
set(OPENCV_INC_DIR "${OPENCV_DIR}/include")
set(OPENCV_LIB_DIR "${OPENCV_DIR}/lib")

# mylib
set(MYLIB_BIN_DIR "${PROJECT_BINARY_DIR}/bin")
set(MYLIB_INC_DIR "${PROJECT_BINARY_DIR}/include")
# MOD-BY-LEETEN 01/23/2011-FROM:
	# set(MYLIB_LIB_DIR_DEBUG "${PROJECT_BINARY_DIR}/lib/debug")
	# set(MYLIB_LIB_DIR_RELEASE "${PROJECT_BINARY_DIR}/lib/release")
# MOD-BY-LEETEN 01/23/2011-TO:
set(MYLIB_LIB_DIR "${PROJECT_BINARY_DIR}/lib")
set(MYLIB_LIB_DIR_DEBUG "${MYLIB_LIB_DIR}/debug")
set(MYLIB_LIB_DIR_RELEASE "${MYLIB_LIB_DIR}/release")
# MOD-BY-LEETEN 01/23/2011-END

# ADD-BY-LEETEN 01/23/2011-BEGIN
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")	
# ADD-BY-LEETEN 01/23/2011-END

# add the binary tree to the search path for include files
include_directories (
	"${CUDA_INC_DIR}"
	"${CUDASDK_INC_DIR}"
	"${OPENCV_INC_DIR}"
	"${THIRDPARTY_INC_DIR}"
	"${MYLIB_INC_DIR}"
	)

# add the binary tree to the search path for libraries
list(APPEND LIB_DIRS
	"${CUDA_LIB_DIR}"
	"${CUDASDK_LIB_DIR}"
	"${OPENCV_LIB_DIR}"
	"${THIRDPARTY_LIB_DIR}"
	"${MYLIB_LIB_DIR_DEBUG}"
	"${MYLIB_LIB_DIR_RELEASE}"
	)

link_directories(
	${LIB_DIRS}
)

set(CMAKE_LIBRARY_PATH ${LIB_DIRS})
find_library(FREEGLUT_LIB freeglut.lib)
if( FREEGLUT_LIB AND FREEGLUT )
	add_definitions(
		-DUSE_FREEGLUT
	)
endif()

add_definitions(
	-DOPENCV_VER=${OPENCV_VER}
	-DGLUT_BUILDING_LIB
)

##############################################################
if(DEFINED WIN32)
	set(COPY "copy")
else()
	set(COPY "cp")
endif()

function(build_library libname src_subdir dst_subdir with_shaders)
	set(SRC_DIR "${PROJECT_SOURCE_DIR}/${libname}/${src_subdir}/")
	
	AUX_SOURCE_DIRECTORY ("${SRC_DIR}" 	SRC_FILES)
	file(GLOB HEADER_FILES "${SRC_DIR}/*.h" )
	file(GLOB FRAG_FILES "${SRC_DIR}/*.frag" )
	file(GLOB VERT_FILES "${SRC_DIR}/*.vert" )
	file(GLOB GEOM_FILES "${SRC_DIR}/*.geom" )

	# clear the target
	list(LENGTH SRC_FILES N_SRC_FILES)
	if( N_SRC_FILES EQUAL 0 )
		add_custom_target (${libname} 
			# MOD-BY-LEETEN 01/23/2011-FROM:
				# SOURCES ${HEADER_FILES} ${FRAG_FILES} ${VERT_FILES})
			# TO:
			SOURCES ${HEADER_FILES} ${FRAG_FILES} ${VERT_FILES} ${GEOM_FILES}
			)
			# MOD-BY-LEETEN 01/23/2011-END
	else()
		add_library ( ${libname} 
			# MOD-BY-LEETEN 01/23/2011-FROM:
				# ${SRC_FILES} ${HEADER_FILES} ${FRAG_FILES} ${VERT_FILES}
			# TO:
			${SRC_FILES} ${HEADER_FILES} ${FRAG_FILES} ${VERT_FILES} ${GEOM_FILES}
			# MOD-BY-LEETEN 01/23/2011-END
			 )
		set_target_properties(${libname} PROPERTIES 
			DEBUG_OUTPUT_NAME "${libname}_d"
			RELEASE_OUTPUT_NAME "${libname}_r"
		)
	endif()

	# create the dst. folder
	set(DST_INC_DIR "${MYLIB_INC_DIR}/${dst_subdir}")
	add_custom_command (
		TARGET ${libname}
		PRE_BUILD
		COMMAND cmake -E make_directory ${DST_INC_DIR}
	)
	
	# copy all headers to the dst. file
	file(TO_NATIVE_PATH ${DST_INC_DIR} DST_INC_NATIVE_DIR)
	foreach(HEADER ${HEADER_FILES})
		file(TO_NATIVE_PATH ${HEADER} HEADER_NATIVE_PATH)
		
		add_custom_command (
			TARGET ${libname}
			PRE_BUILD
			COMMAND ${COPY} ${HEADER_NATIVE_PATH} ${DST_INC_NATIVE_DIR} )

		set_target_properties(${libname} PROPERTIES 
			PUBLIC_HEADER ${HEADER}
		)
	endforeach(HEADER)
	
	if( with_shaders )
		# convert all GLSL shaders to .h
		set(SHADER_FILES "")
		list(APPEND SHADER_FILES ${FRAG_FILES} ${VERT_FILES} ${GEOM_FILES} )
		foreach(SHADER ${SHADER_FILES})
			file(TO_NATIVE_PATH ${SHADER} SHADER_NATIVE_PATH)
			add_custom_command (
			  TARGET ${libname}
			  PRE_BUILD
			  COMMAND shader2string ${SHADER_NATIVE_PATH} ${SHADER_NATIVE_PATH}.h
			)
		endforeach(SHADER)
	endif()

	# copy_headers (${libname} ${src_subdir} ${dst_subdir})
endfunction (build_library)

# ADD-BY-LEETEN 01/23/2011-BEGIN
function(build_executable exename src_dir)
	AUX_SOURCE_DIRECTORY ("${src_dir}" 	SRC_FILES)
	file(GLOB HEADER_FILES "${src_dir}/*.h" )
	file(GLOB VERT_FILES "${src_dir}/*.vert" )
	file(GLOB FRAG_FILES "${src_dir}/*.frag" )
	file(GLOB GEOM_FILES "${src_dir}/*.geom" )
	
	add_executable ( ${exename} ${SRC_FILES} ${HEADER_FILES} ${VERT_FILES} ${FRAG_FILES} ${GEOM_FILES})

	# convert all GLSL shaders to .h
	set(SHADER_FILES "")
	list(APPEND SHADER_FILES ${FRAG_FILES} ${VERT_FILES} ${GEOM_FILES} )
	foreach(SHADER ${SHADER_FILES})
		file(TO_NATIVE_PATH ${SHADER} SHADER_NATIVE_PATH)
		add_custom_command (
		  TARGET ${exename}
		  PRE_BUILD
		  COMMAND shader2string ${SHADER_NATIVE_PATH} ${SHADER_NATIVE_PATH}.h
		)
	endforeach(SHADER)

	# copy_headers (${libname} ${src_subdir} ${dst_subdir})
endfunction (build_executable)

# ADD-BY-LEETEN 01/23/2011-END
 
##############################################################
# shader2string
# MOD-BY-LEETEN 01/23/2011-FROM:
	# add_executable (shader2string ${PROJECT_SOURCE_DIR}/libshader/shader2string/shader2string.cpp)
# TO:
build_executable(shader2string ${PROJECT_SOURCE_DIR}/libshader/shader2string)
# MOD-BY-LEETEN 01/23/2011-END
add_dependencies(shader2string libshader)
target_link_libraries (shader2string libshader)
target_link_libraries (shader2string "${THIRDPARTY_LIB_DIR}/glew32.lib")
# DEL-BY-LEETEN 01/23/2011-FROM:
	# install(
		# TARGETS shader2string 
		# DESTINATION ${MYLIB_BIN_DIR})
# DEL-BY-LEETEN 01/23/2011-END

##############################################################
foreach(LIB_NAME 
			libbuf 
			libclock 
			libcuda 
			liblog 
			libfbow
			libshader 
			libopt 
			libopengl 
			libfps 
			libtf2 
			libfbo
	)
	build_library(${LIB_NAME} "/" "/" false)
endforeach(LIB_NAME)

##############################################################
# libgw
build_library(libgw "libgw/" "/" false)
add_dependencies(libgw libbuf libopengl liblog libfps)

##############################################################
# libraries w/ their own subfolder under the folder include/
foreach(LIB_NAME 
			libdvr2 
	)
	build_library(${LIB_NAME} "/" "${LIB_NAME}/" false)
endforeach(LIB_NAME)

##############################################################
# libraries w/ their own subfolder under the folder include/
# and source codes in GLSL (.frag, .vert, and .geom)
foreach(LIB_NAME 
			libclip
			libdvr
			libtfw
	)
	build_library(${LIB_NAME} "/" "${LIB_NAME}/" true)
endforeach(LIB_NAME)

# MOD-BY-LEETEN 01/23/2011-FROM:
	# add_dependencies(libclip libfbow shader2string)
# TO:
add_dependencies(libclip libgw libfbow shader2string)
add_dependencies(libtfw libgw)
# MOD-BY-LEETEN 01/23/2011-END

add_dependencies(libdvr libfbo)
add_dependencies(libdvr2 libfbow)

##############################################################		 
# LIBGW testing program
# 
foreach(LIBGW_TESTER_NAME example_single_win example_single_win_global example_multi_wins example_multi_wins_global example_multi_wins_glui)
	# MOD-BY-LEETEN 01/23/2011-FROM:
		# add_executable (${LIBGW_TESTER_NAME} ${PROJECT_SOURCE_DIR}/libgw/examples/${LIBGW_TESTER_NAME}/${LIBGW_TESTER_NAME}.cpp)
	# TO:
	build_executable(${LIBGW_TESTER_NAME} ${PROJECT_SOURCE_DIR}/libgw/examples/${LIBGW_TESTER_NAME})
	# MOD-BY-LEETEN 01/23/2011-END
	# MOD-BY-LEETEN 01/23/2011-FROM:
		# add_dependencies(${LIBGW_TESTER_NAME} libgw liblog libbuf libfbow)
	# TO
	add_dependencies(${LIBGW_TESTER_NAME} libgw liblog libbuf libfbow libfps)
	# MOD-BY-LEETEN 01/23/2011-END
	target_link_libraries (${LIBGW_TESTER_NAME} libgw)
	add_test (${LIBGW_TESTER_NAME} ${LIBGW_TESTER_NAME})
endforeach(LIBGW_TESTER_NAME)

# ADD-BY-LEETEN 01/23/2011-BEGIN
build_executable(example_dvr ${PROJECT_SOURCE_DIR}/libdvr/example_dvr)
add_dependencies(example_dvr libgw libdvr)
target_link_libraries (example_dvr libgw libdvr libtfw libopt)
add_test (example_dvr example_dvr)

build_executable(example_dvr2 ${PROJECT_SOURCE_DIR}/libdvr2/example_dvr2)
add_dependencies(example_dvr2 libgw libdvr2)
target_link_libraries (example_dvr2 libgw libdvr2 libtfw libopt libclip libshader)
add_test (example_dvr2 example_dvr2)
# ADD-BY-LEETEN 01/23/2011-END
  
##############################################################
# create the header for the makefile
configure_file (
	"${PROJECT_SOURCE_DIR}/mylib.mak.in"
	"${PROJECT_BINARY_DIR}/mylib.mak"
)

# ADD-BY-LEETEN 01/24/2011-BEGIN
##############################################################
install(
	TARGETS shader2string 
	DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
install(
	DIRECTORY ${MYLIB_INC_DIR}
	DESTINATION "${CMAKE_INSTALL_PREFIX}")
install(
	DIRECTORY ${MYLIB_LIB_DIR}
	DESTINATION "${CMAKE_INSTALL_PREFIX}")
# ADD-BY-LEETEN 01/24/2011-END

#
# $Log: not supported by cvs2svn $
# Revision 1.2  2011/01/24 02:21:08  leeten
#
# [01/23/2011]
# 1. [ADD] Add the variable LIBRARY_OUTPUT_PATH.
# 2. [ADD] Make the LIBGW samples depend on LIBFPS.
# 
#