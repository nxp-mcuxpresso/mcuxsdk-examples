
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/power_mode_switch/clock_mcglite.c
            examples/_boards/${board}/demo_apps/power_mode_switch/power_mode_switch.c
            examples/_boards/${board}/demo_apps/power_mode_switch/power_mode_switch.h
)

mcux_add_macro(
    CC "-DFRDM_MCXC041\
       -DFREEDOM"
)
