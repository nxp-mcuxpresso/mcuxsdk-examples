# board+application specific cmake

mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES ${board_root}/${board}/eiq_examples/mpp/drivers/dsp_config.h
    )

mcux_add_macro(
    CC "-DRTOS_HEAP_SIZE=1024 \
    -DHAL_TIMER_PRECISION_1_US \
    -DconfigGENERATE_RUN_TIME_STATS=1\
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DSDK_DEBUGCONSOLE=1"
    CX "DconfigGENERATE_RUN_TIME_STATS=1\
    -DHAL_TIMER_PRECISION_1_US \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DSDK_DEBUGCONSOLE=1"
)

#increase __ncache_size__ also increases the size of npu_ncache section accessible to NPU
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__ncache_size__=0x001F0000"
)

# needed to boot core 1
mcux_add_armgcc_configuration(
    CC "-DBOOT_SECONDARY_CORE \
        -DCORE1_IMAGE_COPY_TO_RAM \
        -DMCMGR_USED \
        -DRPMSG_USED"
)

mcux_add_configuration(
    CC "-D__USE_SHMEM"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x4000 \
             __heap_size__=0x2000 \
             __use_shmem__=1 \
             __multicore__=1 \
             __camera_buffers_shmem_size__=0xC8000 \
            "
)
