mcux_add_iar_configuration(
    LD "--config_def=__dual_image__=1"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__dual_image__=1\""
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__dual_image__=1"
)