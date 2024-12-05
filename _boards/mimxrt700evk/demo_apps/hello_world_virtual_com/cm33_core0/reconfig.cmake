
mcux_add_macro(
  CC "-DUSB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/hello_world_virtual_com/usb_device_board_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/hello_world_virtual_com/
    INCLUDES ${board_root}/${board}/demo_apps/hello_world_virtual_com/${core_id}
)

mcux_add_macro(
    CC "-DDEBUG_CONSOLE_IO_USBCDC=1\
       -DSERIAL_PORT_TYPE_USBCDC=1\
       -DPRINTF_FLOAT_ENABLE=0\
       -DSCANF_FLOAT_ENABLE=0\
       -DPRINTF_ADVANCED_ENABLE=0\
       -DSCANF_ADVANCED_ENABLE=0\
       -DUSB_STACK_BM\
       -DBOARD_USE_VIRTUALCOM=1"
)
