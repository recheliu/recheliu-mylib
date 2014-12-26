CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#The name of the project
PROJECT(libarray_src)

find_package(mylib REQUIRED)

set(HEADER_FILES
	# This one is used to troubleshoot a performance isseu,
	# which will be deprecated soon.
	arrayND.h
	vectorND.h
	)
	
add_custom_target (${PROJECT_NAME}
	SOURCES	${HEADER_FILES}
)

install(
	FILES ${HEADER_FILES}
	DESTINATION "include/libarray")

