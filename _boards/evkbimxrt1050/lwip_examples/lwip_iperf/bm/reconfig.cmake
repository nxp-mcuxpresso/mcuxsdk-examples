# Add additional configuration

mcux_remove_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O0"
    CX "-O0"
)
mcux_add_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-Og"
    CX "-Og"
)

mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
        -DENET_RXBUFF_NUM=14\
        -DENET_RXBD_NUM=9"
)
mcux_add_macro(
    TOOLCHAINS iar mdk armgcc
    TARGETS sdram_debug sdram_release
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
