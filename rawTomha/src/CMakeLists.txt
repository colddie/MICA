cmake_minimum_required(VERSION 2.8)
 
project(rawTomha)
 
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

find_package (Boost COMPONENTS filesystem REQUIRED)
if (Boost_FOUND)
    include_directories ( ${Boost_INCLUDE_DIRS} )
else (Boost_FOUND)
	message (FATAL_ERROR "Cannot build without Boost, Please set Boost_DIR.")
endif (Boost_FOUND)

add_executable(rawTomha rawTomha.cxx)
target_link_libraries(rawTomha ${ITK_LIBRARIES} ${Boost_LIBRARIES})