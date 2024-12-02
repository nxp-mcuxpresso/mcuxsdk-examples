#armgcc configurations
mcux_add_armgcc_configuration(
    CC "-Os"
)

#iar configurations
mcux_remove_iar_configuration(
    TARGETS debug flash_debug
    CX "-On"
    CC "-On"
)

mcux_add_iar_configuration(
    TARGETS debug flash_debug
    CC "-Ohz"
    CX "-Ohz"
)

#mdk configurations
mcux_add_mdk_configuration(
    CC "-Oz"
)

#armgcc common configs
mcux_add_armgcc_configuration(
    CC "-Wno-unused-variable"
)

#iar common configs
mcux_add_iar_configuration(
    CC "--diag_suppress Pe546,Pe188,Pe111,Pa084,Pe068,Pe177,Pe223"
    CX "--diag_suppress Pe546,Pe188,Pe111,Pa084,Pe068,Pe177,Pe223"
)

mcux_add_macro(
    CC "-DMCUXPRESSO_SDK"
)

#mdk common configs
mcux_add_macro(
    TOOLCHAINS mdk
    CC "-DMULADDC_CANNOT_USE_R7"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x2000\
        -Xlinker --defsym=__heap_size__=0x8000"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000\
        --config_def=__heap_size__=0x8000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\"\
        --predefine=\"-D__heap_size__=0x8000\""
)
