mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__stack_size__=0x1000"
)

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x1000"
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x1000\""
)

mcux_add_macro(
  CC "-DUSB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES
    "examples/_boards/${board}/usb_examples/usb_device_audio_generator/freertos"
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/freertos"
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/freertos/${core_id}"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/freertos/${core_id}/usb_audio_config.h"
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/freertos/${core_id}/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/freertos/${core_id}/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_audio_generator/freertos/${core_id}/pin_mux.h"
    "middleware/usb/example/usb_device_audio_generator/freertos/audio_data.c"
)
