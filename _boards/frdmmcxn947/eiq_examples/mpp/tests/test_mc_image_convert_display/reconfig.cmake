# board+application specific cmake

# Memory configuration
# Set core1 one total memory (including text and data)
# must be 512 bytes aligned
set(CORE1_TOTAL_MEM 0x2FC00)

# Calculate CORE1_BOOT_ADDRESS by subtracting __core1_total_mem__ from 0x20060000 (end of RAM)
math(EXPR CORE1_BOOT_ADDRESS "0x20060000 - ${CORE1_TOTAL_MEM}" OUTPUT_FORMAT HEXADECIMAL)

mcux_add_macro(
    CC "-DconfigGENERATE_RUN_TIME_STATS=1"
    CX "-DconfigGENERATE_RUN_TIME_STATS=1"
)

# needed to boot core 1
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
             __multicore__=1  \
             __core1_total_mem__=${CORE1_TOTAL_MEM} \
            "
)
