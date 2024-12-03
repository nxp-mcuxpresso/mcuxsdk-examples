
mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS sdram_debug sdram_release
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
