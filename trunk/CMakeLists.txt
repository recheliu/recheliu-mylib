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
# MOD-BY-LEETEN 01/24/2011-FROM:
	# set(FREEGLUT ON CACHE BOOL "Flag to decide whether FREEGLUT is used")
# TO:
set(FREEGLUT OFF CACHE BOOL "Flag to decide whether FREEGLUT is used")
# MOD-BY-LEETEN 01/24/2011-END
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

# MOD-BY-LEETEN 01/24/2011-FROM:
	# function(build_library libname src_subdir dst_subdir with_shaders)
# TO:
function(build_library libname src_subdir dst_subdir)
# MOD-BY-LEETEN 01/24/2011-END
	set(SRC_DIR "${PROJECT_SOURCE_DIR}/${libname}/${src_subdir}/")
	
	AUX_SOURCE_DIRECTORY ("${SRC_DIR}" 	SRC_FILES)
	file(GLOB HEADER_FILES "${SRC_DIR}/*.h" )
	file(GLOB FRAG_FILES "${SRC_DIR}/*.frag" )
	file(GLOB VERT_FILES "${SRC_DIR}/*.vert" )
	file(GLOB GEOM_FILES "${SRC_DIR}/*.geom" )

	list(LENGTH SRC_FILES N_SRC_FILES)
	if( N_SRC_FILES EQUAL 0 )
		# MOD-BY-LEETEN 01/24/2011-FROM:
			# add_custom_target (${libname} 
				# # MOD-BY-LEETEN 01/23/2011-FROM:
					# # SOURCES ${HEADER_FILES} ${FRAG_FILES} ${VERT_FILES})
				# # TO:
				# # TEST-MOD-FROM:
					# # SOURCES ${HEADER_FILES} ${FRAG_FILES} ${VERT_FILES} ${GEOM_FILES}
				# # TO:
				# SOURCES ${HEADER_FILES}
				# # TEST-MOD-END
				# )
				# # MOD-BY-LEETEN 01/23/2011-END
		# MOD-BY-LEETEN 01/24/2011-TO:
		set(HEADER_PROJ_NAME ${libname})
		# MOD-BY-LEETEN 01/24/2011-END
	else()
		add_library ( ${libname} 
			# MOD-BY-LEETEN 01/23/2011-FROM:
				# ${SRC_FILES} ${HEADER_FILES} ${FRAG_FILES} ${VERT_FILES}
			# TO:
			# MOD-BY-LEETEN 01/24/2011-FROM:
				# ${SRC_FILES} ${HEADER_FILES} ${FRAG_FILES} ${VERT_FILES} ${GEOM_FILES}
			# TO:
			${SRC_FILES}
			# MOD-BY-LEETEN 01/24/2011-END
			# MOD-BY-LEETEN 01/23/2011-END
			 )
		set_target_properties(${libname} PROPERTIES 
			DEBUG_OUTPUT_NAME "${libname}_d"
			RELEASE_OUTPUT_NAME "${libname}_r"
		)
		set(HEADER_PROJ_NAME ${libname}_h)
	endif()

	# MOD-BY-LEETEN 01/24/2011-FROM:
		# # create the dst. folder
		# set(DST_INC_DIR "${MYLIB_INC_DIR}/${dst_subdir}")
		# add_custom_command (
			# TARGET ${libname}
			# PRE_BUILD
			# COMMAND cmake -E make_directory ${DST_INC_DIR}
		# )

		# # copy all headers to the dst. file
		# file(TO_NATIVE_PATH ${DST_INC_DIR} DST_INC_NATIVE_DIR)
		# foreach(HEADER ${HEADER_FILES})
			# file(TO_NATIVE_PATH ${HEADER} HEADER_NATIVE_PATH)
			
			# add_custom_command (
				# TARGET ${libname}
				# PRE_BUILD
				# COMMAND ${COPY} ${HEADER_NATIVE_PATH} ${DST_INC_NATIVE_DIR} )

			# set_target_properties(${libname} PROPERTIES 
				# PUBLIC_HEADER ${HEADER}
			# )
		# endforeach(HEADER)
	# MOD-BY-LEETEN 01/24/2011-TO:
	add_custom_target (${HEADER_PROJ_NAME} 
		SOURCES ${HEADER_FILES}
		)
		
	set(DST_INC_DIR "${MYLIB_INC_DIR}/${dst_subdir}")
	file(TO_NATIVE_PATH ${DST_INC_DIR} DST_INC_NATIVE_DIR)

	add_custom_command (
		TARGET ${HEADER_PROJ_NAME}
		POST_BUILD
		COMMAND cmake -E make_directory ${DST_INC_DIR} )
		
	foreach(HEADER ${HEADER_FILES})
		file(TO_NATIVE_PATH ${HEADER} HEADER_NATIVE_PATH)
		add_custom_command (
		  TARGET ${HEADER_PROJ_NAME}
		  POST_BUILD
		  COMMAND ${COPY} ${HEADER_NATIVE_PATH} ${DST_INC_NATIVE_DIR} )
	endforeach(HEADER)
	add_dependencies(${libname} ${HEADER_PROJ_NAME})
	# MOD-BY-LEETEN 01/24/2011-END
	
	# MOD-BY-LEETEN 01/24/2011-FROM:
		# if( with_shaders )
			# # convert all GLSL shaders to .h
			# set(SHADER_FILES "")
			# list(APPEND SHADER_FILES ${FRAG_FILES} ${VERT_FILES} ${GEOM_FILES} )
			# foreach(SHADER ${SHADER_FILES})
				# file(TO_NATIVE_PATH ${SHADER} SHADER_NATIVE_PATH)
				# add_custom_command (
				  # TARGET ${libname}
				  # PRE_BUILD
				  # COMMAND shader2string ${SHADER_NATIVE_PATH} ${SHADER_NATIVE_PATH}.h
				# )
			# endforeach(SHADER)
		# endif()
	# MOD-BY-LEETEN 01/24/2011-TO:
	set(SHADER_FILES "")
	list(APPEND SHADER_FILES ${FRAG_FILES} ${VERT_FILES} ${GEOM_FILES} )
	list(LENGTH SHADER_FILES N_SHADER_FILES)
	if( N_SHADER_FILES GREATER 0 )
		set(SHADER_PROJ_NAME ${libname}_glsl)
		add_custom_target (${SHADER_PROJ_NAME} 
			SOURCES ${SHADER_FILES}
			)
			
		# convert all GLSL shaders to .h
		foreach(SHADER ${SHADER_FILES})
			file(TO_NATIVE_PATH ${SHADER} SHADER_NATIVE_PATH)
			add_custom_command (
			  TARGET ${SHADER_PROJ_NAME}
			  POST_BUILD
			  COMMAND shader2string ${SHADER_NATIVE_PATH} ${SHADER_NATIVE_PATH}.h
			)
		endforeach(SHADER)
		if( N_SRC_FILES GREATER 0 )
			add_dependencies(${libname} ${SHADER_PROJ_NAME})
		endif()
	endif()
	# MOD-BY-LEETEN 01/24/2011-END

	# copy_headers (${libname} ${src_subdir} ${dst_subdir})
endfunction (build_library)

# ADD-BY-LEETEN 01/23/2011-BEGIN
function(build_executable exename src_dir)
	AUX_SOURCE_DIRECTORY ("${src_dir}" 	SRC_FILES)
	file(GLOB HEADER_FILES "${src_dir}/*.h" )
	file(GLOB VERT_FILES "${src_dir}/*.vert" )
	file(GLOB FRAG_FILES "${src_dir}/*.frag" )
	file(GLOB GEOM_FILES "${src_dir}/*.geom" )
	
	# MOD-BY-LEETEN 01/24/2011-FROM:
		# add_executable ( ${exename} ${SRC_FILES} ${HEADER_FILES} ${VERT_FILES} ${FRAG_FILES} ${GEOM_FILES})
	# TO:
	add_executable ( ${exename} ${SRC_FILES} ${HEADER_FILES} )
	# MOD-BY-LEETEN 01/24/2011-END

	# MOD-BY-LEETEN 01/24/2011-FROM:
		# # convert all GLSL shaders to .h
		# set(SHADER_FILES "")
		# list(APPEND SHADER_FILES ${FRAG_FILES} ${VERT_FILES} ${GEOM_FILES} )
		# foreach(SHADER ${SHADER_FILES})
			# file(TO_NATIVE_PATH ${SHADER} SHADER_NATIVE_PATH)
			# add_custom_command (
			  # TARGET ${exename}
			  # PRE_BUILD
			  # COMMAND shader2string ${SHADER_NATIVE_PATH} ${SHADER_NATIVE_PATH}.h
			# )
		# endforeach(SHADER)
	# MOD-BY-LEETEN 01/24/2011-TO:
	set(SHADER_FILES "")
	list(APPEND SHADER_FILES ${FRAG_FILES} ${VERT_FILES} ${GEOM_FILES} )
	list(LENGTH SHADER_FILES N_SHADER_FILES)
	if( N_SHADER_FILES GREATER 0 )
		set(SHADER_PROJ_NAME ${exename}_glsl)
		add_custom_target (${SHADER_PROJ_NAME} 
			SOURCES ${SHADER_FILES}
			)
			
		# convert all GLSL shaders to .h
		foreach(SHADER ${SHADER_FILES})
			file(TO_NATIVE_PATH ${SHADER} SHADER_NATIVE_PATH)
			add_custom_command (
			  TARGET ${SHADER_PROJ_NAME}
			  POST_BUILD
			  COMMAND shader2string ${SHADER_NATIVE_PATH} ${SHADER_NATIVE_PATH}.h
			)
		endforeach(SHADER)
		add_dependencies(${exename} ${SHADER_PROJ_NAME})
	endif()
	# MOD-BY-LEETEN 01/24/2011-END

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
# ADD-BY-LEETEN 01/24/2011-BEGIN
if(DEFINED WIN32) 
# ADD-BY-LEETEN 01/24/2011-END
target_link_libraries (shader2string "${THIRDPARTY_LIB_DIR}/glew32.lib")
# ADD-BY-LEETEN 01/24/2011-BEGIN
else()
	target_link_libraries (shader2string "GLEW")
endif()
# ADD-BY-LEETEN 01/24/2011-END

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
			# DEL-BY-LEETEN 01/24/2011-BEGIN
				# libtf2 
			# DEL-BY-LEETEN 01/24/2011-END
			libfbo
	)
	build_library(${LIB_NAME} "/" "/" false)
endforeach(LIB_NAME)

##############################################################
# libgw
build_library(libgw "libgw/" "/")
add_dependencies(libgw libbuf libopengl liblog libfps)

# DEL-BY-LEETEN 01/24/2011-BEGIN
	# ##############################################################
	# # libraries w/ their own subfolder under the folder include/
	# foreach(LIB_NAME 
				# libdvr2 
		# )
		# build_library(${LIB_NAME} "/" "${LIB_NAME}/")
	# endforeach(LIB_NAME)
# DEL-BY-LEETEN 01/24/2011-BEGIN

##############################################################
# libraries w/ their own subfolder under the folder include/
# and source codes in GLSL (.frag, .vert, and .geom)
foreach(LIB_NAME 
			# ADD-BY-LEETEN 01/24/2011-BEGIN
			libdvr2 
			# ADD-BY-LEETEN 01/24/2011-END
			libclip
			libdvr
			libtfw
	)
	build_library(${LIB_NAME} "/" "${LIB_NAME}/")
endforeach(LIB_NAME)

# MOD-BY-LEETEN 01/23/2011-FROM:
	# add_dependencies(libclip libfbow shader2string)
# TO:
add_dependencies(libclip libgw libfbow shader2string)
add_dependencies(libtfw libgw)
# MOD-BY-LEETEN 01/23/2011-END

# ADD-BY-LEETEN 01/24/2011-BEGIN
add_dependencies(libfbo libopengl)
add_dependencies(libopengl libbuf)
# ADD-BY-LEETEN 01/24/2011-END

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
	# ADD-BY-LEETEN 01/24/2011-BEGIN
	if(NOT DEFINED WIN32)
		target_link_libraries (${LIBGW_TESTER_NAME} libfps opencv_core opencv_highgui "GLEW" "GL" "glut" "glui" ) 
	endif()
	# ADD-BY-LEETEN 01/24/2011-END
	add_test (${LIBGW_TESTER_NAME} ${LIBGW_TESTER_NAME})
endforeach(LIBGW_TESTER_NAME)

# ADD-BY-LEETEN 01/23/2011-BEGIN
build_executable(example_dvr ${PROJECT_SOURCE_DIR}/libdvr/example_dvr)
add_dependencies(example_dvr libgw libdvr)
target_link_libraries (example_dvr libgw libdvr libtfw libopt)
# ADD-BY-LEETEN 01/24/2011-BEGIN
if(NOT DEFINED WIN32)
	target_link_libraries (example_dvr libfbo libshader libfps opencv_core opencv_highgui "GL" "glut" "glui" "GLEW" ) 
endif()	
# ADD-BY-LEETEN 01/24/2011-END
add_test (example_dvr example_dvr)

# build_executable(example_dvr2 ${PROJECT_SOURCE_DIR}/libdvr2/example_dvr2)
# add_dependencies(example_dvr2 libgw libdvr2)
# target_link_libraries (example_dvr2 libgw libdvr2 libtfw libopt libclip libshader)
# # ADD-BY-LEETEN 01/24/2011-BEGIN
# target_link_libraries (example_dvr libfbo libshader libfps opencv_core opencv_highgui "GLEW" "GL" "glut" "glui" ) 
# if(NOT DEFINED WIN32)
	# target_link_libraries (example_dvr opencv_core opencv_highgui ) 
# endif()	
# # ADD-BY-LEETEN 01/24/2011-END
# add_test (example_dvr2 example_dvr2)
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
# Revision 1.3  2011/01/24 19:50:51  leeten
#
# [01/24/2011]
# 1. [MOD] Remove the FORCE property when setting the needed variables,
# 2. [ADD] Define a new variable MYLIB_LIB_DIR.
# 3. [ADD] Add the .geom files to the projects.
# 4. [ADD] Define a new function build_executable.
# 5. [ADD] Install the library and headers to the folder CMAKE_INSTALL_PREFIX.
#
# Revision 1.2  2011/01/24 02:21:08  leeten
#
# [01/23/2011]
# 1. [ADD] Add the variable LIBRARY_OUTPUT_PATH.
# 2. [ADD] Make the LIBGW samples depend on LIBFPS.
# 
#
