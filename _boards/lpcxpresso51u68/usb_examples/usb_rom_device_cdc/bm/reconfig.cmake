
mcux_add_source(
    BASE_PATH "${SdkRootDirPath}"
    SOURCES "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/hardware_init.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/pin_mux.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/pin_mux.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd_5411x.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/romapi_5411x.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/cdc_desc.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/cdc_main.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/app_usbd_cfg.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/cdc_vcom.c"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/cdc_vcom.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/error.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd_cdcuser.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd_dfuuser.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd_msc.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/iap.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd_core.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd_hid.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd_mscuser.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/lpc_types.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd_adc.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd_desc.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd_hiduser.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd_rom_api.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd_cdc.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd_dfu.h"
            "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc/usbd_hw.h"
)

mcux_add_include(
    BASE_PATH "${SdkRootDirPath}"
    INCLUDES "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm"
             "middleware/usb/example/boards/${board}/usb_rom_device_cdc/bm/inc"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000\
        --config_def=__heap_size__=0x1000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\"\
        --predefine=\"-D__heap_size__=0x1000\""
)
