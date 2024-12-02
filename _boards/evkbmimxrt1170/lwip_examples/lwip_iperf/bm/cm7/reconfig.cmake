# Add additional configuration
mcux_add_macro(
    CC "-DLWIP_ENET_FLEXIBLE_CONFIGURATION\
       -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
       -DENET_RXBUFF_NUM=14\
       -DENET_RXBD_NUM=9"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)
