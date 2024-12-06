mcux_add_macro(
    CC "-DENET_PTP1588FEATURE_REQUIRED"
)

mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x3000\
             __stack_size__=0x3000\
             __ram_vector_table__=1"
)
