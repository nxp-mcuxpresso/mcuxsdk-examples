mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/power_mode_switch/peripherals.c
            ${board_root}/${board}/demo_apps/power_mode_switch/peripherals.h
)
include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/reconfig.cmake)
