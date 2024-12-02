# Add additional configuration
mcux_add_macro(
    CC "-DLWIP_ENET_FLEXIBLE_CONFIGURATION\
       -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
