

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/sdmmc_examples/sdcard_polling
)
