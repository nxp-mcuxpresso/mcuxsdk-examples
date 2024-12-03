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
