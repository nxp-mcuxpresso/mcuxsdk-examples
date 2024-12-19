
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/hello_world_virtual_com/usb_device_board_config.h
            examples/_boards/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
)

mcux_add_armgcc_configuration(
    CC "-mno-unaligned-access"
)
mcux_add_iar_configuration(
    CC "--no_unaligned_access"
)

mcux_add_mdk_configuration(
  CC "-mno-unaligned-access"
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
)
