
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/power_mode_switch_mcxa/low_level_align_with_ds/power_mode_switch.h
    SOURCES ${board_root}/${board}/demo_apps/power_mode_switch/low_level/pm_device.c
            ${board_root}/${board}/demo_apps/power_mode_switch/low_level/pm_device.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/power_mode_switch_mcxa/low_level_align_with_ds
)
