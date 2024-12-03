
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/driver_examples/flexcomm/i2s/dma_tdm
)
