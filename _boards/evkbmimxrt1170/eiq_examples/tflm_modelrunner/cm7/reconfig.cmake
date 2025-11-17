mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
       -DMODEL_SIZE=100*1024\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DUSE_RTOS=1\
       -DMODELRUNNER_HTTP=1\
       -D__FPU_PRESENT=1"
    CX "-DSDK_DEBUGCONSOLE_UART\
       -DMODEL_SIZE=100*1024\
       -DMODELRUNNER_HTTP=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DUSE_RTOS=1\
       -D__FPU_PRESENT=1"
)

mcux_remove_iar_configuration(
        CC "-Oh -On"
)

mcux_add_armgcc_configuration(
    TARGETS release
    CC "-O3"
    CX "-O3"
)

mcux_add_mdk_configuration(
        CX "-std=gnu++17"
)

mcux_add_iar_configuration(
    CC "--dlib_config full\
       -Ohs\
       --diag_suppress=Pe068,Pa025"
    LD "--redirect _Printf=_PrintfFull"
)
mcux_add_mdk_configuration(
    LD "--predefine=-D__stack_size__=0x17000\
       --predefine=-D__heap_size__=0x2000000\
       --library_type=standardlib\
       --diag_suppress=6439,6776"
)
mcux_remove_mdk_configuration(
  CC "-fshort-wchar"
  CX "-fshort-wchar"
  )
mcux_add_mdk_configuration(
    TARGETS flexspi_nor_sdram_release
    CC "-O3"
)

mcux_add_mdk_configuration(
    CC "-Wno-format -Wno-enum-compare-switch"
    CX "-Wno-format -Wno-enum-compare-switch"
)
mcux_add_armgcc_configuration(
    CC "-O3"
    CX "-O3"
)
mcux_add_armgcc_configuration(
    CC "-Wno-stringop-overflow"
    CX "-Wno-stringop-overflow"
)
mcux_add_macro(
    CC "PRINTF_ADVANCED_ENABLE=1 \
        FSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE \
	FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
        PRINTF_FLOAT_ENABLE=1"
    CX "MODELRUNNER_HTTP=1 \
       USE_RTOS=1" 
    )
mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x2000000\
  --config_def=__stack_size__=0x4000"
  )
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x2000000\
  -Xlinker --defsym=__stack_size__=0x4000"
  )
