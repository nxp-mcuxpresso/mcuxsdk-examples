mcux_add_mdk_configuration(
  LD "--predefine=\"-D__heap_size__=0x1000\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x2000\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__usb_use_usbram__=1\""
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/usb/example/boards/${board}/usb_device_composite_hid_audio_unified_lpc_lite/bm/app.h
            middleware/usb/example/boards/${board}/usb_device_composite_hid_audio_unified_lpc_lite/bm/usb_audio_config.h
            middleware/usb/example/boards/${board}/usb_device_composite_hid_audio_unified_lpc_lite/bm/./hardware_init.c
            middleware/usb/example/boards/${board}/usb_device_composite_hid_audio_unified_lpc_lite/bm/./pin_mux.c
            middleware/usb/example/boards/${board}/usb_device_composite_hid_audio_unified_lpc_lite/bm/./pin_mux.h
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc_lite/bm/audio_unified.c
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc_lite/bm/audio_unified.h
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc_lite/bm/composite.h
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc_lite/bm/hid_keyboard.c
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc_lite/bm/hid_keyboard.h
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc_lite/bm/usb_device_descriptor.c
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc_lite/bm/usb_device_descriptor.h
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc_lite/bm/usb_device_audio.c
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc_lite/bm/usb_device_audio.h
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc_lite/bm/usb_device_hid.c
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc_lite/bm/usb_device_hid.h
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc_lite/bm/usb_device_ch9.c
            middleware/usb/example/usb_device_composite_hid_audio_unified_lpc_lite/bm/usb_device_ch9.h
            
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/usb/example/boards/${board}/usb_device_composite_hid_audio_unified_lpc_lite/bm
)


mcux_add_macro(
    CC "-DHAL_AUDIO_ISR_PRIORITY=0\
       -DUSB_STACK_BM\
       -DUSB_STACK_USE_DEDICATED_RAM=1"
)




