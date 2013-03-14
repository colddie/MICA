cmake_minimum_required(VERSION 2.8)

project(resampleImage)

if( NOT CMAKE_BUILD_TYPE )
set( CMAKE_BUILD_TYPE Debug CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo
MinSizeRel."
       FORCE )
#set( CMAKE_CXX_FLAGS_RELEASE -O3 DNDEBUG)
endif()



find_package (ITK)
if (ITK_FOUND)
        include ( ${ITK_USE_FILE} )
else (ITK_FOUND)
        message (FATAL_ERROR "Cannot build without ITK, Please set ITK_DIR.")
endif (ITK_FOUND)

set ( COMMON_DIRECTORIES  "G:/Common" )
#mark_as_advanced ( COMMON_DIRECTORIES )
#set( COMMON_DIRECTORIES "${COMMON_DIRECTORIES}")
include_directories (${COMMON_DIRECTORIES})
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
file (GLOB COMMON_DIRECTORIES "${COMMON_DIRECTORIES}/*.*")

set(SRC_LIST resampleImage.cxx resampleImage.h resampleImage.txx ${COMMON_DIRECTORIES})
add_executable(resampleImage ${SRC_LIST})
target_link_libraries(resampleImage ${ITK_LIBRARIES})
#ADD_LIBRARY (libaddS SHARED ${SRC_LIST})





