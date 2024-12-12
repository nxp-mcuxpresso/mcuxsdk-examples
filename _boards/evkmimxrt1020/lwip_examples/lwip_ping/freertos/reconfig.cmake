# Add additional configuration
mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE"
)
mcux_add_macro(
    TOOLCHAINS iar mdk armgcc
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
