
mcux_add_macro(
    CC "-DEXAMPLE_DHARA_GARBAGE_COLLECTION_RATIO=255\
       -DEXAMPLE_DHARA_NAND_LOG2_PAGE_SIZE=12\
       -DEXAMPLE_DHARA_NAND_LOG2_PAGE_PER_BLOCK=7\
       -DEXAMPLE_DHARA_NAND_BLOCKS_NUMBER=4096\
       -DPRINTF_FLOAT_ENABLE=1\
       -DEXAMPLE_DHARA_ENABLE_SOFTWARE_ECC"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x4000\
        --config_def=__heap_size__=0x4000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x1000\"\
        --predefine=\"-D__heap_size__=0x1000\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000\
        -Xlinker --defsym=__heap_size__=0x1000"
)
