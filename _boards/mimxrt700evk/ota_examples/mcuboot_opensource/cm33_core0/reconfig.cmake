mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/ota_examples/mcuboot_opensource
)

# Add additional configuration
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

# MCUX-77300 - fails mflash init due optimization
mcux_add_armgcc_configuration(
    TARGETS flash_release
    CC "-O3"
)
mcux_remove_armgcc_configuration(
    TARGETS flash_release
    CC "-Os"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS flash_debug flash_release
        LINKER ${device_root}/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core0_flash.icf
)

mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS flash_debug flash_release
        LINKER ${device_root}/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld
)

mcux_remove_mdk_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS flash_debug flash_release
        LINKER ${device_root}/RT/RT700/MIMXRT798S/arm/MIMXRT798Sxxxx_cm33_core0_flash.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT798Sxxxx_cm33_core0_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT798Sxxxx_cm33_core0_flash.ld
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT798Sxxxx_cm33_core0_flash.scf
)
