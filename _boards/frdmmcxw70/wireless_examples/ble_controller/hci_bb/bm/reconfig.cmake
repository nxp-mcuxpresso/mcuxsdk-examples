
# Compiler options for all toolchains
mcux_add_configuration(
    CC "       -DHAL_UART_ADAPTER_FIFO=1"
)

# Linker symbols
mcux_add_linker_symbol(
    SYMBOLS "gUseNVMLink_d=1\
             gEraseNVMLink_d=1\
             __ram_vector_table__=1"
)

mcux_add_source(
    SOURCES
        ../app_preinclude.h
)

mcux_add_source(
    PREINCLUDE TRUE
    SOURCES ../app_preinclude.h
)

mcux_add_source(
    SOURCES ../readme.txt
)