# Add additional configuration
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
        -DLWIP_ENET_FLEXIBLE_CONFIGURATION\
        -DLWIP_SINGLE_NETIF=0\
        -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE"
)
mcux_add_iar_configuration(
    TARGETS sdram_release
    CC "-Oh"
)
mcux_add_mdk_configuration(
    TARGETS debug sdram_debug
    CC "-Oz"
)

# Remove additional configuration
mcux_remove_iar_configuration(
    TARGETS sdram_release
    CC "-Om"
)
mcux_remove_mdk_configuration(
    TARGETS debug sdram_debug
    CC "-O1"
)
