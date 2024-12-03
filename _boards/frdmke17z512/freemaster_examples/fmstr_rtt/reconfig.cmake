# Workaround for empty assembler object files - generate debug information always
mcux_add_iar_configuration(
    TARGETS release
    AS "-r"
)