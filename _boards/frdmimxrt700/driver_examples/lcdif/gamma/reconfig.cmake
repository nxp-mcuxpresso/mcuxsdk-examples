# Add additional configuration
mcux_add_macro(
    CC "-DBOARD_ENABLE_PSRAM_CACHE=0\
        -DSDK_I2C_BASED_COMPONENT_USED=1"
)

mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/lcdif/pin_mux.c
            ${board_root}/${board}/driver_examples/lcdif/pin_mux.h
)