mcux_add_source(
    TOOLCHAINS armgcc
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/audio_examples/common/linker/startup_armgcc.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
)

# Add additional configuration
mcux_add_macro(
    CC "-DPLATFORM_RT1170_EVKB\
        -DSAI_XFER_QUEUE_SIZE=4\
        -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING\
        -DOSA_USED\
        -DSHELL_TASK_STACK_SIZE=4000"
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1170/MIMXRT1176/gcc/MIMXRT1176xxxxx_cm7_flexspi_nor.ld
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/audio_examples/common/linker/MIMXRT1176xxxxx_cm7_flexspi_nor.ld
)
