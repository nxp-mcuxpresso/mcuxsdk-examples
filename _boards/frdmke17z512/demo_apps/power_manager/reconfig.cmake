
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/power_manager/clock_scg.c
            examples/_boards/${board}/demo_apps/power_manager/peripherals.c
            examples/_boards/${board}/demo_apps/power_manager/peripherals.h
            examples/_boards/${board}/demo_apps/power_manager/power_manager.h
            examples/_boards/${board}/demo_apps/power_manager/power_manager.c
)

mcux_add_macro(
    CC "-DFRDM_KE17Z512\
       -DFREEDOM"
)
