# Add additional configuration
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-Og"
)

# Override stack size
mcux_remove_linker_symbol(
    SYMBOLS "__stack_size__=2048"
)
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=1000"
)

# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)
