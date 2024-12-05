
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/adc12_low_power_peripheral/peripherals.c
            ${board_root}/${board}/demo_apps/adc12_low_power_peripheral/peripherals.h
)

mcux_add_macro(
    CC "-DFRDM_KE17Z512\
       -DFREEDOM"
)
