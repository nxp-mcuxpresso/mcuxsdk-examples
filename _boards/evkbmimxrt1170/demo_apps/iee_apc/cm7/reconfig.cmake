
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/iee_apc/flexspi_nor_flash_ops.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/iee_apc
)

mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=0\
       -DSCANF_FLOAT_ENABLE=0\
       -DPRINTF_ADVANCED_ENABLE=0\
       -DSCANF_ADVANCED_ENABLE=0"
)
