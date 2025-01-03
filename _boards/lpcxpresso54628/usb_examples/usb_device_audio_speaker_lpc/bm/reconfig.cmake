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


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/usb_examples/usb_device_audio_speaker_lpc/bm/app.h
            ${board_root}/${board}/usb_examples/usb_device_audio_speaker_lpc/bm/usb_audio_config.h
            ${board_root}/${board}/usb_examples/usb_device_audio_speaker_lpc/bm/./hardware_init.c
            ${board_root}/${board}/usb_examples/usb_device_audio_speaker_lpc/bm/./pin_mux.c
            ${board_root}/${board}/usb_examples/usb_device_audio_speaker_lpc/bm/./pin_mux.h
            examples/usb_examples/usb_device_audio_speaker_lpc/bm/audio_speaker.h
            examples/usb_examples/usb_device_audio_speaker_lpc/bm/usb_device_descriptor.c
            examples/usb_examples/usb_device_audio_speaker_lpc/bm/usb_device_descriptor.h
            
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/usb_examples/usb_device_audio_speaker_lpc/bm
)


mcux_add_macro(
    CC "-DHAL_AUDIO_ISR_PRIORITY=0\
       -DUSB_STACK_BM\
       -DUSB_STACK_USE_DEDICATED_RAM=1"
)




