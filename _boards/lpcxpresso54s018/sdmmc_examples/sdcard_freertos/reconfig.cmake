

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)



# Add additional configuration
mcux_add_macro(
    AS "-DIMG_BAUDRATE=96000000"
)
