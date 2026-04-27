
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x2000"
)

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__stack_size__=0x2000"
)

mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x2000"
)

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x2000"
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__heap_size__=0x2000\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x2000\""
)

mcux_add_iar_configuration(
  LD "--config_def=__heap_noncacheable__=1"
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__heap_noncacheable__=1\""
)

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_noncacheable__=1"
)

mcux_add_macro(
  CC "-DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
)
mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES
    "${board_root}/${board}/usb_examples/usb_host_printer_plain_text/bm"
    "${board_root}/${board}/usb_examples/usb_host_printer_plain_text/bm"
    "${board_root}/${board}/usb_examples/usb_host_printer_plain_text/bm/${core_id}"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "${board_root}/${board}/usb_examples/usb_host_printer_plain_text/bm/${core_id}/hardware_init.c"

)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES
)
