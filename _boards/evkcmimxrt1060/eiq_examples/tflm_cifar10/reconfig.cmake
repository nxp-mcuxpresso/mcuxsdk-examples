mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES examples/eiq_examples/tflm_cifar10/pcq/
  )
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/eiq_examples/tflm_cifar10/pcq/model_data.h
  examples/eiq_examples/tflm_cifar10/pcq/model_cifarnet_ops_micro.cpp
  )
mcux_remove_mdk_configuration(
  CC "-fshort-wchar"
  CX "-fshort-wchar"
  )
mcux_add_macro(
  CC "XIP_BOOT_HEADER_DCD_ENABLE=1 \
      SKIP_SYSCLK_INIT=1 \
      USE_SDRAM=1"
)

