
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
)

mcux_add_mdk_configuration(
    CC "-Wno-gcc-compat"
)
