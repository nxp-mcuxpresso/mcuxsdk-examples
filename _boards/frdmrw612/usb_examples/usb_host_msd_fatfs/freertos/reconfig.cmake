mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__stack_size__=0x1000"
)

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x1000"
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x1000\""
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "${board_root}/${board}/usb_examples/usb_host_msd_fatfs/freertos"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "${board_root}/${board}/usb_examples/usb_host_msd_fatfs/freertos/hardware_init.c"
    "${board_root}/${board}/usb_examples/usb_host_msd_fatfs/freertos/pin_mux.c"
    "${board_root}/${board}/usb_examples/usb_host_msd_fatfs/freertos/pin_mux.h"
)
