
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/lcdifv2/lcdifv2_support.h
            ${board_root}/${board}/driver_examples/lcdifv2/lcdifv2_support.c
            ${board_root}/${board}/driver_examples/lcdifv2/hardware_init.c
            ${board_root}/${board}/driver_examples/lcdifv2/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/lcdifv2/
)

mcux_add_macro(
    CC "-DBOARD_USE_PCAL6524=1 -DSDK_I2C_BASED_COMPONENT_USED=1"
)
