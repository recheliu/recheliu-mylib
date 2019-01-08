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
set(FREEGLUT OFF CACHE BOOL "Flag to decide whether FREEGLUT is used")
set(WITH_STATIC_LINK OFF CACHE BOOL "Flag to decide whether static libraries are linked")	
# CUDA
set(CUDA_DIR "${CUDA_TOOLKIT_ROOT_DIR}")
set(CUDA_INC_DIR "${CUDA_DIR}/include")
set(CUDA_LIB_DIR "${CUDA_DIR}/lib")

# CUDA SDK
set(CUDASDK_DIR "${CUDA_SDK_ROOT_DIR}")
set(CUDASDK_INC_DIR "${CUDASDK_DIR}/inc")
set(CUDASDK_LIB_DIR "${CUDASDK_DIR}/lib")

# 3rd party lib
find_package(ThirdPartyLib)

# OpenCV
set(WITH_OPENCV OFF CACHE BOOL "Flag to decide whether FREEGLUT is used")
if( WITH_OPENCV )
	set(WITH_STATIC_OPENCV	OFF CACHE BOOL "Flag to decide whether the static version of OpenCV is linked")
	set(OPENCV_DIR "C:/OpenCV2.1" CACHE PATH "The installed path of OpenCV")
	set(OPENCV_VER "210" CACHE STRING "The Version of OpenCV")
	set(OPENCV_INC_DIR "${OPENCV_DIR}/include")
	set(OPENCV_LIB_DIR "${OPENCV_DIR}/lib")
	set(OPENCV_MESSAGE		"\"OpenCV Ver. ${OPENCV_VER} will be used."\")
	set(OPENCV_CORE_LIB		"\"opencv_core${OPENCV_VER}.lib\"")
	set(OPENCV_HIGHGUI_LIB	"\"opencv_highgui${OPENCV_VER}.lib\"")
	set(OPENCV_CORE_DEBUG_LIB		"\"opencv_core${OPENCV_VER}d.lib\"")
	set(OPENCV_HIGHGUI_DEBUG_LIB	"\"opencv_highgui${OPENCV_VER}d.lib\"")
	configure_file (
	  "${PROJECT_SOURCE_DIR}/libgw/libgw/GlutWin_config.h.in"
	  "${PROJECT_SOURCE_DIR}/libgw/libgw/GlutWin_config.h"
	)  
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

set(WITH_NRRD OFF CACHE BOOL "Flag to decide whether NRRD is used")
if( ${WITH_NRRD} )
	add_definitions(
		-DWITH_NRRD=1
	)
endif()

set(WITH_OPENGL OFF CACHE BOOL "Flag to decide whether OpenGL is used")
set(WITH_CUDA   OFF CACHE BOOL "Flag to decide whether CUDA is used")

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
	"${CMAKE_SOURCE_DIR}"
	)

# add the binary tree to the search path for libraries
list(APPEND LIB_DIRS
	"${CUDA_LIB_DIR}"
	"${CUDASDK_LIB_DIR}"
	)

link_directories(
	${LIB_DIRS}
)

set(CMAKE_LIBRARY_PATH ${LIB_DIRS})
if( WITH_STATIC_LINK )
	message(WARNING "To link static libraries in lib/ for VC projects, please add '/NODEFAULTLIB:LIBCMT' to CMAKE_EXE_LINKER_FLAGS.")

	add_definitions(
		-DGLEW_STATIC
		-DWITH_STATIC_LINK
	)	
endif()

if( WITH_STATIC_OPENCV )
	message(WARNING "To link static libraries in lib/ for VC projects, please add '/NODEFAULTLIB:LIBCMT' to CMAKE_EXE_LINKER_FLAGS.")
	
	link_directories (
		"${OPENCV_DIR}/share/OpenCV/3rdparty/lib"
		)
		
	add_definitions(
		-DWITH_STATIC_OPENCV
	)	
endif()

if( FREEGLUT )
	add_definitions(
		-DFREEGLUT_LIB_PRAGMAS=0
	)
	add_definitions(
		-DUSE_FREEGLUT
	)
	if( WITH_STATIC_LINK )
		add_definitions(
			-DFREEGLUT_STATIC
		)	
	endif()
endif()

add_definitions(
	-DGLUT_BUILDING_LIB
)

if(DEFINED WIN32)
	add_definitions(
		-D_CRT_SECURE_NO_WARNINGS
		-Dstrdup=_strdup
	)
endif()

add_subdirectory(contourspectrum)
add_subdirectory(liblog)
include_directories(liblog)
add_subdirectory(libarray)
include_directories(libarray)
# TODO: Fix liboptpp.
add_subdirectory(libtimer)

##############################################################
function(build_library libname src_subdir dst_subdir)
	set(SRC_DIR "${PROJECT_SOURCE_DIR}/${libname}/${src_subdir}/")

	include_directories (
		"${SRC_DIR}"
	)
	
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
			${SHADER_FILES}
			${HEADER_FILES}
			${SRC_FILES}
			 )
		set_target_properties(${libname} PROPERTIES 
			DEBUG_OUTPUT_NAME "${libname}_d"
			RELEASE_OUTPUT_NAME "${libname}_r"
		)
		install(
			TARGETS ${libname}
			DESTINATION lib	)	
		
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
        # TEST-ADD-BEGIN
        install(
            FILES ${SHADER}.h
            DESTINATION "include/${dst_subdir}")
        # TEST-ADD-END
        
		# Add dependecy to the shader file to force the genration of their .h in priori to the compilation
		SET_SOURCE_FILES_PROPERTIES(${SRC_FILES} 
			OBJECT_DEPENDS ${SHADER}.h)
	endforeach(SHADER)

	# create the folder for the headers
	install(
		FILES ${HEADER_FILES}
		DESTINATION "include/${dst_subdir}")
endfunction (build_library)

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
		
	target_link_libraries(${exename}	${ThirdPartyLib_LIBRARIES})	
		

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
	
	install(
		TARGETS ${exename}
		DESTINATION bin	)	

endfunction (build_executable)

 
build_executable(shader2string ${PROJECT_SOURCE_DIR}/libfilestr/shader2string)

##############################################################
foreach(LIB_NAME 
			libbuf 
			libclock 
			libopt 
			libfps 
			libfilestr
	)
	build_library(${LIB_NAME} "/" "/" false)
endforeach(LIB_NAME)

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

# Thid library libdvrsuite combines the volume renderer (LIBDVR2), transfer function viewer and editor (LIBTFW)
build_library(libdvrsuite "/" "libdvrsuite")
add_dependencies(libdvrsuite libdvr2 libtfw)

add_dependencies(libclip libgw libfbow shader2string)
add_dependencies(libtfw libgw)

add_dependencies(libfbow libfbo)
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
	target_link_libraries (${LIBGW_TESTER_NAME} 
		libfps 
	)
	if(NOT DEFINED WIN32)
		target_link_libraries (${LIBGW_TESTER_NAME} 
			libfps 
			"GLEW" 
			"GL" 
			"GLU" 
			"glut" 
			"glui" ) 
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
	endif()
	add_test (${LIBGW_TESTER_NAME} ${LIBGW_TESTER_NAME})
endforeach(LIBGW_TESTER_NAME)

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
endif()
  
##############################################################
# create the header for the makefile
configure_file (
	"${PROJECT_SOURCE_DIR}/mylib.mak.in"
	"${PROJECT_BINARY_DIR}/mylib.mak"
)

##############################################################
if( WITH_OPENGL ) 
install(
	TARGETS shader2string 
	DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif() # if( WITH_OPENGL ) 

################################################################
if( WITH_NRRD) 
	add_subdirectory(libnrrd)
endif()

configure_file (
  "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  @ONLY
  )
  
install(
	FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
	DESTINATION ".")

