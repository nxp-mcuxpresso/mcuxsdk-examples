
mcux_add_iar_configuration(
    LD "--config_def=__power_down__=1"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__power_down__=1\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__power_down__=1"
)
