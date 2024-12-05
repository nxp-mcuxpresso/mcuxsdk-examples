
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/cmsis_driver_examples/lpc_i2c/dma_b2b_transfer/slave
)
