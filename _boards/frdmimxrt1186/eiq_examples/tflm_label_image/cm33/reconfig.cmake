mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES examples/eiq_examples/tflm_cifar10/pcq/
  )
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/eiq_examples/tflm_label_image/pcq/model_data.h
  examples/eiq_examples/tflm_label_image/pcq/model_mobilenet_ops_micro.cpp

  )
mcux_remove_mdk_configuration(
  CC "-fshort-wchar"
  CX "-fshort-wchar"
  )

mcux_remove_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
            flexspi_nor_hyperram_debug  flexspi_nor_hyperram_release
    CC "-DXIP_BOOT_HEADER_ENABLE=0"
)

mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
            flexspi_nor_hyperram_debug  flexspi_nor_hyperram_release
    LD "--predefine=-DXIP_BOOT_HEADER_ENABLE=0"
)

mcux_add_macro(
    CC "-DUSE_HYPERRAM=1"
)

mcux_add_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
            flexspi_nor_hyperram_debug  flexspi_nor_hyperram_release
    CC "-DXIP_BOOT_HEADER_ENABLE=1"
)

mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x8000\
  --config_def=__stack_size__=0x2000"
  )
mcux_add_mdk_configuration(
  LD "--predefine=-D__heap_size__=0x8000\
      --predefine=-DXIP_BOOT_HEADER_ENABLE=1\
      --predefine=-D__stack_size__=0x2000"
  )
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x8000\
  -Xlinker --defsym=__stack_size__=0x2000"
  )

