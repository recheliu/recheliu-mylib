CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#The name of the project
PROJECT(libtimer)

set(HEADER_FILES
	libtimer.h
	)
	
add_custom_target (${PROJECT_NAME}
	SOURCES	${HEADER_FILES}
)

install(
	FILES ${HEADER_FILES}
	DESTINATION "include")

