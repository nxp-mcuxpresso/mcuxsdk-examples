
mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/boards/${board}/usb_device_audio_generator/bm"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "examples/_boards/${board}/peripherals/peripherals.c"
    "examples/_boards/${board}/peripherals/peripherals.h"
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/bm/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/bm/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/bm/pin_mux.h"
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/bm/usb_audio_config.h"
    "middleware/usb/example/usb_device_audio_generator/bm/audio_data.c"
)
