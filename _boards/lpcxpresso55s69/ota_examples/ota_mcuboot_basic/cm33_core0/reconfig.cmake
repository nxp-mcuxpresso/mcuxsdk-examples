# Add additional configuration
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/LPC/LPC5500/LPC55S69/iar/LPC55S69_cm33_core0_flash.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/LPC/LPC5500/LPC55S69/gcc/LPC55S69_cm33_core0_flash.ld
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/LPC/LPC5500/LPC55S69/arm/LPC55S69_cm33_core0_flash.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/linker/LPC55S69_cm33_core0_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/linker/LPC55S69_cm33_core0_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/linker/LPC55S69_cm33_core0_flash.ld
)
