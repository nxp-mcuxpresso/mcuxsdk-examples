mcux_add_mdk_configuration(
  LD "--predefine=\"-D__heap_size__=0x1000\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x2000\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__usb_use_usbram__=1\""
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/boards/${board}/usb_device_audio_generator/bm"
)

mcux_add_macro(
  CC "-DUSB_STACK_USE_DEDICATED_RAM=1"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/bm/dmic_config.h"
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/bm/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/bm/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/bm/pin_mux.h"
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/bm/usb_audio_config.h"
    "middleware/usb/example/usb_device_audio_generator/bm/audio_data_dmic.c"
    "middleware/usb/example/usb_device_audio_generator/bm/audio_data_dmic.h"
)
