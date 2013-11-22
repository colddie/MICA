cmake_minimum_required(VERSION 2.8)

PROJECT(RegionGrowing)


if( NOT CMAKE_BUILD_TYPE )

  set( CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo
MinSizeRel."
       FORCE )
endif()

#----------------------------------------------------------------------------
FIND_PACKAGE(ITK)
if(ITK_FOUND)
        include( ${ITK_USE_FILE} )
else(ITK_FOUND)
        message(FATAL_ERROR "Cannot build without ITK, Please set ITK_DIR.")
endif(ITK_FOUND)

#----------------------------------------------------------------------------
FIND_PACKAGE(VTK)
if(VTK_FOUND)
        include( ${VTK_USE_FILE} )
else(VTK_FOUND)
        message(FATAL_ERROR "Cannot build without VTK, Please set VTK_DIR.")
endif(VTK_FOUND)

#----------------------------------------------------------------------------
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

#----------------------------------------------------------------------------
FIND_PACKAGE(CTK)
INCLUDE(${CTK_USE_FILE})

#----------------------------------------------------------------------------
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Cmake/Modules/")

#INCLUDE(${CMAKE_MODULE_PATH}/QxtConfig.cmake)
#INCLUDE(${CMAKE_MODULE_PATH}/QwtConfig.cmake)

#FIND_PACKAGE(Qxt)
#SET(QXT_FIND_COMPONENTS QxtCore, QxtGui)
#SET(QXT_USE_QXTCORE TRUE)
#SET(QXT_USE_QXTGUI TRUE)
#INCLUDE_DIRECTORIES(${QXT_INCLUDE_DIRS})

FIND_PACKAGE(Qwt)
INCLUDE_DIRECTORIES(${QWT_INCLUDE_DIRS})

FIND_PATH(QXT_INCLUDE_CORE
    NAMES Qxt QxtCore
	HINTS ${QT_INCLUDE_CORE})
FIND_PATH(QXT_INCLUDE_GUI
    NAMES Qxt QxtGui
	HINTS ${QT_INCLUDE_GUI})
SET(QXT_INCLUDE_DIRS ${QXT_INCLUDE_CORE} ${QXT_INCLUDE_GUI})
INCLUDE_DIRECTORIES(${QXT_INCLUDE_DIRS})
		
FIND_LIBRARY(QXT_LIBRARY_CORE
    NAMES QxtCore
	HINTS ${QT_LIBRARY_DIR})
FIND_LIBRARY(QXT_LIBRARY_GUI
    NAMES QxtGui
	HINTS ${QT_LIBRARY_DIR})	
	SET(QXT_LIBRARIES ${QXT_LIBRARY_CORE} ${QXT_LIBRARY_GUI})
    
#----------------------------------------------------------------------------
#SET(QWT_LIBRARIES "C:/Qwt-6.0.1/lib")
#SET(QWT_INCLUDE_DIR "C:/Qwt-6.0.1/include")
#INCLUDE_DIRECTORIES(${QWT_INCLUDE_DIR})
#LINK_LIBRARIES(${QWT_LIBRARIES})
#FILE(GLOB QWT_LIBRARIES "${QWT_LIBRARIES}/*.*")

#----------------------------------------------------------------------------
#SET(QXT_LIBRARIES "C:/Qxt/lib")
#SET(QXT_INCLUDE_DIR "C:/Qxt/include/QxtCore" /
#                     "C:/Qxt/include/QxtGui")
#INCLUDE_DIRECTORIES(${QXT_INCLUDE_DIR})
#LINK_DIRECTORIES(${QXT_LIBRARIES})
#FILE(GLOB QXT_LIBRARIES "${QXT_LIBRARIES}/*.*")




# use what QVTK built with
SET(QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "")
SET(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")
SET(QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "")


# Use the include path and library for Qt that is used by VTK.
INCLUDE_DIRECTORIES(
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)


# Set your files and resources here  
# SET(SimpleViewSrcs main.cxx View.cxx)
# SET(SimpleViewUI SimpleView.ui)
# SET(SimpleViewHeaders SimpleView.h)
# SET(SimpleViewResources Icons/icons.qrc)

QT4_WRAP_UI(UISrcs Viewer.ui)
QT4_WRAP_CPP(MOCSrcs Viewer.h)
# QT4_ADD_RESOURCES(ResourceSrcs Icons/icons.qrc)

# SOURCE_GROUP("Resources" FILES
  # ${SimpleViewUI}
  # ${SimpleViewResources}
  # ${EXE_ICON}
# )

# SOURCE_GROUP("Generated" FILES
  # ${UISrcs}
  # ${MOCSrcs}
  # ${ResourceSrcs}
  # ${RCS_SOURCES}
# )

#ADD_DEFINITIONS(-DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT)
#SET_SOURCE_FILES_PROPERTIES(main.cxx Viewer.cxx PROPERTIES
#                            OBJECT_DEPENDS "${UISrcs}")

# It's nice to have the ui in the windows project file...just double click on it
# and designer comes up on that ui file :)
# IF (${CMAKE_BUILD_TOOL} MATCHES "msdev")
  # SET (SimpleViewSrcs ${SimpleViewSrcs} ${SimpleViewUI})
# ENDIF (${CMAKE_BUILD_TOOL} MATCHES "msdev")
# IF (${CMAKE_BUILD_TOOL} MATCHES "devenv")
  # SET (SimpleViewSrcs ${SimpleViewSrcs} ${SimpleViewUI})
# ENDIF (${CMAKE_BUILD_TOOL} MATCHES "devenv")

SET( COMMON_DIRECTORIES  "G:/Common" )
#mark_as_advanced ( COMMON_DIRECTORIES )
#set( COMMON_DIRECTORIES "${COMMON_DIRECTORIES}")
INCLUDE_DIRECTORIES(${COMMON_DIRECTORIES})
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
FILE(GLOB COMMON_DIRECTORIES "${COMMON_DIRECTORIES}/*.*")

SET(COMMONSrcs itkImageToVTKImageFilter.txx itkImageToVTKImageFilter.h ${COMMON_DIRECTORIES})

ADD_EXECUTABLE( RegionGrowing main.cxx Viewer.cxx ${COMMONSrcs} ${UISrcs} ${MOCSrcs})

TARGET_LINK_LIBRARIES( RegionGrowing
  QVTK
  ${QT_LIBRARIES}
  ${ITK_LIBRARIES}
  vtkRendering
  vtkGraphics
  vtkIO
  vtkCommon
  vtkInfovis
  vtkViews
  ${CTK_LIBRARIES}
  ${QXT_LIBRARIES}
  ${QWT_LIBRARIES}
)



