
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/adc12_low_power_peripheral/peripherals.c
            examples/_boards/${board}/demo_apps/adc12_low_power_peripheral/peripherals.h
)

mcux_add_macro(
    CC "-DFRDM_KE15Z\
       -DFREEDOM"
)
