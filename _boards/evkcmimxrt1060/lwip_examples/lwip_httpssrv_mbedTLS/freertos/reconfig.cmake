mcux_remove_linker_symbol(
    SYMBOLS "__heap_size__=110000"
)
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=120000"
)

mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
        -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
