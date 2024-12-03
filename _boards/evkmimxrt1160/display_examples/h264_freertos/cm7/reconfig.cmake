
# Add additional configuration
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_debug
    CX "-O1"
)
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_release
    CX "-O2"
    CC "-O3"
)
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc mdk iar
    TARGETS flexspi_nor_release flexspi_nor_debug
    CC "-DUSE_SDRAM\
       -DXIP_BOOT_HEADER_DCD_ENABLE=1\
       -DDATA_SECTION_IS_CACHEABLE=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)

# Remove additional configuration
mcux_remove_iar_configuration(
    TARGETS flexspi_nor_debug
    CC "-On"
)
mcux_add_iar_configuration(
    TARGETS flexspi_nor_debug
    CC "-Om"
)
mcux_remove_iar_configuration(
    TARGETS flexspi_nor_release
    CC "-Om"
)
mcux_remove_armgcc_configuration(
    TARGETS flexspi_nor_debug
    CX "-O0"
)
mcux_remove_armgcc_configuration(
    TARGETS flexspi_nor_release
    CX "-Os"
    CC "-Os"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/RT/RT1160/MIMXRT1166/iar/MIMXRT1166xxxxx_cm7_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/RT/RT1160/MIMXRT1166/arm/MIMXRT1166xxxxx_cm7_flexspi_nor.scf
)
mcux_remove_armgcc_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/RT/RT1160/MIMXRT1166/gcc/MIMXRT1166xxxxx_cm7_flexspi_nor.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/display_examples/h264_freertos/linker/MIMXRT1166xxxxx_cm7_flexspi_nor_sdram.icf
)
mcux_add_mdk_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/display_examples/h264_freertos/linker/MIMXRT1166xxxxx_cm7_flexspi_nor_sdram.scf
)
mcux_add_armgcc_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/display_examples/h264_freertos/linker/MIMXRT1166xxxxx_cm7_flexspi_nor_sdram.ld
)
