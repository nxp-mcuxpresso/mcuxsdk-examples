

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
)



# Add additional configuration
mcux_add_macro(
    CC "-DBOARD_SDMMC_NEED_MANUAL_TUNING"
)
