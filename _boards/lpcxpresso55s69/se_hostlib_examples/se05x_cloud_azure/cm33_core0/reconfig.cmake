
mcux_add_iar_configuration(
    CC "--diag_suppress=Pa082,Pa050,Pe188,Pa089,Pe546,Pa084"
    CX "--diag_suppress=Pa082,Pa050,Pe188,Pa089,Pe546,Pe167,Pe144,Pa183,Pa084"
)

mcux_add_macro(
    CC "-DLPC_55x=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)

mcux_remove_iar_configuration(
    CC "--diag_suppress=Pa082,Pa050"
    CX "--diag_suppress=Pa082,Pa050"
)


mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/LPC/LPC5500/LPC55S69/gcc/LPC55S69_cm33_core0_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/se_hostlib_examples/se05x_cloud_azure/linker/LPC55S69_cm33_core0_flash.ld
)