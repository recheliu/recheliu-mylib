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
set(THIRDPARTY_DIR "D:/lib" CACHE PATH "The path to the 3rd party libraries")
set(FREEGLUT OFF CACHE BOOL "Flag to decide whether FREEGLUT is used")
set(WITH_STATIC_LINK OFF CACHE BOOL "Flag to decide whether static libraries are linked")	# ADD-BY-LEETEN 04/05/2012
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
# ADD-BY-LEETEN 01/19/2013-BEGIN
set(WITH_OPENCV OFF CACHE BOOL "Flag to decide whether FREEGLUT is used")
if( WITH_OPENCV )
	set(WITH_STATIC_OPENCV	OFF CACHE BOOL "Flag to decide whether the static version of OpenCV is linked")
	set(OPENCV_DIR "C:/OpenCV2.1" CACHE PATH "The installed path of OpenCV")
	set(OPENCV_VER "210" CACHE STRING "The Version of OpenCV")
# ADD-BY-LEETEN 01/19/2013-END
set(OPENCV_INC_DIR "${OPENCV_DIR}/include")
set(OPENCV_LIB_DIR "${OPENCV_DIR}/lib")
# ADD-BY-LEETEN 10/14/2011-BEGIN
set(OPENCV_MESSAGE		"\"OpenCV Ver. ${OPENCV_VER} will be used."\")
set(OPENCV_CORE_LIB		"\"opencv_core${OPENCV_VER}.lib\"")
set(OPENCV_HIGHGUI_LIB	"\"opencv_highgui${OPENCV_VER}.lib\"")
# ADD-BY-LEETEN 04/05/2012-BEGIN
set(OPENCV_CORE_DEBUG_LIB		"\"opencv_core${OPENCV_VER}d.lib\"")
set(OPENCV_HIGHGUI_DEBUG_LIB	"\"opencv_highgui${OPENCV_VER}d.lib\"")
# ADD-BY-LEETEN 04/05/2012-END
configure_file (
  "${PROJECT_SOURCE_DIR}/libgw/libgw/GlutWin_config.h.in"
  "${PROJECT_SOURCE_DIR}/libgw/libgw/GlutWin_config.h"
)  
# ADD-BY-LEETEN 10/14/2011-END
# ADD-BY-LEETEN 01/19/2013-BEGIN
	include_directories (
		"${OPENCV_INC_DIR}"
		"${OPENCV_INC_DIR}/opencv"	
	)

	list(APPEND LIB_DIRS
		"${OPENCV_LIB_DIR}"
	)

	add_definitions(
		-DWITH_OPENCV
		-DOPENCV_VER=${OPENCV_VER}
	)
endif() 
# ADD-BY-LEETEN 01/19/2013-END

# ADD-BY-LEETEN 10/21/2011-BEGIN
set(WITH_NRRD OFF CACHE BOOL "Flag to decide whether NRRD is used")
if( ${WITH_NRRD} )
	add_definitions(
		-DWITH_NRRD=1
	)
endif()
# ADD-BY-LEETEN 10/21/2011-END

# ADD-BY-LEETEN 09/07/2011-BEGIN
set(WITH_OPENGL OFF CACHE BOOL "Flag to decide whether OpenGL is used")
set(WITH_CUDA   OFF CACHE BOOL "Flag to decide whether CUDA is used")
# ADD-BY-LEETEN 09/07/2011-END

# mylib
set(MYLIB_BIN_DIR "${PROJECT_BINARY_DIR}/bin")
set(MYLIB_INC_DIR "${PROJECT_BINARY_DIR}/include")
set(MYLIB_LIB_DIR "${PROJECT_BINARY_DIR}/lib")
set(MYLIB_LIB_DIR_DEBUG "${MYLIB_LIB_DIR}/debug")
set(MYLIB_LIB_DIR_RELEASE "${MYLIB_LIB_DIR}/release")

# add the binary tree to the search path for include files
include_directories (
	"${CUDA_INC_DIR}"
	"${CUDASDK_INC_DIR}"
	"${THIRDPARTY_INC_DIR}"
	"${CMAKE_SOURCE_DIR}"
	)

# add the binary tree to the search path for libraries
list(APPEND LIB_DIRS
	"${CUDA_LIB_DIR}"
	"${CUDASDK_LIB_DIR}"
	"${THIRDPARTY_LIB_DIR}"
	)

link_directories(
	${LIB_DIRS}
)

set(CMAKE_LIBRARY_PATH ${LIB_DIRS})
# ADD-BY-LEETEN 04/05/2012-BEGIN
if( WITH_STATIC_LINK )
	message(WARNING "To link static libraries in lib/ for VC projects, please add '/NODEFAULTLIB:LIBCMT' to CMAKE_EXE_LINKER_FLAGS.")

	add_definitions(
		-DGLEW_STATIC
		-DWITH_STATIC_LINK
	)	
endif()
# ADD-BY-LEETEN 04/05/2012-END

# ADD-BY-LEETEN 07/12/2012-BEGIN
if( WITH_STATIC_OPENCV )
	message(WARNING "To link static libraries in lib/ for VC projects, please add '/NODEFAULTLIB:LIBCMT' to CMAKE_EXE_LINKER_FLAGS.")
	
	link_directories (
		"${OPENCV_DIR}/share/OpenCV/3rdparty/lib"
		)
		
	add_definitions(
		-DWITH_STATIC_OPENCV
	)	
endif()
# ADD-BY-LEETEN 07/12/2012-BEGIN


if( FREEGLUT )
	add_definitions(
		-DFREEGLUT_LIB_PRAGMAS=0
	)
	add_definitions(
		-DUSE_FREEGLUT
	)
	# ADD-BY-LEETEN 04/05/2012-BEGIN
	if( WITH_STATIC_LINK )
		add_definitions(
			-DFREEGLUT_STATIC
		)	
	endif()
	# ADD-BY-LEETEN 04/05/2012-END
endif()

add_definitions(
	-DGLUT_BUILDING_LIB
)

##############################################################
function(build_library libname src_subdir dst_subdir)
	set(SRC_DIR "${PROJECT_SOURCE_DIR}/${libname}/${src_subdir}/")

	# ADD-BY-LEETEN 09/02/2011-BEGIN
	include_directories (
		"${SRC_DIR}"
	)
	# ADD-BY-LEETEN 09/02/2011-END
	
	AUX_SOURCE_DIRECTORY ("${SRC_DIR}" 	SRC_FILES)
	file(GLOB HEADER_FILES "${SRC_DIR}/*.h" )
	file(GLOB FRAG_FILES "${SRC_DIR}/*.frag" )
	file(GLOB VERT_FILES "${SRC_DIR}/*.vert" )
	file(GLOB GEOM_FILES "${SRC_DIR}/*.geom" )

	# remove the headers convertted from the shaders
	set(TMP_HEADER_FILES "")
	foreach(HEADER ${HEADER_FILES})
		string(
			REGEX MATCH "frag.h|vert.h|geom.h"
			MATCHED_PREFIX ${HEADER}
		)
		if( NOT MATCHED_PREFIX )
			list(APPEND TMP_HEADER_FILES ${HEADER})
		endif()
	endforeach(HEADER)
	set(HEADER_FILES ${TMP_HEADER_FILES})
	
	# merge all shader files into a single list 
	set(SHADER_FILES "")
	list(APPEND SHADER_FILES ${FRAG_FILES} ${VERT_FILES} ${GEOM_FILES} )
	
	list(LENGTH SRC_FILES N_SRC_FILES)
	if( N_SRC_FILES EQUAL 0 )
		add_custom_target (
			${libname}
			SOURCES 
				${SHADER_FILES}
				${HEADER_FILES}
				${SRC_FILES}
		)
	else()
		add_library ( ${libname} 
			# ADD-BY-LEETEN 05/15/2011-BEGIN
			${SHADER_FILES}
			${HEADER_FILES}
			# ADD-BY-LEETEN 05/15/2011-END
			${SRC_FILES}
			 )
		set_target_properties(${libname} PROPERTIES 
			DEBUG_OUTPUT_NAME "${libname}_d"
			RELEASE_OUTPUT_NAME "${libname}_r"
		)
		# ADD-BY-LEETEN 09/02/2011-BEGIN
		install(
			TARGETS ${libname}
			DESTINATION lib	)	
		# ADD-BY-LEETEN 09/02/2011-END
		
	endif()

	# convert all GLSL shaders to .h
	foreach(SHADER ${SHADER_FILES})
		file(TO_NATIVE_PATH ${SHADER} SHADER_NATIVE_PATH)
		file(TO_NATIVE_PATH ${SHADER}.h SHADER_OUTPUT_NATIVE_PATH)
		add_custom_command (
			OUTPUT 	${SHADER}.h
			MAIN_DEPENDENCY ${SHADER}
			DEPENDS ${SHADER}
			COMMAND shader2string ${SHADER_NATIVE_PATH} ${SHADER_OUTPUT_NATIVE_PATH}
		)
		# ADD-BY-LEETEN 02/08/2013-BEGIN
		# Add dependecy to the shader file to force the genration of their .h in priori to the compilation
		SET_SOURCE_FILES_PROPERTIES(${SRC_FILES} 
			OBJECT_DEPENDS ${SHADER}.h)
		# ADD-BY-LEETEN 02/08/2013-END
	endforeach(SHADER)

	# create the folder for the headers
	install(
		FILES ${HEADER_FILES}
		DESTINATION "include/${dst_subdir}")
endfunction (build_library)

# ADD-BY-LEETEN 01/23/2011-BEGIN
function(build_executable exename src_dir)
	AUX_SOURCE_DIRECTORY ("${src_dir}" 	SRC_FILES)
	file(GLOB HEADER_FILES "${src_dir}/*.h" )
	file(GLOB VERT_FILES "${src_dir}/*.vert" )
	file(GLOB FRAG_FILES "${src_dir}/*.frag" )
	file(GLOB GEOM_FILES "${src_dir}/*.geom" )
	
	# remove the headers convertted from the shaders
	set(TMP_HEADER_FILES "")
	foreach(HEADER ${HEADER_FILES})
		string(
			REGEX MATCH "frag.h|vert.h|geom.h"
			MATCHED_PREFIX ${HEADER}
		)
		if( NOT MATCHED_PREFIX )
			list(APPEND TMP_HEADER_FILES ${HEADER})
		endif()
	endforeach(HEADER)
	set(HEADER_FILES ${TMP_HEADER_FILES})

	# merge all shader files into a single list
	set(SHADER_FILES "")
	list(APPEND SHADER_FILES ${FRAG_FILES} ${VERT_FILES} ${GEOM_FILES} )
	
	add_executable ( 
		${exename} 
		${SRC_FILES} 
		${HEADER_FILES} 
		${SHADER_FILES}
		)

	# convert all GLSL shaders to .h
	foreach(SHADER ${SHADER_FILES})
		file(TO_NATIVE_PATH ${SHADER} SHADER_NATIVE_PATH)
		file(TO_NATIVE_PATH ${SHADER}.h SHADER_OUTPUT_NATIVE_PATH)
		add_custom_command(
			OUTPUT ${SHADER}.h
			MAIN_DEPENDENCY ${SHADER}
			DEPENDS ${SHADER}
			COMMAND shader2string ${SHADER_NATIVE_PATH} ${SHADER_OUTPUT_NATIVE_PATH}
		)
	endforeach(SHADER)
	
	# ADD-BY-LEETEN 09/02/2011-BEGIN
	install(
		TARGETS ${exename}
		DESTINATION bin	)	
	# ADD-BY-LEETEN 09/02/2011-END
endfunction (build_executable)

# ADD-BY-LEETEN 01/23/2011-END
 
build_executable(shader2string ${PROJECT_SOURCE_DIR}/libfilestr/shader2string)

##############################################################
foreach(LIB_NAME 
			libbuf 
			libclock 
			liblog 
			libopt 
			libfps 
			libgrid		# ADD-BY-LEETEN 12/03/2011
			libfilestr	# ADD-BY-LEETEN 01/26/2013
			contourspectrum # ADD-BY-LEETEN 04/20/2013
	)
	build_library(${LIB_NAME} "/" "/" false)
endforeach(LIB_NAME)

# ADD-BY-LEETEN 09/07/2011-BEGIN
if( WITH_CUDA )
	foreach(LIB_NAME 
			libcuda 
		)
		build_library(${LIB_NAME} "/" "/" false)
	endforeach(LIB_NAME)
endif()

if( WITH_OPENGL )
	foreach(LIB_NAME 
			libfbow
			libshader 
			libopengl 
			libfbo
		)
		build_library(${LIB_NAME} "/" "/" false)
	endforeach(LIB_NAME)

##############################################################
# libgw
build_library(libgw "libgw/" "/")
add_dependencies(libgw libbuf libopengl liblog libfps)

##############################################################
# libraries w/ their own subfolder under the folder include/
# and source codes in GLSL (.frag, .vert, and .geom)
foreach(LIB_NAME 
			libdvr2 
			libclip
			libdvr
			libtfw
	)
	build_library(${LIB_NAME} "/" "${LIB_NAME}/")
endforeach(LIB_NAME)

# ADD-BY-LEETEN 02/08/2013-BEGIN
# Thid library libdvrsuite combines the volume renderer (LIBDVR2), transfer function viewer and editor (LIBTFW)
build_library(libdvrsuite "/" "libdvrsuite")
add_dependencies(libdvrsuite libdvr2 libtfw)
# ADD-BY-LEETEN 02/08/2013-END

add_dependencies(libclip libgw libfbow shader2string)
add_dependencies(libtfw libgw)

# ADD-BY-LEETEN 01/28/2011-BEGIN
add_dependencies(libfbow libfbo)
# ADD-BY-LEETEN 01/28/2011-END
add_dependencies(libfbo libopengl)
add_dependencies(libopengl libbuf)

add_dependencies(libdvr libfbo)
add_dependencies(libdvr2 libfbow)

##############################################################		 
# LIBGW testing program
# 
foreach(LIBGW_TESTER_NAME example_single_win example_single_win_global example_multi_wins example_multi_wins_global example_multi_wins_glui)
	build_executable(${LIBGW_TESTER_NAME} ${PROJECT_SOURCE_DIR}/libgw/examples/${LIBGW_TESTER_NAME})
	add_dependencies(${LIBGW_TESTER_NAME} libgw liblog libbuf libfbow libfps)
	target_link_libraries (${LIBGW_TESTER_NAME} libgw)
	# ADD-BY-LEETEN 09/02/2011-BEGIN
	target_link_libraries (${LIBGW_TESTER_NAME} 
		libfps 
	)
	# ADD-BY-LEETEN 09/02/2011-END
	if(NOT DEFINED WIN32)
		target_link_libraries (${LIBGW_TESTER_NAME} 
			libfps 
			# DEL-BY-LEETEN 04/12/2013-BEGIN
			# opencv_core 
			# opencv_highgui 
			# DEL-BY-LEETEN 04/12/2013-END
			"GLEW" 
			"GL" 
			"GLU" 
			"glut" 
			"glui" ) 
		      # ADD-BY-LEETEN 04/12/2013-BEGIN
		      if(WITH_OPENCV)
			target_link_libraries (${LIBGW_TESTER_NAME} 
			  opencv_core
			  opencv_highgui
			  )
		      else()
			target_link_libraries (${LIBGW_TESTER_NAME} 
			  lodepng
			  )
		      endif()
		      # ADD-BY-LEETEN 04/12/2013-END
	endif()
	add_test (${LIBGW_TESTER_NAME} ${LIBGW_TESTER_NAME})
endforeach(LIBGW_TESTER_NAME)

# # MOD-BY-LEETEN 04/20/2013-FROM:
# ADD-BY-LEETEN 01/23/2011-BEGIN
# build_executable(example_dvr ${PROJECT_SOURCE_DIR}/libdvr/example_dvr)
# add_dependencies(example_dvr libgw libdvr)
# target_link_libraries (example_dvr 
	# libtfw
	# libdvr
	# libopt
	# libshader
	# libfbo	
	# libgw 
	# libfps 
	# ) 
# if(NOT DEFINED WIN32)	
	# target_link_libraries (example_dvr
	  # # DEL-BY-LEETEN 04/12/2013-BEGIN
		# # opencv_core 
		# # opencv_highgui 
		# # DEL-BY-LEETEN 04/12/2013-END
		# GLEW 
		# GL 
		# GLU 
		# glut 
		# glui 
	# ) 
      # # ADD-BY-LEETEN 04/12/2013-BEGIN
      # if( WITH_OPENCV )
	# target_link_libraries (example_dvr
	  # opencv_core 
	  # opencv_highgui 
	  # )
      # else()
	# target_link_libraries (example_dvr
	  # lodepng
	  # )
	# endif()
	# # ADD-BY-LEETEN 04/12/2013-END
# endif()
# add_test (example_dvr example_dvr "${PROJECT_SOURCE_DIR}/data/hipip")

# build_executable(example_dvr2 ${PROJECT_SOURCE_DIR}/libdvr2/example_dvr2)
# add_dependencies(example_dvr2 libgw libdvr2)
# target_link_libraries (example_dvr2
	# libtfw
	# libdvr2
	# libopt
	# libshader
	# libfbo
	# libclip
	# libgw 
	# # ADD-BY-LEETEN 01/28/2011-BEGIN
	# libfps 
	# # ADD-BY-LEETEN 01/28/2011-END
	# ) 
# if(NOT DEFINED WIN32)	
	# target_link_libraries (example_dvr2
	  # # DEL-BY-LEETEN 04/12/2013-BEGIN
		# # opencv_core 
		# # opencv_highgui 
		# # TEST-DEL-END
		# # DEL-BY-LEETEN 04/12/2013-END
		# ITKNrrdIO # ADD-BY-LEETEN 04/12/2013
		# z # ADD-BY-LEETEN 04/12/2013
		# GLEW 
		# GL 
		# GLU 
		# glut 
		# glui 
		# )
      # # ADD-BY-LEETEN 04/12/2013-BEGIN
      # if( WITH_OPENCV )
	# target_link_libraries (example_dvr2
	  # opencv_core 
	  # opencv_highgui 
	  # )
      # else()
	# target_link_libraries (example_dvr2
	  # lodepng
	  # )
	# endif()
	# # ADD-BY-LEETEN 04/12/2013-BEGIN
# endif()
# add_test (example_dvr2 example_dvr2 "${PROJECT_SOURCE_DIR}/data/hipip")

# # ADD-BY-LEETEN 02/08/2013-BEGIN
# build_executable(example_dvr_suite ${PROJECT_SOURCE_DIR}/libdvrsuite/example_dvr_suite)
# add_dependencies(example_dvr_suite libdvrsuite)
# target_link_libraries (example_dvr_suite
	# libdvrsuite # ADD-BY-LEETEN 04/12/2013
	# libtfw
	# libdvr2
	# # DEL-BY-LEETEN 04/12/2013: libdvrsuite
	# libopt
	# libshader
	# libfbo
	# libclip
	# libgw 
	# libfps 
	# ) 
# if(NOT DEFINED WIN32)	
  # # MOD-BY-LEETEN 04/12/2013-FROM:	target_link_libraries (example_dvr2_suite
  # target_link_libraries (example_dvr_suite
    # ITKNrrdIO 
    # z
  # # MOD-BY-LEETEN 04/12/2013-END
		# GLEW 
		# GL 
		# GLU 
		# glut 
		# glui 
		# )
      # # ADD-BY-LEETEN 04/12/2013-BEGIN
      # if( WITH_OPENCV )
	# target_link_libraries (example_dvr_suite
	  # opencv_core 
	  # opencv_highgui 
	  # )
      # else()
	# target_link_libraries (example_dvr_suite
	  # lodepng
	  # )
	# endif()
	# # ADD-BY-LEETEN 04/12/2013-END
# endif()
# add_test (example_dvr_suite example_dvr_suite "${PROJECT_SOURCE_DIR}/data/neghip.nhdr")
# ADD-BY-LEETEN 02/08/2013-END
# # MOD-BY-LEETEN 04/20/2013-TO:
	function(build_dvr dvrprj dvrsrc dvrlibs testargs)
		build_executable(${dvrprj} ${dvrsrc})
		foreach(dvrlib ${dvrlibs})
			add_dependencies(${dvrprj} ${dvrlib})
			target_link_libraries (${dvrprj}
				${dvrlib}
			)
		endforeach(dvrlib)		
		target_link_libraries (${dvrprj}
			libtfw
			libopt
			libshader
			libfbo
			libclip
			libgw 
			libfps 
			) 
		if(NOT DEFINED WIN32)	
			target_link_libraries (${dvrprj}
				ITKNrrdIO 
				z
				GLEW 
				GL 
				GLU 
				glut 
				glui 
				)
			if( WITH_OPENCV )
				target_link_libraries (${dvrprj}
					opencv_core 
					opencv_highgui 
				)
			else()
				target_link_libraries (${dvrprj}
					lodepng
				)
			endif()
		endif()

		add_test (${dvrprj} ${dvrprj} ${testargs})
	endfunction (build_dvr)
	
	build_dvr(example_dvr 		${PROJECT_SOURCE_DIR}/libdvr/example_dvr 			libdvr		"${PROJECT_SOURCE_DIR}/data/hipip")
	if( WITH_NRRD )
		build_dvr(example_dvr2 	${PROJECT_SOURCE_DIR}/libdvr2/example_dvr2			libdvr2		"${PROJECT_SOURCE_DIR}/data/neghip.nhdr")
	else()
		build_dvr(example_dvr2 	${PROJECT_SOURCE_DIR}/libdvr2/example_dvr2			libdvr2		"${PROJECT_SOURCE_DIR}/data/hipip")
	endif()
	build_dvr(example_dvr_suite ${PROJECT_SOURCE_DIR}/libdvrsuite/example_dvr_suite	"libdvr2;libdvrsuite"	"${PROJECT_SOURCE_DIR}/data/neghip.nhdr")
# # MOD-BY-LEETEN 04/20/2013-END
endif()
# ADD-BY-LEETEN 09/07/2011-END
  
##############################################################
# create the header for the makefile
configure_file (
	"${PROJECT_SOURCE_DIR}/mylib.mak.in"
	"${PROJECT_BINARY_DIR}/mylib.mak"
)

##############################################################
if( WITH_OPENGL ) # ADD-BY-LEETEN 09/07/2011
install(
	TARGETS shader2string 
	DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif() # if( WITH_OPENGL ) # ADD-BY-LEETEN 09/07/2011

#
# $Log: not supported by cvs2svn $
# Revision 1.14  2011-10-22 01:53:19  leeten
#
# [10/21/2011]
# 1. [ADD] Define a new variable WITH_NRRD. If it is 1, NRRD will be used, which is currently used by example_dvr2.
#
# Revision 1.13  2011-10-14 22:25:31  leeten
#
# [10/14/2011]
# 1. [ADD] Create the configuration file (GlutWin_config.h) for GlutWin. Currently this config file include the filenames of OpenCV libraries.
# 2. [KIL] Deleted older code segments.
#
# Revision 1.12  2011-09-08 00:58:47  leeten
#
#
# [09/07/2011]
# 1. [ADD] Add new parameter WITH_OPENGL and WITH_CUDA. When these preprocessors are off, a subset of libraries won't be compiled.
#
# Revision 1.11  2011-09-03 03:42:38  leeten
#
# [09/02/2011]
# 1. [KIL] Remove old deleted code segments.
# 2. [MOD] Change the parameter of the insall command so the manual copy can be avioded and make sure that all files will be installed.
#
# Revision 1.10  2011-07-06 03:21:25  leeten
#
# [07/05/2011]
# 1. [DEL] Remove old-deleted code segments.
#
# Revision 1.9  2011-05-16 00:05:44  leeten
#
# [05/15/2011]
# 1. [DEL] Remove the project w/ postfixes _glsh and _h.
# 2. [ADD] Use add_custom_command w/ MIN_DEPENDENCY and DEPENDS to force the compilation when a shader file is modified.
# 3. [DEL] Remove the headers generated by the programs from the project.
#
# Revision 1.8  2011/01/30 21:34:26  leeten
#
# [01/30/2011]
# 1. [DEL] Remove the redundant link to libfps.
# 2. [DEL] Remove the redundant links to the OpenGL/OpenCV libraries.
#
# Revision 1.7  2011/01/28 16:01:06  leeten
#
# [01/28/2011]
# 1. [ADD] Link the OpenGL and OpenCV libraries twice in order to build example_dvr and example_dvr2 on Linux.
# 2. [DEL] Remove the code deleted before 01/24/2011.
# 3. [ADD] Add the subfolder opencv to the include path.
# 4. [ADD] Link the library glut when build shader2string.
#
# Revision 1.6  2011/01/27 02:45:37  leeten
#
# [01/26/2011]
# 1. [DEL] Remove old deleted code segments.
#
# Revision 1.5  2011/01/25 06:18:09  leeten
#
# [01/25/2011]
# 1. [ADD] Add the path of the dataset HIPIP to the tests example_dvr and example_dvr2.
# 2. [ADD] Add the executable example_dvr2.
#
# Revision 1.4  2011/01/25 02:39:19  leeten
#
# [01/24/2011]
# 1. [MOD] Set the default value of FREEGLU to OFF.
# 2. [DEL] Remove the paramter with_shader in the function build_library.
# 3. [MOD] For the libraries with headers, create a new project to copy these headers to include/. The original project will depend on this project so thiss project will be built before the original project.
# 4. [MOD] For the libraries with shaders, create a new project to convert these shaders to .h. The original project will depend on this project so thiss project will be built before the original project.
# 5. [DEL] Remove the shaders and headers from the library projects.
# 6. [ADD] Add extra dependencies.
#
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
