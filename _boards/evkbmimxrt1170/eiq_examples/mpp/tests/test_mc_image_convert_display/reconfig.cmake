# board+application specific cmake

# Memory configuration
# Set custom CORE1_BOOT_ADDRESS
set(CORE1_BOOT_ADDRESS 0x20200000)
# Set total core1 data memory size
set(CORE1_DATA_MEM 0x01000000)

mcux_add_macro(
    CC "-DconfigGENERATE_RUN_TIME_STATS=1"
    CX "-DconfigGENERATE_RUN_TIME_STATS=1"
)

mcux_add_armgcc_configuration(
    CC "-DBOOT_SECONDARY_CORE \
        -DRPMSG_USED \
        -DCORE1_BOOT_ADDRESS=${CORE1_BOOT_ADDRESS}"
)

mcux_add_configuration(
    CC "-D__USE_SHMEM"
)

mcux_add_linker_symbol(
    SYMBOLS "__use_shmem__=1 \
             __multicore__=1 \
             __core1_data_mem__=${CORE1_DATA_MEM} \
            "
)
