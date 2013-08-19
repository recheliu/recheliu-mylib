CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#The name of the project
PROJECT(contourspectrum)

set(HEADER_FILES
	contourspectrum.h)
	
add_custom_target (${PROJECT_NAME}
	SOURCES	${HEADER_FILES}
)

install(
	FILES ${HEADER_FILES}
	DESTINATION "include/")

set(WITH_PYTHON	OFF	CACHE BOOL	"Is Python supported?")

if( WITH_PYTHON )
	add_subdirectory(python)
endif()