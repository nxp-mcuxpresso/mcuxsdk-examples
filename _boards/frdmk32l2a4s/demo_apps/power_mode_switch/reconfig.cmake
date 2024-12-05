
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/power_mode_switch/clock_scg.c
            ${board_root}/${board}/demo_apps/power_mode_switch/power_mode_switch.c
            ${board_root}/${board}/demo_apps/power_mode_switch/power_mode_switch.h
)

mcux_add_macro(
    CC "-DFRDM_K32L2A4S\
       -DFREEDOM"
)
