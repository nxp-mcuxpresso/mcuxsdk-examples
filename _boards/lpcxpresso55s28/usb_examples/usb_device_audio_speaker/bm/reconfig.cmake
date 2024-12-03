mcux_add_armgcc_configuration(
  CC "-mno-unaligned-access"
  TARGETS "release"
)

mcux_add_iar_configuration(
  CC "--no_unaligned_access"
  TARGETS "release"
)

mcux_add_mdk_configuration(
  CC "-mno-unaligned-access"
)

mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x1000"
)

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x2000"
)

mcux_add_iar_configuration(
  LD "--config_def=__usb_use_usbram__=1"
)

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
  INCLUDES "middleware/usb/example/boards/${board}/usb_device_audio_speaker/bm"
)

mcux_add_macro(
  CC "-DUSING_SAI -DUSB_STACK_USE_DEDICATED_RAM=1"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/boards/${board}/usb_device_audio_speaker/bm/app.h"
    "middleware/usb/example/boards/${board}/usb_device_audio_speaker/bm/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_audio_speaker/bm/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_audio_speaker/bm/pin_mux.h"
    "middleware/usb/example/boards/${board}/usb_device_audio_speaker/bm/usb_audio_config.h"
)
