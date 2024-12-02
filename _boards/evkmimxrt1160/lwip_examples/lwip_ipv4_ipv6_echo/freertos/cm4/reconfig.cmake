# Add additional configuration
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
        -DLWIP_ENET_FLEXIBLE_CONFIGURATION\
        -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
        -DLWIP_SINGLE_NETIF=0"
)
mcux_add_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-Os"
)
mcux_add_iar_configuration(
    TARGETS debug sdram_debug sdram_release
    CC "-Ohz"
)
mcux_add_mdk_configuration(
    TARGETS debug sdram_debug
    CC "-Oz"
)

# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O0"
)
mcux_remove_iar_configuration(
    TARGETS debug
    CC "-On"
)
mcux_remove_iar_configuration(
    TARGETS sdram_debug sdram_release
    CC "-Om"
)
mcux_remove_mdk_configuration(
    TARGETS debug sdram_debug
    CC "-O1"
)
