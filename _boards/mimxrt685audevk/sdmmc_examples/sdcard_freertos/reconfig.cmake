

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
)
