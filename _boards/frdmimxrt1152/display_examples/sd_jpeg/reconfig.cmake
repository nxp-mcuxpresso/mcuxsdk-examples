
mcux_add_macro(
    CC "-DBOARD_USE_PCAL6524=1 -DSDK_I2C_BASED_COMPONENT_USED=1"
)
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_macro(
    TOOLCHAINS iar mdk
    TARGETS flexspi_nor_hyperram_debug flexspi_nor_hyperram_release
    CC "-DUSE_HYPERRAM\
       -DXIP_BOOT_HEADER_XMCD_ENABLE=1\
       -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_hyperram_debug flexspi_nor_hyperram_release
    CC "-DUSE_HYPERRAM\
       -DXIP_BOOT_HEADER_XMCD_ENABLE=1\
       -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)
