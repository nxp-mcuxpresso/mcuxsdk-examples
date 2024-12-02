# Add additional configuration
mcux_add_iar_configuration(
    CX "--diag_suppress Pa082,Pa050,Pe257,Pa039"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)



# Remove additional configuration
mcux_remove_iar_configuration(
    CX "--diag_suppress Pa082,Pa050"
)


# Add or remove Linker File Configurations

mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/MCX/MCXA/MCXA156/iar/MCXA156_flash.icf
)

mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/MCX/MCXA/MCXA156/gcc/MCXA156_flash.ld
)

mcux_remove_mdk_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/MCX/MCXA/MCXA156/arm/MCXA156_flash.csf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/linker/MCXA156_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/linker/MCXA156_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/linker/MCXA156_flash.ld
)
