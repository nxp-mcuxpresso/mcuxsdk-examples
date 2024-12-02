# Add additional configuration
mcux_add_macro(
    CC "-DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\""
)

# Override stack size
mcux_remove_linker_symbol(
    SYMBOLS "__stack_size__=1000"
)
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x2000"
)

# Override heap size
mcux_remove_linker_symbol(
    SYMBOLS "__heap_size__=40000"
)
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0xA000"
)
