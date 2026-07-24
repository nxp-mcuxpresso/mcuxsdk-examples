mcux_add_macro(
    TOOLCHAINS armgcc iar mdk mcux
    TARGETS debug release psram_debug psram_release psram_txt_debug psram_txt_release xspi_nor_debug xspi_nor_release xspi_nor_psram_debug xspi_nor_psram_release
    CC "-DCOREMARK_USING_SYSTICK=1"
)

mcux_remove_armgcc_configuration(
    TARGETS release psram_release
    CC "-Os"
)

mcux_add_armgcc_configuration(
    TARGETS release psram_release
    CC "-O3\
        -funroll-all-loops"
)

mcux_remove_mdk_configuration(
    TARGETS release psram_release
    CC "-Oz"
)

mcux_add_mdk_configuration(
    TARGETS release psram_release
    CC "-Omax"
    LD "--lto --keep=Reset_Handler_C"
)
