
# Add additional configuration
mcux_add_iar_configuration(
    TARGETS flexspi_nor_sdram_debug
    CC "-Om"
)
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_sdram_debug
    CX "-O1"
)
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_sdram_release
    CX "-O2"
    CC "-O3"
)
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)

# Remove additional configuration
mcux_remove_iar_configuration(
    TARGETS flexspi_nor_sdram_debug
    CC "-On"
)

mcux_remove_iar_configuration(
    TARGETS flexspi_nor_sdram_release
    CC "-Om"
)
mcux_remove_armgcc_configuration(
    TARGETS flexspi_nor_sdram_debug
    CX "-O0"
)
mcux_remove_armgcc_configuration(
    TARGETS flexspi_nor_sdram_release
    CX "-Os"
    CC "-Os"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER devices/RT/RT1060/MIMXRT1062/iar/MIMXRT1062xxxxx_flexspi_nor_sdram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER devices/RT/RT1060/MIMXRT1062/arm/MIMXRT1062xxxxx_flexspi_nor_sdram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER devices/RT/RT1060/MIMXRT1062/gcc/MIMXRT1062xxxxx_flexspi_nor_sdram.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER examples/_boards/${board}/display_examples/h264_freertos/linker/MIMXRT1062xxxxx_flexspi_nor_sdram.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER examples/_boards/${board}/display_examples/h264_freertos/linker/MIMXRT1062xxxxx_flexspi_nor_sdram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER examples/_boards/${board}/display_examples/h264_freertos/linker/MIMXRT1062xxxxx_flexspi_nor_sdram.ld
)
