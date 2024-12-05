
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/bubble_peripheral/bubble.c
            ${board_root}/${board}/demo_apps/bubble_peripheral/peripherals.c
            ${board_root}/${board}/demo_apps/bubble_peripheral/peripherals.h
            ${board_root}/${board}/FlashKLxx256KROM_with_config_write_enabled.board
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_I2C_BASED_COMPONENT_USED=1\
       -DFRDM_K32L2B\
       -DFREEDOM"
)
