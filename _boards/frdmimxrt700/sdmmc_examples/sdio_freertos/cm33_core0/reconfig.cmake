

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
)



# Add additional configuration
mcux_add_macro(
    CC "-DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING\
        -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
