
mcux_add_source(
    BASE_PATH "${SdkRootDirPath}"
    SOURCES "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/hardware_init.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/pin_mux.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/pin_mux.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd_5460x.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/romapi_5460x.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/Power_Tasks.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/audio_codec.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/audio_main.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/delay.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/Power_Tasks.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/audio_codec.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/audio_usbd.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/delay.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/i2s_if.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/main.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/sine_file.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/app_usbd_cfg.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/audio_desc.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/audio_usbd.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/i2s_if.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/pll_setup.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/sine_file.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/error.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd_cdcuser.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd_dfuuser.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd_msc.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/iap.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd_core.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd_hid.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd_mscuser.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/lpc_types.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd_adc.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd_desc.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd_hiduser.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd_rom_api.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd_cdc.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd_dfu.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc/usbd_hw.h"
)

mcux_add_include(
    BASE_PATH "${SdkRootDirPath}"
    INCLUDES "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm"
             "${board_root}/${board}/usb_examples/usb_rom_device_audio/bm/inc"
)

mcux_add_macro(
    CC "-DUSB_STACK_USE_DEDICATED_RAM=1"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\"\
        --predefine=\"-D__heap_size__=0x1000\""
)
