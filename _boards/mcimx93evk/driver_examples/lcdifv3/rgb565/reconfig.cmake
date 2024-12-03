
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/driver_examples/lcdifv3/pin_mux.c
            examples/_boards/${board}/driver_examples/lcdifv3/pin_mux.h
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_ADP5585=1\
       -DRM67199_ENABLE=1"
)
