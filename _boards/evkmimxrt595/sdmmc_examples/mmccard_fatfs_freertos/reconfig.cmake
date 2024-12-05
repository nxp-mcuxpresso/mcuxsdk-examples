

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/sdmmc/example/mmccard_fatfs_freertos/ffconf.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
)



# Add additional configuration
mcux_add_macro(
    CC "-DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING"
)
