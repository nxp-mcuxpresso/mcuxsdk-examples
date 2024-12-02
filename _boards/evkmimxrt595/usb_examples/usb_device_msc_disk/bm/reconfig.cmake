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
  INCLUDES "middleware/usb/example/boards/${board}/usb_device_msc_disk/bm"
)

mcux_add_macro(
  CC "-DUSB_STACK_USE_DEDICATED_RAM=1"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "examples/_boards/${board}/sdmmc_config.c"
    "examples/_boards/${board}/sdmmc_config.h"
    "middleware/usb/example/boards/${board}/usb_device_msc_disk/bm/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_msc_disk/bm/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_msc_disk/bm/pin_mux.h"
    "middleware/usb/example/boards/${board}/usb_device_msc_disk/bm/usb_device_msc_config.h"
    "middleware/usb/utility/usb_disk_adapter.h"
    "middleware/usb/utility/usb_mmc_adapter.c"
)
