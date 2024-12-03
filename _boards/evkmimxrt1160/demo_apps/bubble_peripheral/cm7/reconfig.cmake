
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/bubble_peripheral/bubble.c
            examples/_boards/${board}/demo_apps/bubble_peripheral/peripherals.c
            examples/_boards/${board}/demo_apps/bubble_peripheral/peripherals.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/demo_apps/bubble_peripheral
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_I2C_BASED_COMPONENT_USED=1"
)
