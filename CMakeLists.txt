# CMakeList.txt
#
# Compile using UBUNTU WSL:
# https://learn.microsoft.com/de-de/cpp/linux/cmake-linux-project?view=msvc-170
# https://stackoverflow.com/questions/32801638/cmake-error-at-cmakelists-txt30-project-no-cmake-c-compiler-could-be-found

# Disable Windows message
if (WIN32)
	message( SEND_ERROR "CODEBASE ONLY DEFINED FOR UNIX SYSTEMS!" )
endif (WIN32)

# Set unix enviroment
if (UNIX)
	project ("EgmTool" CXX)
	cmake_minimum_required (VERSION 3.19)

	# Include files
	file(GLOB SOURCES	"${CMAKE_SOURCE_DIR}/*.cpp" "${CMAKE_SOURCE_DIR}/*.h"
						"${CMAKE_SOURCE_DIR}/src/egm/demo/*.cpp" "${CMAKE_SOURCE_DIR}/src/egm/demo/*.h")

	# Include third-party files
	include_directories(${CMAKE_SOURCE_DIR}/src/egm/inc)

	# Add sources to project executable
	add_executable (EgmTool ${SOURCES})

	# Link libaries
	target_link_libraries(EgmTool PUBLIC ${CMAKE_SOURCE_DIR}/src/egm/lib/libprotobuf.a)
endif (UNIX)