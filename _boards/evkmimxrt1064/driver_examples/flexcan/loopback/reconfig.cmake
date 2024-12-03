
mcux_add_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O1"
    CX "-O1"
)

mcux_remove_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O0"
    CX "-O0"
)
