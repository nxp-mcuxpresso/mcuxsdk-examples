# Add additional configuration

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
    CX "-O0"
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-Og"
    CX "-Og"
)

mcux_add_macro(
    CC "-DLWIP_ENET_FLEXIBLE_CONFIGURATION\
        -DSDK_I2C_BASED_COMPONENT_USED=1\
        -DBOARD_USE_PCAL6524=1"
)
mcux_add_macro(
    TOOLCHAINS iar armgcc
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE"
)
