
# Heap configuration
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x2000"
)
# Stack configuration
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x2000"
)
# Define macro
mcux_add_macro(
    TARGETS hyperram_debug hyperram_release
    CC  "DATA_SECTION_IS_CACHEABLE=1"
)
# Define macro
mcux_add_macro(
    TARGETS hyperram_txt_debug hyperram_txt_release
    CC  "DATA_SECTION_IS_CACHEABLE=1"
)