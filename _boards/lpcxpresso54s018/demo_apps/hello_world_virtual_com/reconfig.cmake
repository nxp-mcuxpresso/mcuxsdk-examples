
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/hello_world_virtual_com/usb_device_board_config.h
)

mcux_add_macro(
    CC "-DUSB_STACK_USE_DEDICATED_RAM=1\
       -DSERIAL_PORT_TYPE_USBCDC=1\
       -DPRINTF_FLOAT_ENABLE=0\
       -DSCANF_FLOAT_ENABLE=0\
       -DPRINTF_ADVANCED_ENABLE=0\
       -DSCANF_ADVANCED_ENABLE=0\
       -DUSB_STACK_BM\
       -DBOARD_USE_VIRTUALCOM=1"
    AS "-DIMG_BAUDRATE=96000000"
)
