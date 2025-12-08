mcux_add_xtensa_configuration(
  CC "-std=c99"
  CX "-stdlib=libc++ \
    -std=c++17"
)

mcux_remove_configuration(
  CC "-std=gnu99"
  CX "-std=c++11"
)

mcux_add_include(
  INCLUDES .
  )
mcux_add_source(
  SOURCES model_cifarnet_ops_npu.cpp
  model_data.h
  )
