
# Compiler options for all toolchains
mcux_add_configuration(
    CC "        -DHAL_UART_ADAPTER_FIFO=1" 
)

# Linker symbols
mcux_add_linker_symbol(
    SYMBOLS "gUseNVMLink_d=1\
             gEraseNVMLink_d=1\
             __ram_vector_table__=1\
             gUseProdInfoLegacyMode_d=1\
             __use_shmem__=1"
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wireless/framework/boards/kw45_k32w1/KW45B41Z83
            middleware/wireless/bluetooth/boards/kw45b41zevk/wireless_examples
)

mcux_add_source(
    SOURCES ../../../app_preinclude_common.h
        ../app_preinclude.h
        ../readme.txt
)

mcux_add_source(
    PREINCLUDE TRUE
    SOURCES ../app_preinclude.h
)
