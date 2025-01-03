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
    SOURCES ${board_root}/${board}/usb_examples/usb_device_audio_speaker_lpc_lite/bm/app.h
            ${board_root}/${board}/usb_examples/usb_device_audio_speaker_lpc_lite/bm/usb_audio_config.h
            ${board_root}/${board}/usb_examples/usb_device_audio_speaker_lpc_lite/bm/./hardware_init.c
            ${board_root}/${board}/usb_examples/usb_device_audio_speaker_lpc_lite/bm/./pin_mux.c
            ${board_root}/${board}/usb_examples/usb_device_audio_speaker_lpc_lite/bm/./pin_mux.h
            examples/usb_examples/usb_device_audio_speaker_lpc_lite/bm/audio_speaker.h
            examples/usb_examples/usb_device_audio_speaker_lpc_lite/bm/usb_device_descriptor.c
            examples/usb_examples/usb_device_audio_speaker_lpc_lite/bm/usb_device_descriptor.h
            examples/usb_examples/usb_device_audio_speaker_lpc_lite/bm/usb_device_audio.c
            examples/usb_examples/usb_device_audio_speaker_lpc_lite/bm/usb_device_audio.h
            examples/usb_examples/usb_device_audio_speaker_lpc_lite/bm/usb_device_ch9.c
            examples/usb_examples/usb_device_audio_speaker_lpc_lite/bm/usb_device_ch9.h
            
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/usb_examples/usb_device_audio_speaker_lpc_lite/bm
)


mcux_add_macro(
    CC "-DHAL_AUDIO_ISR_PRIORITY=0\
       -DUSB_STACK_BM\
       -DUSB_STACK_USE_DEDICATED_RAM=1"
)




