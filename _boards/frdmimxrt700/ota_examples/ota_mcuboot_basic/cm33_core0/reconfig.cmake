# MCUX-82395 problem with boot - decrease optimization
mcux_add_armgcc_configuration(
    TARGETS flash_release
    CC "-O3"
)

mcux_remove_armgcc_configuration(
    TARGETS flash_release
    CC "-Os"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ota_examples/_common/sb3_api/sb3_api.h
            examples/ota_examples/_common/sb3_api/sb3_api_rt7xx.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ota_examples/_common/sb3_api
)


# Add additional configuration
mcux_add_macro(
    CC "-DXIP_IMAGE\
       -DXIP_EXTERNAL_FLASH\
       -DMCUBOOT_OTA_SB3_SUPPORT"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
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

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MIMXRT798Sxxxx_cm33_core0_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MIMXRT798Sxxxx_cm33_core0_flash.ld
)
