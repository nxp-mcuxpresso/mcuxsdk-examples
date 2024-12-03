
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/demo_apps/led_blinky
)

mcux_add_mdk_configuration(
    LD "--diag_suppress 6329"
)
