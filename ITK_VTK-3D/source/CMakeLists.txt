#--------------------------------------------------------
#  By Huoling Luo (ÂÞ»ðÁé) & Xiaodong Zhang (ÕÅÏþ¶«) 
# 
#  ÂÞ»ðÁé QQ: 348774226 & MSN: shuiling119@hotmail.com
#  ÕÅÏþ¶« QQ: 285249802 & Email: zhangxd530@163.com
# 
#--------------------------------------------------------

cmake_minimum_required( VERSION 2.8 )
project( itk_vtk-3d )

#----------------------------------------------------------------------------
find_package (ITK)
if (ITK_FOUND)
	include ( ${ITK_USE_FILE} )
else (ITK_FOUND)
	message (FATAL_ERROR "Cannot build without ITK, Please set ITK_DIR.")
endif (ITK_FOUND)

#----------------------------------------------------------------------------
find_package (VTK)
if (VTK_FOUND)
	include ( ${VTK_USE_FILE} )
else (VTK_FOUND)
	message (FATAL_ERROR "Cannot build without VTK, Please set VTK_DIR.")
endif (VTK_FOUND)

#----------------------------------------------------------------------------
add_executable( ${PROJECT_NAME} main.cpp itkImageToVTKImageFilter.txx )

target_link_libraries( ${PROJECT_NAME} 
  ${ITK_LIBRARIES}
  vtkRendering 
  vtkCommon 
  )

# -------------------------------------------------------------------------
# Construct a list of paths containing runtime 
# directories for project applications on Windows
# set(PROJECT_RUNTIME_PATH 
    # "${VTK_LIBRARY_DIRS}/@VS_BUILD_TYPE@;${ITK_LIBRARY_DIRS}/@VS_BUILD_TYPE@;${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/@VS_BUILD_TYPE@"
    # )

# include(CreateWindowsBatchScript.cmake)

# If we are under Windows, create two batch files which correctly
# set up the environment for the application and for Visual Studio
# if(WIN32)
  # set(VS_SOLUTION_FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.sln")
  # foreach(VS_BUILD_TYPE debug release)
    # CreateWindowsBatchScript("${CMAKE_SOURCE_DIR}/StartVS.bat.in"
      # ${PROJECT_BINARY_DIR}/StartVS_${VS_BUILD_TYPE}.bat
      # ${VS_BUILD_TYPE})
  # endforeach()
# endif(WIN32)



