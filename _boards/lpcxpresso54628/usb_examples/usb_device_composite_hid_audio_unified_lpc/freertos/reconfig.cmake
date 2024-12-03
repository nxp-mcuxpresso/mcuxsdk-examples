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
    SOURCES middleware/usb/example/boards/${board}/usb_device_composite_hid_audio_unified_lpc/freertos/app.h
            middleware/usb/example/boards/${board}/usb_device_composite_hid_audio_unified_lpc/freertos/usb_audio_config.h
            middleware/usb/example/boards/${board}/usb_device_composite_hid_audio_unified_lpc/freertos/./hardware_init.c
            middleware/usb/example/boards/${board}/usb_device_composite_hid_audio_unified_lpc/freertos/./pin_mux.c
            middleware/usb/example/boards/${board}/usb_device_composite_hid_audio_unified_lpc/freertos/./pin_mux.h
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc/freertos/audio_unified.c
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc/freertos/audio_unified.h
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc/freertos/composite.h
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc/freertos/hid_keyboard.c
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc/freertos/hid_keyboard.h
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc/freertos/usb_device_descriptor.c
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc/freertos/usb_device_descriptor.h
            


)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/usb/example/boards/${board}/usb_device_composite_hid_audio_unified_lpc/freertos
             )


mcux_add_macro(
    CC "-DHAL_AUDIO_ISR_PRIORITY=0\
       -DUSB_STACK_FREERTOS\
\
       -DUSB_STACK_USE_DEDICATED_RAM=1"
)




