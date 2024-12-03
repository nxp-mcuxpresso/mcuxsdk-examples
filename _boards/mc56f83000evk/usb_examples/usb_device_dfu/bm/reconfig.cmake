
mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/boards/${board}/usb_device_dfu/bm"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "examples/_boards/${board}/peripherals/peripherals.c"
    "examples/_boards/${board}/peripherals/peripherals.h"
    "middleware/usb/example/boards/${board}/usb_device_dfu/bm/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_dfu/bm/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_dfu/bm/pin_mux.h"
    "middleware/usb/example/boards/${board}/usb_device_dfu/bm/usb_device_dfu_config.h"
    "middleware/usb/utility/usb_dsc_flash.c"
    "middleware/usb/utility/usb_flash.h"
)
