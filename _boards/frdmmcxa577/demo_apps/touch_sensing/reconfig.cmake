mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/touch_sensing/clock_config.h
            ${board_root}/${board}/demo_apps/touch_sensing/clock_config.c
)
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/touch_sensing
)