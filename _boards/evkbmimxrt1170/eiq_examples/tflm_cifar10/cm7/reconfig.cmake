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
mcux_add_mcux_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x80000&&region=BOARD_SDRAM&&location=Post Data \
     -Xlinker --defsym=__stack_size__=0x2000&&region=SRAM_DTC_cm7&&location=End"
)
