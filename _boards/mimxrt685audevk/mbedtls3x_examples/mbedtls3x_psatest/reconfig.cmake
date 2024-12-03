mcux_add_armgcc_configuration(
    CC "-Os"
)
mcux_add_mdk_configuration(
    TARGETS debug
    CC "-Oz"
)

mcux_add_macro(
    TOOLCHAINS mdk
    CC "-DMULADDC_CANNOT_USE_R7"
)

mcux_remove_mdk_configuration(
    CC "-O1"
)
