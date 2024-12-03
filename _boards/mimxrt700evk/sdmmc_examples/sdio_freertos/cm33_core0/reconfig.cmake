

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)



# Add additional configuration
mcux_add_macro(
    CC "-DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING\
        -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
