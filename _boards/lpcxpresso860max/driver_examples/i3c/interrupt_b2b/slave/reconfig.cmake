
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-Og"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)
