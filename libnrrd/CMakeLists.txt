CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#The name of the project
PROJECT(libnrrd)

set(HEADER_FILES
	libnrrd.h)
	
add_custom_target (libnrrd
	SOURCES	${HEADER_FILES}
)

install(
	FILES ${HEADER_FILES}
	DESTINATION "include/")

