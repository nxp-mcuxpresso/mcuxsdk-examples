
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
)

mcux_add_macro(
    CC "-DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1\
       -DEE_DATA_VALUE_REMAINING_PART=16\
       -DFLASH_PGM_SIZE=16"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)
