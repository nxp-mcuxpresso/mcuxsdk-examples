

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/sdmmc/example/sdcard_fatfs_freertos/ffconf.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
)



# Add additional configuration
mcux_add_macro(
    CC "-DBOARD_SDMMC_NEED_MANUAL_TUNING\
       -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING"
)
