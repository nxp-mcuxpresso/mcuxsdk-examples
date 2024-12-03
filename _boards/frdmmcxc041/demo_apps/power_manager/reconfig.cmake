
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/power_manager/clock_mcglite.c
            examples/_boards/${board}/demo_apps/power_manager/power_manager.h
            examples/_boards/${board}/demo_apps/power_manager/power_manager.c
)

mcux_add_macro(
    CC "-DFRDM_MCXC041\
       -DFREEDOM"
)
