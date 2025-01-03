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
  INCLUDES "${board_root}/${board}/usb_examples/usb_device_composite_cdc_msc_disk/freertos"
)

mcux_add_macro(
  CC "-DUSB_STACK_USE_DEDICATED_RAM=1"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "${board_root}/${board}/sdmmc_config.c"
    "${board_root}/${board}/sdmmc_config.h"
    "${board_root}/${board}/usb_examples/usb_device_composite_cdc_msc_disk/freertos/hardware_init.c"
    "${board_root}/${board}/usb_examples/usb_device_composite_cdc_msc_disk/freertos/pin_mux.c"
    "${board_root}/${board}/usb_examples/usb_device_composite_cdc_msc_disk/freertos/pin_mux.h"
    "${board_root}/${board}/usb_examples/usb_device_composite_cdc_msc_disk/freertos/usb_device_msc_config.h"
    "middleware/usb/utility/usb_disk_adapter.h"
    "middleware/usb/utility/usb_sdcard_adapter.c"
)
