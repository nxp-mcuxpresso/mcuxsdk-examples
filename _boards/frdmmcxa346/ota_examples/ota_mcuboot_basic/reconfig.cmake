# Add additional configuration
mcux_add_iar_configuration(
    CX "--diag_suppress=Pe257,Pa039"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXA/MCXA346/iar/MCXA346_flash.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXA/MCXA346/gcc/MCXA346_flash.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MCXA346_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MCXA346_flash.ld
)
