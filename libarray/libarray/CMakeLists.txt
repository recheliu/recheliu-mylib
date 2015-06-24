CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#The name of the project
PROJECT(libarray_src)

# DEL-BY-LEETEN 2015/06/23:	find_package(mylib REQUIRED)

set(HEADER_FILES
	# This one is used to troubleshoot a performance isseu,
	# which will be deprecated soon.
	arrayND.h
	
	vectorND.h
	# ADD-BY-LEETEN 2014/12/25-BEGIN-BEGIN
	util.h
	# ADD-BY-LEETEN 2014/12/25-BEGIN-END
	)
	
add_custom_target (${PROJECT_NAME}
	SOURCES	${HEADER_FILES}
)

install(
	FILES ${HEADER_FILES}
	DESTINATION "include/libarray")

