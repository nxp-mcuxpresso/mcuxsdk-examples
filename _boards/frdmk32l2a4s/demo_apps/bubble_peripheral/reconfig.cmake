
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/bubble_peripheral/bubble.c
            examples/_boards/${board}/demo_apps/bubble_peripheral/peripherals.c
            examples/_boards/${board}/demo_apps/bubble_peripheral/peripherals.h
            examples/_boards/${board}/demo_apps/bubble_peripheral/clock_config.c
            examples/_boards/${board}/demo_apps/bubble_peripheral/clock_config.h
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_I2C_BASED_COMPONENT_USED=1\
       -DFRDM_K32L2A4S\
       -DFREEDOM"
)
