


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
        LINKER devices/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core0_flash.icf
)

mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS flash_debug flash_release
        LINKER devices/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld
)

mcux_remove_mdk_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS flash_debug flash_release
        LINKER devices/RT/RT700/MIMXRT798S/arm/MIMXRT798Sxxxx_cm33_core0_flash.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/ota_examples/linker/MIMXRT798Sxxxx_cm33_core0_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/ota_examples/linker/MIMXRT798Sxxxx_cm33_core0_flash.ld
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/ota_examples/linker/MIMXRT798Sxxxx_cm33_core0_flash.scf
)
