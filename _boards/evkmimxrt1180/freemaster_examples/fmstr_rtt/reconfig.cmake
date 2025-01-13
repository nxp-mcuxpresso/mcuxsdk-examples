# Define macro
mcux_add_macro(
    TARGETS hyperram_debug hyperram_release
    CC  "DATA_SECTION_IS_CACHEABLE=1"
)