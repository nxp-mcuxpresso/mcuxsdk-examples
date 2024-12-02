
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
       -DPHY_STABILITY_DELAY_US=500000U"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS sdram_debug sdram_release
    CC "-DUSE_SDRAM"
)
