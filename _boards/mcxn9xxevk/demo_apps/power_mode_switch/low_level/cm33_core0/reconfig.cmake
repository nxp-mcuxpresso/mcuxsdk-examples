
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/power_mode_switch_mcx/low_level/power_mode_switch.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/power_mode_switch_mcx/low_level
             examples/_boards/${board}/demo_apps/power_mode_switch/low_level
)
