mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/sdmmc/example/sdcard_fatfs_freertos/ffconf.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/sdmmc_examples/sdcard_fatfs_freertos
)

mcux_add_macro(
    CC "-DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING\
       -DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_PCAL6524=1\
       -DBOARD_USE_PCA9555=1"
)
