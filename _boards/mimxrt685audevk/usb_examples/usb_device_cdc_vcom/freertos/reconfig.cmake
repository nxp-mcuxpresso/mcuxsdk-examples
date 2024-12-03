mcux_add_armgcc_configuration(
  CC "-mno-unaligned-access"
  TARGETS
    "flash_release"
    "release"
)

mcux_add_iar_configuration(
  CC "--no_unaligned_access"
  TARGETS
    "flash_release"
    "release"
)

mcux_add_mdk_configuration(
  CC "-mno-unaligned-access"
)

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
  INCLUDES "middleware/usb/example/boards/${board}/usb_device_cdc_vcom/freertos"
)

mcux_add_macro(
  CC "-DUSB_STACK_USE_DEDICATED_RAM=1"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/boards/${board}/usb_device_cdc_vcom/freertos/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_cdc_vcom/freertos/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_cdc_vcom/freertos/pin_mux.h"
)
