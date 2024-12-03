# Add additional configuration
mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE"
)
mcux_add_iar_configuration(
    TARGETS debug
    CC "-Om"
)
mcux_add_macro(
    TOOLCHAINS iar mdk armgcc
    TARGETS sdram_debug sdram_release
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)

# Remove additional configuration
mcux_remove_iar_configuration(
    TARGETS debug
    CC "-On"
)
