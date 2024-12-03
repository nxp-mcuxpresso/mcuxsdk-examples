mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_sync/RT/*.c
            examples/audio_examples/maestro_sync/RT/*.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_sync/RT
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
)

mcux_add_source(
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    SOURCES app_definitions.h
            app.h
            test_audio.h
)

# Add additional configuration
mcux_add_macro(
    CC "-DPLATFORM_RT1060\
        -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
        -DSAI_XFER_QUEUE_SIZE=4\
        -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING\
        -DOSA_USED\
        -DSHELL_TASK_STACK_SIZE=4000"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1060/MIMXRT1062/gcc/MIMXRT1062xxxxx_flexspi_nor.ld
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/audio_examples/common/linker/MIMXRT1062xxxxx_flexspi_nor.ld
)
