

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/sdmmc/example/sdcard_fatfs/ffconf.h
            examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/sdmmc_examples/sdcard_fatfs
)
