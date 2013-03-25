cmake_minimum_required(VERSION 2.8)

PROJECT(Viewer)

if( NOT CMAKE_BUILD_TYPE )

  set( CMAKE_BUILD_TYPE Debug CACHE STRING
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
QT4_ADD_RESOURCES(ResourceSrcs Icons/icons.qrc)

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

set ( COMMON_DIRECTORIES  "G:/Common" )
#mark_as_advanced ( COMMON_DIRECTORIES )
#set( COMMON_DIRECTORIES "${COMMON_DIRECTORIES}")
include_directories (${COMMON_DIRECTORIES})
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
file (GLOB COMMON_DIRECTORIES "${COMMON_DIRECTORIES}/*.*")

SET(COMMONSrcs itkImageToVTKImageFilter.txx itkImageToVTKImageFilter.h ${COMMON_DIRECTORIES})

ADD_EXECUTABLE( Viewer main.cxx Viewer.cxx ${COMMONSrcs} ${UISrcs} ${MOCSrcs} ${ResourceSrcs})

TARGET_LINK_LIBRARIES( Viewer
  QVTK
  ${QT_LIBRARIES}
  ${ITK_LIBRARIES}
  vtkRendering
  vtkGraphics
  vtkIO
  vtkCommon
  vtkInfovis
  vtkViews
)



