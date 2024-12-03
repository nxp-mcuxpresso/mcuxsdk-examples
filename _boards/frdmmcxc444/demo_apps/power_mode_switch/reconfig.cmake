
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/power_mode_switch/clock_mcglite.c
            examples/_boards/${board}/demo_apps/power_mode_switch/power_mode_switch.c
            examples/_boards/${board}/demo_apps/power_mode_switch/power_mode_switch.h
            examples/_boards/${board}/FlashKLxx256KROM_with_config_write_enabled.board
)

mcux_add_macro(
    CC "-DFRDM_MCXC444\
       -DFREEDOM"
)
