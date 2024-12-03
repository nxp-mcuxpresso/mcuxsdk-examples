
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/hello_world_virtual_com/usb_device_board_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/demo_apps/hello_world_virtual_com
)

mcux_add_mdk_configuration(
    TARGETS flexspi_nor_debug flexspi_nor_release
    LD "--legacyalign\
       --diag_suppress=3912"
)

mcux_add_macro(
  CC "-DUSB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
  TARGETS
    "hyperram_debug"
    "hyperram_release"
)

mcux_add_macro(
  CC "-DDATA_SECTION_IS_CACHEABLE=0"
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

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x2000\
        -Xlinker --defsym=__heap_size__=0x2000"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000\
        --config_def=__heap_size__=0x2000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\"\
        --predefine=\"-D__heap_size__=0x2000\""
)
