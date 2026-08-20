mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/ota_examples/mcuboot_opensource
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS xspi_nor_debug xspi_nor_release
        LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_xspi_nor.icf
)

mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS xspi_nor_debug xspi_nor_release
        LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_xspi_nor.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_debug xspi_nor_release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT2663xxxxx_xspi_nor.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_debug xspi_nor_release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT2663xxxxx_xspi_nor.ld
)