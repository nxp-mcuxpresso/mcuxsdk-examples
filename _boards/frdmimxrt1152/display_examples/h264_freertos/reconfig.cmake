
# Add additional configuration
mcux_remove_iar_configuration(
    TARGETS flexspi_nor_debug
    CC "-On"
    CX "-On"
)
mcux_add_iar_configuration(
    TARGETS flexspi_nor_debug
    CC "-Om"
    CX "-Om"
)
mcux_remove_armgcc_configuration(
    TARGETS flexspi_nor_debug
    CC "-O0"
    CX "-O0"
)
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_debug
    CC "-O1"
    CX "-O1"
)
mcux_remove_armgcc_configuration(
    TARGETS flexspi_nor_release
    CC "-Os"
    CX "-Os"
)
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_release
    CC "-O3"
    CX "-O2"
)
mcux_add_macro(
    CC "-DBOARD_USE_PCAL6524=1 -DSDK_I2C_BASED_COMPONENT_USED=1"
)
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc mdk iar
    TARGETS flexspi_nor_release flexspi_nor_debug
    CC "-DUSE_HYPERRAM\
       -DXIP_BOOT_HEADER_XMCD_ENABLE=1\
       -DDATA_SECTION_IS_CACHEABLE=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/iar/MIMXRT1152xxxxx_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/arm/MIMXRT1152xxxxx_flexspi_nor.scf
)
mcux_remove_armgcc_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/gcc/MIMXRT1152xxxxx_flexspi_nor.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${board_root}/${board}/display_examples/h264_freertos/linker/MIMXRT1152xxxxx_flexspi_nor_hyperram.icf
)
mcux_add_mdk_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${board_root}/${board}/display_examples/h264_freertos/linker/MIMXRT1152xxxxx_flexspi_nor_hyperram.scf
)
mcux_add_armgcc_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${board_root}/${board}/display_examples/h264_freertos/linker/MIMXRT1152xxxxx_flexspi_nor_hyperram.ld
)
