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
  INCLUDES "middleware/usb/example/boards/${board}/usb_device_composite_hid_audio/freertos"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/boards/${board}/usb_device_composite_hid_audio/freertos/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_composite_hid_audio/freertos/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_composite_hid_audio/freertos/pin_mux.h"
    "middleware/usb/example/boards/${board}/usb_device_composite_hid_audio/freertos/usb_audio_config.h"
    "middleware/usb/example/usb_device_composite_hid_audio/freertos/audio_data.c"
)
