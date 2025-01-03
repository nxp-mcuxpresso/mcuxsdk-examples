
mcux_add_source(
    BASE_PATH "${SdkRootDirPath}"
    SOURCES "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/hardware_init.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/pin_mux.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/pin_mux.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd_5460x.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/romapi_5460x.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/hid_desc.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/hid_generic.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/hid_main.c"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/hid_generic.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/error.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd_cdcuser.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd_dfuuser.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd_msc.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/iap.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/app_usbd_cfg.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd_core.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd_hid.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd_mscuser.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/lpc_types.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd_adc.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd_desc.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd_hiduser.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd_rom_api.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd_cdc.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd_dfu.h"
            "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc/usbd_hw.h"
)

mcux_add_include(
    BASE_PATH "${SdkRootDirPath}"
    INCLUDES "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm"
             "${board_root}/${board}/usb_examples/usb_rom_device_hid_generic/bm/inc"
)

mcux_add_macro(
    CC "-DUSB_STACK_USE_DEDICATED_RAM=1"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\"\
        --predefine=\"-D__heap_size__=0x1000\""
)
