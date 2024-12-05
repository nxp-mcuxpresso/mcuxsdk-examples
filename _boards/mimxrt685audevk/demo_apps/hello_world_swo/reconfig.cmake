
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
)

mcux_add_macro(
    CC "-DSERIAL_PORT_TYPE_SWO=1"
)
