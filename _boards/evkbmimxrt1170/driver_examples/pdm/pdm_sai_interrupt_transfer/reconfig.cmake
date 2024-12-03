
mcux_remove_armgcc_configuration(
    TARGETS sdram_debug
    CC "-O0"
)

mcux_add_armgcc_configuration(
    TARGETS sdram_debug
    CC "-O1"
)
