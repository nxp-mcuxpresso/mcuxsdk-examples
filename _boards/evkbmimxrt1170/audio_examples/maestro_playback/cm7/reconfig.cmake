mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_playback/RT/*.c
            examples/audio_examples/maestro_playback/RT/*.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_playback/RT
)

mcux_add_source(
    TOOLCHAINS armgcc
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/audio_examples/common/linker/startup_armgcc.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
)

mcux_add_macro(
    CC "-DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING\
        -DOSA_USED\
        -DSHELL_TASK_STACK_SIZE=4000"
)

# Add additional configuration
mcux_add_iar_configuration(
    CC "--vla\
        --align_sp_on_irq\
        --macro_positions_in_diagnostics\
        --header_context\
        --no_unroll\
        --no_inline\
        --no_tbaa\
        --no_unaligned_access\
        --dlib_config full\
        --diag_suppress Pa050,Pa082,Pe188"
)

# Remove additional configuration
#mcux_remove_iar_configuration(
#    CC "--diag_suppress Pa082,Pa050"
#    CX "--diag_suppress Pa082,Pa050"
#)

mcux_add_macro(
    TOOLCHAINS armgcc mcux
    CC "-DPLATFORM_ID=LVM_IMXRT1170"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1170/MIMXRT1176/iar/MIMXRT1176xxxxx_cm7_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1170/MIMXRT1176/arm/MIMXRT1176xxxxx_cm7_flexspi_nor.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1170/MIMXRT1176/gcc/MIMXRT1176xxxxx_cm7_flexspi_nor.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/audio_examples/common/linker/MIMXRT1176xxxxx_cm7_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/audio_examples/common/linker/MIMXRT1176xxxxx_cm7_flexspi_nor.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/audio_examples/common/linker/MIMXRT1176xxxxx_cm7_flexspi_nor.ld
)
