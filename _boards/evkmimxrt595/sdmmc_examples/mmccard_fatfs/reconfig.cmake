

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/sdmmc/example/mmccard_fatfs/ffconf.h
            examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
)
