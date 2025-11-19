# Add additional configuration
mcux_add_macro(
    TOOLCHAINS iar mdk armgcc mcux
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
        -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
