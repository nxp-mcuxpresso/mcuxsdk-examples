
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/ecompass_peripheral/peripherals.c
            examples/_boards/${board}/demo_apps/ecompass_peripheral/peripherals.h
)

mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_I2C_BASED_COMPONENT_USED=1\
       -DFRDM_KE15Z\
       -DFREEDOM"
)
