# board+application specific cmake

mcux_add_macro(
    CC "-DRTOS_HEAP_SIZE=32 \
        -DHAL_TIMER_PRECISION_1_US"
    CX "-DHAL_TIMER_PRECISION_1_US"
)

mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x0400 \
             __stack_size__=0x0800 \
            "
)