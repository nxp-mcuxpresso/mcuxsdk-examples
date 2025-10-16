mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/wifi_bt_config.c
            ${board_root}/${board}/wifi_bt_config.h
)

mcux_add_macro(
    CC "-DXIP_IMAGE\
       -DXIP_EXTERNAL_FLASH\
       -DHIGH_SPEED_SDIO_CLOCK"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/RT/RT600/MIMXRT685S/iar/MIMXRT685Sxxxx_cm33_flash.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/RT/RT600/MIMXRT685S/gcc/MIMXRT685Sxxxx_cm33_flash.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MIMXRT685Sxxxx_cm33_flash_mcuboot.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MIMXRT685Sxxxx_cm33_flash_mcuboot.ld
)
