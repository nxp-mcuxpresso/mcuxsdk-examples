

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
)



# Add additional configuration
mcux_add_macro(
    AS "-DIMG_BAUDRATE=96000000"
)
