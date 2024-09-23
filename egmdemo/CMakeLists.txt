# CMakeList.txt

# Disable Windows message
if (WIN32)
	message( SEND_ERROR "CODEBASE ONLY DEFINED FOR UNIX SYSTEMS!" )
endif (WIN32)

# Set unix enviroment
if (UNIX)
	project ("egmdemo" CXX)
	cmake_minimum_required (VERSION 3.19)

	# Include files
	file(GLOB SOURCES	"${CMAKE_SOURCE_DIR}/*.cpp" "${CMAKE_SOURCE_DIR}/*.h"
						"${CMAKE_SOURCE_DIR}/src/egm/*.cpp" "${CMAKE_SOURCE_DIR}/src/egm/*.h")

	# Include third-party files
	include_directories(${CMAKE_SOURCE_DIR}/src/inc)

	# Add sources to project executable
	add_executable (egmdemo ${SOURCES})

	# Link libaries
	#add_library(libprotobuf SHARED IMPORTED)
	#set_target_properties(libprotobuf PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/lib)
	target_link_libraries(egmdemo PUBLIC ${CMAKE_SOURCE_DIR}/src/lib/libprotobuf.a)
endif (UNIX)