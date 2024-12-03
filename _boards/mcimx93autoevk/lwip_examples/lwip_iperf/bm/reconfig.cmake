# Add additional configuration
mcux_add_macro(
    CC "-DLWIP_ENET_FLEXIBLE_CONFIGURATION\
        -DENET_RXBUFF_NUM=14\
        -DENET_RXBD_NUM=9\
        -DSDK_I2C_BASED_COMPONENT_USED=1\
        -DBOARD_USE_PCAL6524=1"
)
mcux_add_macro(
    TOOLCHAINS iar armgcc
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE"
)
