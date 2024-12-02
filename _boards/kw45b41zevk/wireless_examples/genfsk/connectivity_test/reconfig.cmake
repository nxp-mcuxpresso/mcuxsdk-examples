
# Compiler options for all toolchains
mcux_add_configuration(
    CC "        -DCPU_KW45B41Z83AFTA\
                -DSERIAL_PORT_TYPE_UART=1\
                -DENABLE_RAM_VECTOR_TABLE\
                -DHAL_UART_DMA_ENABLE=0\
                -DSERIAL_PORT_TYPE_UART_DMA=0\
                -DHAL_UART_ADAPTER_FIFO=1\
                -DKW45_A0_SUPPORT=0\
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

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wireless/framework/boards/kw45_k32w1/KW45B41Z83
)

mcux_add_source(
    SOURCES 
        ../../app_preinclude_common.h
        ./app_preinclude.h
        ./readme.txt
)

mcux_add_source(
    PREINCLUDE TRUE
    SOURCES ./app_preinclude.h
)
