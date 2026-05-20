# Workaround for empty assembler object files - generate debug information always
mcux_add_iar_configuration(
    TARGETS release
    AS "-r"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)

mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
)