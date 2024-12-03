# Add additional configuration
mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE"
)
mcux_add_macro(
    TOOLCHAINS iar mdk armgcc
    TARGETS sdram_debug sdram_release
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_iar_configuration(
    TARGETS debug sdram_debug
    CC "-Oh"
)
mcux_add_mdk_configuration(
    TARGETS debug sdram_debug
    CC "-Oz"
)
mcux_add_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O2"
)

# Remove additional configuration
mcux_remove_iar_configuration(
    TARGETS debug
    CC "-On"
)
mcux_remove_iar_configuration(
    TARGETS sdram_debug
    CC "-Om"
)
mcux_remove_mdk_configuration(
    TARGETS debug sdram_debug
    CC "-O1"
)
mcux_remove_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O0"
)
