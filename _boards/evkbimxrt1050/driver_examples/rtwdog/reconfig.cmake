
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_debug flexspi_nor_release
    CC "-DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)
