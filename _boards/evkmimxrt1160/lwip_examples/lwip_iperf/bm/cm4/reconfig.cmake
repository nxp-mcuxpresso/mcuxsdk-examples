# Add additional configuration
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
        -DLWIP_ENET_FLEXIBLE_CONFIGURATION\
        -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
        -DENET_RXBUFF_NUM=14\
        -DENET_RXBD_NUM=9"
)
mcux_add_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-Og"
)

# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O0"
)
