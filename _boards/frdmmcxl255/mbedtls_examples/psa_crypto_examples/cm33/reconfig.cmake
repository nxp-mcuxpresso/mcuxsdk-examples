# ARMGCC debug target does not fit; slightly increase optimization as workaround
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
    CX "-O0"
)

mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
    CX "-O1"
)

mcux_remove_iar_configuration(
    TARGETS debug
    CC "-On"
    CX "-On"
)

mcux_add_iar_configuration(
    TARGETS debug
    CC "-Om"
    CX "-Om"
)
