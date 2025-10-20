
mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1\
       -DSDK_DEBUGCONSOLE_UART"
)

mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS hyperram_release release sdram_release
    CC "-DLFS_NO_ASSERT"
)

mcux_remove_iar_configuration(
    LD "--config_def=__heap_size__=0x400"
)
mcux_add_iar_configuration(
    LD "--config_def=__heap_size__=0x1000"
)

mcux_remove_mdk_configuration(
    LD "--predefine=\"-D__heap_size__=0x400\""
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__heap_size__=0x1000\""
)

mcux_remove_armgcc_configuration(
    LD "--predefine=\"-D__heap_size__=0x400\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__heap_size__=0x1000"
)
