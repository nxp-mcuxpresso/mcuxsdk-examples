mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "${board_root}/${board}/usb_examples/usb_device_dfu/bm"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "${board_root}/${board}/usb_examples/usb_device_dfu/bm/hardware_init.c"
    "${board_root}/${board}/usb_examples/usb_device_dfu/bm/pin_mux.c"
    "${board_root}/${board}/usb_examples/usb_device_dfu/bm/pin_mux.h"
    "${board_root}/${board}/usb_examples/usb_device_dfu/bm/usb_device_dfu_config.h"
    "middleware/usb/utility/usb_dsc_flash.c"
    "middleware/usb/utility/usb_flash.h"
)
