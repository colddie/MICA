cmake_minimum_required(VERSION 2.8)
 
project(bsplineMultiReg)
 
if( NOT CMAKE_BUILD_TYPE )

set( CMAKE_BUILD_TYPE DEBUG CACHE STRING
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

set ( COMMON_DIRECTORIES  "G:/Common" )
set ( REG_COMMON_DIRECTORIES "G:/Registration/Common" )
#mark_as_advanced ( COMMON_DIRECTORIES )
#set( COMMON_DIRECTORIES "${COMMON_DIRECTORIES}")
include_directories (${COMMON_DIRECTORIES})
include_directories (${REG_COMMON_DIRECTORIES})
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
file (GLOB COMMON_DIRECTORIES "${COMMON_DIRECTORIES}/*.*")
file (GLOB REG_COMMON_DIRECTORIES "${REG_COMMON_DIRECTORIES}/*.*")

set(SRC main.cxx bsplineMultiReg.cxx bsplineMultiReg.h ${COMMON_DIRECTORIES} ${REG_COMMON_DIRECTORIES})
add_executable(bsplineMultiReg ${SRC})
target_link_libraries(bsplineMultiReg ${ITK_LIBRARIES})
