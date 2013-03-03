#--------------------------------------------------------
#  By Huoling Luo (ÂÞ»ðÁé) & Xiaodong Zhang (ÕÅÏþ¶«) 
# 
#  ÂÞ»ðÁé QQ: 348774226 & MSN: shuiling119@hotmail.com
#  ÕÅÏþ¶« QQ: 285249802 & Email: zhangxd530@163.com
# 
#--------------------------------------------------------

function(CreateWindowsBatchScript in out build_type)
  if(GDCM_DIR)
    set(GDCM_BIN_DIR "${GDCM_DIR}/bin/${build_type}")
  else()
    set(GDCM_BIN_DIR)
  endif()
  
  if(OpenCV_DIR)
    set(OpenCV_BIN_DIR "${OpenCV_DIR}/bin/${build_type}")
  else()
    set(OpenCV_BIN_DIR)
  endif()

  if(VTK_DIR)
    set(VTK_BIN_DIR "${VTK_DIR}/bin/${build_type}")
  else()
    set(VTK_BIN_DIR)
  endif()

  if(ITK_DIR)
    set(ITK_BIN_DIR "${ITK_DIR}/bin/${build_type}")
  else()
    set(ITK_BIN_DIR)
  endif()
  
  set(VS_BUILD_TYPE ${build_type})
  configure_file(${in} ${out} @ONLY)
  # substitute again
  configure_file(${out} ${out} @ONLY)
endfunction()