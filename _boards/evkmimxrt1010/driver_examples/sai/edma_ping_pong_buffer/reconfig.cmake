
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
)
mcux_add_iar_configuration(
    TARGETS debug
    CC "-Ol"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)
mcux_remove_iar_configuration(
    TARGETS debug
    CC "-On"
)
