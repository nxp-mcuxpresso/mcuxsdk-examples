
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/driver_examples/casper
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x800"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x800\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x800"
)
