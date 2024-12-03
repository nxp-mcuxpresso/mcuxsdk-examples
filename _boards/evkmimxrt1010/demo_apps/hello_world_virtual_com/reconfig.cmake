
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/hello_world_virtual_com/usb_device_board_config.h
)

mcux_add_mdk_configuration(
    TARGETS flexspi_nor_debug flexspi_nor_release
    LD "--legacyalign\
       --diag_suppress=3912"
)

mcux_add_macro(
  CC "-DDATA_SECTION_IS_CACHEABLE=0"
  TARGETS
    "debug"
    "release"
    "flexspi_nor_debug"
    "flexspi_nor_release"
  TOOLCHAINS
    "armgcc"
    "iar"
    "mdk"
)

mcux_add_macro(
    CC "-DUSB_STACK_BM\
       -DBOARD_USE_VIRTUALCOM=1\
       -DDEBUG_CONSOLE_IO_USBCDC=1\
       -DSERIAL_PORT_TYPE_USBCDC=1\
       -DPRINTF_FLOAT_ENABLE=0\
       -DSCANF_FLOAT_ENABLE=0\
       -DPRINTF_ADVANCED_ENABLE=0\
       -DSCANF_ADVANCED_ENABLE=0"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS debug
    CC "-DDEBUG_CONSOLE_ASSERT_DISABLE=1"
)
