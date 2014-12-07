CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#The name of the project
PROJECT(liboptpp_src)

# TEST-ADD-BEGIN
find_package(mylib REQUIRED)
# TEST-ADD-END

set(HEADER_FILES
	OptEntry.h

	OptValueVector.h
	OptPrimitiveTypeVector.h
	
	OptCharPtrVector.h
	OptStringVector.h
	
	OptEnum.h
	OptBoolean.h
	
	OptParser.h
	
	liboptpp.h
	)
	
add_custom_target (${PROJECT_NAME}
	SOURCES	${HEADER_FILES}
)

install(
	FILES ${HEADER_FILES}
	DESTINATION "include/liboptpp")

