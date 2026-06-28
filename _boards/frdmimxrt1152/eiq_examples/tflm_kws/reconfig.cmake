mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES ${board_root}/${board}/eiq_examples/tflm_kws/cpu/
  )

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES ${board_root}/${board}/eiq_examples/tflm_kws/cpu/model_data.h
  ${board_root}/${board}/eiq_examples/tflm_kws/cpu/model_ds_cnn_ops_micro.cpp
  )

mcux_remove_mdk_configuration(
  CC "-fshort-wchar"
  CX "-fshort-wchar"
  )
mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x8000\
      --config_def=__stack_size__=0x2000"
  )
mcux_add_mdk_configuration(
  LD "--predefine=-D__heap_size__=0x8000\
     --predefine=-D__stack_size__=0x2000"
  )
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x8000\
      -Xlinker --defsym=__stack_size__=0x2000"
  )
