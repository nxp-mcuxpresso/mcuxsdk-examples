
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/driver_examples/lcdifv2/pin_mux.c
            examples/_boards/${board}/driver_examples/lcdifv2/pin_mux.h
            examples/_boards/${board}/driver_examples/lcdifv2/lcdifv2_support.h
            examples/_boards/${board}/driver_examples/lcdifv2/lcdifv2_support.c
            examples/_boards/${board}/driver_examples/lcdifv2/${core_id}/hardware_init.c
            examples/_boards/${board}/driver_examples/lcdifv2/${core_id}/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/lcdifv2/${core_id}
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
)
