mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x1000"
)

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__stack_size__=0x1000"
)

mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x1000"
)

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x1000"
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__heap_size__=0x1000\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x1000\""
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES
    "${board_root}/${board}/usb_examples/usb_device_composite_hid_audio/bm"
    "${board_root}/${board}/usb_examples/usb_device_composite_hid_audio/bm"
    "${board_root}/${board}/usb_examples/usb_device_composite_hid_audio/bm/${core_id}"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "${board_root}/${board}/usb_examples/usb_device_composite_hid_audio/bm/${core_id}/usb_audio_config.h"
    "${board_root}/${board}/usb_examples/usb_device_composite_hid_audio/bm/hardware_init.c"
    "${board_root}/${board}/usb_examples/usb_device_composite_hid_audio/bm/pin_mux.c"
    "${board_root}/${board}/usb_examples/usb_device_composite_hid_audio/bm/pin_mux.h"
    "examples/usb_examples/usb_device_composite_hid_audio/bm/audio_data.c"
)
