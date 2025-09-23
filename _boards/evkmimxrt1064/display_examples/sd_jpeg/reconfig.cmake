
# Add additional configuration
mcux_add_mdk_configuration(
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LD "--keep=*(.boot_hdr.dcd_data)"
)
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_macro(
    TOOLCHAINS iar mdk armgcc
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    CC "-DSKIP_SYSCLK_INIT\
       -DXIP_BOOT_HEADER_DCD_ENABLE=1"
)

mcux_add_iar_configuration(
    TARGETS sdram_debug flexspi_nor_sdram_debug
    CC "-Om"
)

mcux_remove_iar_configuration(
    TARGETS sdram_debug flexspi_nor_sdram_debug
    CC "-On"
)

mcux_add_armgcc_configuration(
    TARGETS sdram_debug
    CC "-O1"
    CX "-O1"
)

mcux_remove_armgcc_configuration(
    TARGETS sdram_debug
    CC "-O0"
    CX "-O0"
)
