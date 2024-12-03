
# Compiler options for all toolchains
mcux_add_configuration(
    CC "       -DHAL_UART_ADAPTER_FIFO=1"
)

# Linker symbols
mcux_add_linker_symbol(
    SYMBOLS "gUseNVMLink_d=1\
             gEraseNVMLink_d=1\
             __ram_vector_table__=1\
             gFlashNbuImage_d=1\
             gUseProdInfoLegacyMode_d=1\
             __use_shmem__=1"
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wireless/framework/boards/kw47_mcxw72x/KW47B42ZB7
            middleware/wireless/bluetooth/boards/frdmmcxw7x/wireless_examples
)

mcux_add_source(
    SOURCES ../../../../app_preinclude_common.h
        ../../app_preinclude.h
)

mcux_add_source(
    PREINCLUDE TRUE
    SOURCES ../../app_preinclude.h
)

mcux_add_source(
    SOURCES ../../readme.txt
)

mcux_add_macro(ECC_RAM_STCM8_INIT=1)