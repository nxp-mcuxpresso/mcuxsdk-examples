
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/hello_world_virtual_com/usb_device_board_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/hello_world_virtual_com
)

mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=0\
       -DSCANF_FLOAT_ENABLE=0\
       -DPRINTF_ADVANCED_ENABLE=0\
       -DSCANF_ADVANCED_ENABLE=0\
       -DUSB_STACK_BM\
       -DBOARD_USE_VIRTUALCOM=1"
)
