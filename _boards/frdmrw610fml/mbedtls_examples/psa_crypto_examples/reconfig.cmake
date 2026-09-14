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

#mdk common configs
mcux_add_macro(
    TOOLCHAINS mdk
    CC "-DMULADDC_CANNOT_USE_R7"
)

