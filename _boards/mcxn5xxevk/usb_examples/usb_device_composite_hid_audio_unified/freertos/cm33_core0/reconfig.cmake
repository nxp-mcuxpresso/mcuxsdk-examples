mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x100"
)

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__stack_size__=0x2000"
)

mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x100"
)

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x2000"
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__heap_size__=0x100\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x2000\""
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "${board_root}/${board}/usb_examples/usb_device_composite_hid_audio_unified/freertos"
)

mcux_add_macro(
  CC "-DUSB_DEVICE_AUDIO_SPEAKER_DEDICATED_INTERVAL=1"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "${board_root}/${board}/usb_examples/usb_device_composite_hid_audio_unified/freertos/app.h"
    "${board_root}/${board}/usb_examples/usb_device_composite_hid_audio_unified/freertos/hardware_init.c"
    "${board_root}/${board}/usb_examples/usb_device_composite_hid_audio_unified/freertos/pin_mux.c"
    "${board_root}/${board}/usb_examples/usb_device_composite_hid_audio_unified/freertos/pin_mux.h"
    "${board_root}/${board}/usb_examples/usb_device_composite_hid_audio_unified/freertos/usb_audio_config.h"
)
