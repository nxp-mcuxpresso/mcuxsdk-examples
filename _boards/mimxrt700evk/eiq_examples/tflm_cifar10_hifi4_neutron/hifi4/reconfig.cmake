mcux_add_xtensa_configuration(
  CC "-std=c99"
  CX "-stdlib=libc++ \
    -DTENSORARENA_NONCACHE=1\
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

mcux_add_xtensa_configuration(
    TARGETS release
    LD "-mlsp=${SdkRootDirPath}/${board_root}/mimxrt700evk/eiq_examples/tflm_cifar10_hifi4_neutron/linker/hifi4/min-rt"
)
