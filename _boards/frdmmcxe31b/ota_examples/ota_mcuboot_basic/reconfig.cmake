# Add additional configuration
mcux_add_iar_configuration(
    CX "--diag_suppress=Pe257,Pa039"
)

mcux_add_macro(
    CC "-DBYPASS_ECC_ITCM_INIT=1\
        -DBYPASS_ECC_DTCM_INIT=1\
        -DBYPASS_ECC_SRAM_INIT=1"
)

# Add or remove Linker File Configurations

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXE/MCXE31B/iar/MCXE31B_flash.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXE/MCXE31B/gcc/MCXE31B_flash.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MCXE31B_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MCXE31B_flash.ld
)
