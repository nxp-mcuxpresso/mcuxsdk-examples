
mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/boards/${board}/usb_device_msc_ramdisk/bm"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "examples/_boards/${board}/peripherals/peripherals.c"
    "examples/_boards/${board}/peripherals/peripherals.h"
    "middleware/usb/example/boards/${board}/usb_device_msc_ramdisk/bm/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_msc_ramdisk/bm/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_msc_ramdisk/bm/pin_mux.h"
)
