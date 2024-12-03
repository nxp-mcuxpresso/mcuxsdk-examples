
mcux_add_iar_configuration(
    TARGETS sdram_debug
    CC "-Om"
    CX "-Om"
)

mcux_remove_iar_configuration(
    TARGETS sdram_debug
    CC "-On"
)

mcux_add_armgcc_configuration(
    TARGETS sdram_debug
    CC "-O1"
    CX "-O1"
)

mcux_remove_armgcc_configuration(
    TARGETS sdram_debug
    CC "-O0"
    CX "-O0"
)

mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)





