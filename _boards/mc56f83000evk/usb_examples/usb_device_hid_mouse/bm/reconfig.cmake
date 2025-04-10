mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "${board_root}/${board}/usb_examples/usb_device_hid_mouse/bm"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "${board_root}/${board}/usb_examples/usb_device_hid_mouse/bm/hardware_init.c"
    "${board_root}/${board}/usb_examples/usb_device_hid_mouse/bm/pin_mux.c"
    "${board_root}/${board}/usb_examples/usb_device_hid_mouse/bm/pin_mux.h"
)
