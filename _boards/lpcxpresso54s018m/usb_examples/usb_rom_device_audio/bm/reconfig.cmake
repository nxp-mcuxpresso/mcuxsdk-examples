
mcux_add_source(
    BASE_PATH "${SdkRootDirPath}"
    SOURCES "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/hardware_init.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/pin_mux.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/pin_mux.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd_5460x.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/romapi_5460x.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/Power_Tasks.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/audio_codec.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/audio_main.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/delay.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/Power_Tasks.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/audio_codec.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/audio_usbd.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/delay.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/i2s_if.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/main.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/sine_file.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/app_usbd_cfg.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/audio_desc.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/audio_usbd.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/i2s_if.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/pll_setup.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/sine_file.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/error.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd_cdcuser.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd_dfuuser.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd_msc.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/iap.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd_core.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd_hid.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd_mscuser.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/lpc_types.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd_adc.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd_desc.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd_hiduser.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd_rom_api.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd_cdc.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd_dfu.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc/usbd_hw.h"
)

mcux_add_include(
    BASE_PATH "${SdkRootDirPath}"
    INCLUDES "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm"
             "middleware/usb/example/boards/${board}/usb_rom_device_audio/bm/inc"
)

mcux_add_macro(
    CC "-DUSB_STACK_USE_DEDICATED_RAM=1"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\"\
        --predefine=\"-D__heap_size__=0x1000\""
)
