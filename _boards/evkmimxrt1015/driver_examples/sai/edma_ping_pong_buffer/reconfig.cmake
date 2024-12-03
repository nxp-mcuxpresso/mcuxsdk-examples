
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)
