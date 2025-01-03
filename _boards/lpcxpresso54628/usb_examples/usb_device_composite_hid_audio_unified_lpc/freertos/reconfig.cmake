mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x100"
)

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x2000"
)

mcux_add_iar_configuration(
  LD "--config_def=__usb_use_usbram__=1"
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__heap_size__=0x100\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x2000\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__usb_use_usbram__=1\""
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/usb_examples/usb_device_composite_hid_audio_unified_lpc/freertos/app.h
            ${board_root}/${board}/usb_examples/usb_device_composite_hid_audio_unified_lpc/freertos/usb_audio_config.h
            ${board_root}/${board}/usb_examples/usb_device_composite_hid_audio_unified_lpc/freertos/./hardware_init.c
            ${board_root}/${board}/usb_examples/usb_device_composite_hid_audio_unified_lpc/freertos/./pin_mux.c
            ${board_root}/${board}/usb_examples/usb_device_composite_hid_audio_unified_lpc/freertos/./pin_mux.h
            examples/usb_examples/usb_device_composite_hid_audio_unified_lpc/freertos/audio_unified.c
            examples/usb_examples/usb_device_composite_hid_audio_unified_lpc/freertos/audio_unified.h
            examples/usb_examples/usb_device_composite_hid_audio_unified_lpc/freertos/composite.h
            examples/usb_examples/usb_device_composite_hid_audio_unified_lpc/freertos/hid_keyboard.c
            examples/usb_examples/usb_device_composite_hid_audio_unified_lpc/freertos/hid_keyboard.h
            examples/usb_examples/usb_device_composite_hid_audio_unified_lpc/freertos/usb_device_descriptor.c
            examples/usb_examples/usb_device_composite_hid_audio_unified_lpc/freertos/usb_device_descriptor.h
            


)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/usb_examples/usb_device_composite_hid_audio_unified_lpc/freertos
             )


mcux_add_macro(
    CC "-DHAL_AUDIO_ISR_PRIORITY=0\
       -DUSB_STACK_FREERTOS\
\
       -DUSB_STACK_USE_DEDICATED_RAM=1"
)




