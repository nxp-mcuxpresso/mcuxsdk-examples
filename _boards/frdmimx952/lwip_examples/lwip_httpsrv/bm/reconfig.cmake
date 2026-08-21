
mcux_add_macro(
    CC "-DNETC_PORT_USE_INCLUDES=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/lwip_examples/lwip_netc_port.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/lwip_examples
)

# Remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX952/MIMX9529/iar/MIMX9529_cm7_ram.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX952/MIMX9529/gcc/MIMX9529_cm7_ram.ld
)

# Add Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/frdmimx952/iar/MIMX95294_cm7_lwip_ram.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/frdmimx952/gcc/MIMX95294_cm7_lwip_ram.ld
)
