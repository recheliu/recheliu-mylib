CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(liblog)

set(HEADER_FILES
    logvar.h
	liblog.h
    logmpi.h
    lognc.h
	)
	
add_custom_target (${PROJECT_NAME}
	SOURCES	${HEADER_FILES}
)

install(
	FILES ${HEADER_FILES}
	DESTINATION "include")

# # Currently the headers will be installed to 2 locations. 
# # One is directly under include (for backward compatibility).
# # The other is include/liblog (in order to organize the headers).
# install(
	# FILES ${HEADER_FILES}
	# DESTINATION "include/liblog")
