
# Compiler options for all toolchains
mcux_add_configuration(
    CC "-DSERIAL_PORT_TYPE_UART=1\
        -DHAL_UART_DMA_ENABLE=0\
        -DSERIAL_PORT_TYPE_UART_DMA=0\
        -DHAL_UART_ADAPTER_FIFO=1\
        -DTIMER_PORT_TYPE_LPTMR=1\
        -DHAL_FLASH_ROMAPI_DRIVER=1\
        -DSERIAL_MANAGER_NON_BLOCKING_MODE=1"
)

# Linker symbols
mcux_add_linker_symbol(
    SYMBOLS "gUseNVMLink_d=1\
             gEraseNVMLink_d=1\
             __ram_vector_table__=1"
)

mcux_add_source(
    SOURCES
        ./app_preinclude.h
)

mcux_add_source(
    PREINCLUDE TRUE
    SOURCES ./app_preinclude.h
)
