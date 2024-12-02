
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/dmic/dmic_i2s_hwvad
)

mcux_add_macro(
    CC "-DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING\
       -DSDK_I2C_BASED_COMPONENT_USED=1\
       -DSDK_DEBUGCONSOLE=1"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS flash_debug flash_release
    CC "-DDMIC_FLASH_TARGET"
)
