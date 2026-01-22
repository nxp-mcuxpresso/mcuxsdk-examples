
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/ota_examples/ota_mcuboot_server/enet/lwip_netc_port.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ota_examples/ota_mcuboot_server
)

mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_debug
    CC "-Og"
)

mcux_add_macro(
    CC "-DFSL_ETH_ENABLE_CACHE_CONTROL \
        -DSDK_NETC_USED=1 \
        -DNETC_PORT_USE_INCLUDES"
)

mcux_remove_armgcc_configuration(
    TARGETS debug flexspi_nor_debug
    CC "-O0"
)
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-g"
    CX "-g"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1180/MIMXRT1186/iar/MIMXRT1186xxxxx_cm33_flexspi_nor.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1180/MIMXRT1186/gcc/MIMXRT1186xxxxx_cm33_flexspi_nor.ld
)


mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MIMXRT1186xxxxx_cm33_flexspi_nor_mcuboot.icf
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MIMXRT1186xxxxx_cm33_flexspi_nor_mcuboot.ld
)
mcux_remove_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
    CC "-DXIP_BOOT_HEADER_ENABLE=1"
)

mcux_add_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
    CC "-DXIP_BOOT_HEADER_ENABLE=0"
)