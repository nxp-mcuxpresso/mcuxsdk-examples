

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/sdmmc/example/sdcard_fatfs/ffconf.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
)

mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
