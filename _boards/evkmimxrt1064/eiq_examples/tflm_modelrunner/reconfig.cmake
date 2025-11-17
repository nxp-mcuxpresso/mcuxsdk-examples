mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
       -DBOARD_NETWORK_USE_100M_ENET_PORT\
       -DMODEL_SIZE=100*1024\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DUSE_RTOS=1\
       -DMODELRUNNER_HTTP=1\
       -D__FPU_PRESENT=1"
    CX "-DSDK_DEBUGCONSOLE_UART\
       -DBOARD_NETWORK_USE_100M_ENET_PORT\
       -DMODEL_SIZE=100*1024\
       -DMODELRUNNER_HTTP=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DUSE_RTOS=1\
       -D__FPU_PRESENT=1"
)
mcux_add_macro(
  CC "XIP_BOOT_HEADER_DCD_ENABLE=1 \
      USE_SDRAM=1"
)

mcux_remove_iar_configuration(
        CC "-Oh -On"
)

mcux_add_armgcc_configuration(
    TARGETS release
    CC "-O3"
    CX "-O3"
)

mcux_add_armgcc_configuration(
    CC "-Wno-stringop-overflow"
    CX "-Wno-stringop-overflow"
)
mcux_add_mdk_configuration(
    CC "-Wno-format -Wno-enum-compare-switch"
    CX "-Wno-format -Wno-enum-compare-switch"
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
    TARGETS flexspi_nor_sdram_release
    CC "-O3"
)
mcux_add_armgcc_configuration(
    CC "-O3"
    CX "-O3"
)

mcux_add_mdk_configuration(
    LD "--predefine=-D__stack_size__=0x17000\
       --predefine=-D__heap_size__=0x1c00000\
       --library_type=standardlib\
       --diag_suppress=6439,6776"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x17000\
        --config_def=__heap_size__=0x1c00000"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__heap_size__=0x1c00000\
        -Xlinker --defsym=__stack_size__=0x17000"
)
mcux_remove_mdk_configuration(
  CC "-fshort-wchar"
  CX "-fshort-wchar"
  )
