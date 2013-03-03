cmake_minimum_required(VERSION 2.8)
 
project(mhaToim)
 
if( NOT CMAKE_BUILD_TYPE )

set( CMAKE_BUILD_TYPE Debug CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo
MinSizeRel."
       FORCE )
endif()

find_package (ITK)
if (ITK_FOUND)
	include ( ${ITK_USE_FILE} )
else (ITK_FOUND)
	message (FATAL_ERROR "Cannot build without ITK, Please set ITK_DIR.")
endif (ITK_FOUND)

add_executable(mhaToim mhaToim.cxx)
target_link_libraries(mhaToim ${ITK_LIBRARIES})