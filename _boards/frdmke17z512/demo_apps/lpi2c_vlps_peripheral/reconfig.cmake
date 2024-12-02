
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/lpi2c_vlps_peripheral/peripherals.c
            examples/_boards/${board}/demo_apps/lpi2c_vlps_peripheral/peripherals.h
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DFRDM_KE17Z512\
       -DFREEDOM"
)
