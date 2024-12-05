
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/power_manager/clock_scg.c
            ${board_root}/${board}/demo_apps/power_manager/peripherals.c
            ${board_root}/${board}/demo_apps/power_manager/peripherals.h
            ${board_root}/${board}/demo_apps/power_manager/power_manager.h
            ${board_root}/${board}/demo_apps/power_manager/power_manager.c
)

mcux_add_macro(
    CC "-DFRDM_KE17Z512\
       -DFREEDOM"
)
