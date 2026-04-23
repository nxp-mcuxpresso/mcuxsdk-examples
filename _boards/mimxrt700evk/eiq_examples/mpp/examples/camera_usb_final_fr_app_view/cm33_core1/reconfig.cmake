# board+application specific cmake

mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES ${board_root}/${board}/eiq_examples/mpp/drivers/dsp_config.h
    )

mcux_add_configuration(
    CC "-D__USE_SHMEM"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x400 \
             __use_shmem__=1 \
             __multicore__=1 \
             __camera_buffers_shmem_size__=0xC8000 \
            "
)
