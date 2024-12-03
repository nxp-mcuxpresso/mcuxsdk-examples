
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexcomm/i2s/music.h
            examples/_boards/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES platform/drivers/flexcomm/i2s/example
             examples/_boards/${board}/driver_examples/i2s/ping_pong_buffer
             examples/driver_examples/flexcomm/i2s
)
