
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_macro(
    TOOLCHAINS iar mdk
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    CC "-DUSE_SDRAM\
       -DXIP_BOOT_HEADER_DCD_ENABLE=1\
       -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    CC "-DUSE_SDRAM\
       -DXIP_BOOT_HEADER_DCD_ENABLE=1\
       -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)
