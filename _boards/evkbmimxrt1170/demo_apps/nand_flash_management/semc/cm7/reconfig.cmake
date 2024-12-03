
mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=1\
       -DCACHE_MAINTAIN=1"
)

mcux_add_linker_symbol(
      SYMBOLS "__stack_size__=0x4000 __heap_size__=0x4000"
)
