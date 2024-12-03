
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
    INCLUDES middleware/wireless/framework/boards/kw47_mcxw72x/MCXW727C
)

mcux_add_source(
    SOURCES 
        ../../app_preinclude_common.h
        ./app_preinclude.h
)

mcux_add_source(
    PREINCLUDE TRUE
    SOURCES ./app_preinclude.h
)

mcux_add_source(
    SOURCES ./readme.txt
)

mcux_add_macro(ECC_RAM_STCM8_INIT=1)

#mcux_remove_iar_linker_script(
#        TARGETS debug release
#        BASE_PATH ${SdkRootDirPath}
#        LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
#)

#mcux_add_iar_linker_script(
#        TARGETS debug release
#        LINKER ../../../../../middleware/wireless/framework/Common/devices/kw45_k32w1/iar/connectivity.icf
#)
