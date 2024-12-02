
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
)
mcux_add_macro(
    TARGETS debug flash_debug flash_release release
    CC "-DDMIC_FLASH_TARGET"
)
