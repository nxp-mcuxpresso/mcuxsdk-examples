mcux_add_iar_configuration(
    LD "--config_def=__powerquad__=1"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__powerquad__=1\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__powerquad__=1"
)
