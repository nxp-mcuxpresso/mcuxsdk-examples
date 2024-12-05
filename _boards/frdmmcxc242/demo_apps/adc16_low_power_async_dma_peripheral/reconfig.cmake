
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/adc16_low_power_async_dma_peripheral/peripherals.c
            ${board_root}/${board}/demo_apps/adc16_low_power_async_dma_peripheral/peripherals.h
)

mcux_add_macro(
    CC "-DFRDM_MCXC242\
       -DFREEDOM"
)
