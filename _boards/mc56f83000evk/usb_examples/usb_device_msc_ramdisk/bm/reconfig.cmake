
mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "${board_root}/${board}/usb_examples/usb_device_msc_ramdisk/bm"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "${board_root}/${board}/peripherals/peripherals.c"
    "${board_root}/${board}/peripherals/peripherals.h"
    "${board_root}/${board}/usb_examples/usb_device_msc_ramdisk/bm/hardware_init.c"
    "${board_root}/${board}/usb_examples/usb_device_msc_ramdisk/bm/pin_mux.c"
    "${board_root}/${board}/usb_examples/usb_device_msc_ramdisk/bm/pin_mux.h"
)
