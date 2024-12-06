


# Add additional configuration
mcux_add_macro(
    CC "-DXIP_IMAGE\
       -DXIP_EXTERNAL_FLASH"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release 
    LINKER devices/RT/RT600/MIMXRT685S/iar/MIMXRT685Sxxxx_cm33_flash.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release 
    LINKER devices/RT/RT600/MIMXRT685S/gcc/MIMXRT685Sxxxx_cm33_flash.ld
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release 
    LINKER devices/RT/RT600/MIMXRT685S/arm/MIMXRT685Sxxxx_cm33_flash.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release 
    LINKER examples/_boards/${board}/ota_examples/ota_mcuboot_basic/linker/MIMXRT685Sxxxx_cm33_flash_mcuboot.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release 
    LINKER examples/_boards/${board}/ota_examples/ota_mcuboot_basic/linker/MIMXRT685Sxxxx_cm33_flash_mcuboot.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release 
    LINKER examples/_boards/${board}/ota_examples/ota_mcuboot_basic/linker/MIMXRT685Sxxxx_cm33_flash_mcuboot.ld
)
