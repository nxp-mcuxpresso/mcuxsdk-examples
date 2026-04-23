# board+application specific cmake

# Memory configuration
# Set core1 total memory (it includes both text and data sections)
# Increasing this while keeping CORE1_BOOT_ADDRESS fixed will increase data memory for core 1
set(CORE1_TOTAL_MEM 0x1C8000)
# Set custom CORE1_BOOT_ADDRESS
set(CORE1_BOOT_ADDRESS 0x20680000)

mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES ${board_root}/${board}/eiq_examples/mpp/drivers/dsp_config.h
    )

mcux_add_macro(
    CC "-DconfigGENERATE_RUN_TIME_STATS=1 \
        -DHAL_TIMER_PRECISION_1_US"
    CX "-DconfigGENERATE_RUN_TIME_STATS=1 \
        -DHAL_TIMER_PRECISION_1_US"
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
             __multicore__=1 \
             __core1_total_mem__=${CORE1_TOTAL_MEM} \
             __core1_text_start__=${CORE1_BOOT_ADDRESS} \
            "
)
