mcux_add_include(
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    INCLUDES .
)

mcux_add_source(
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    SOURCES app_definitions.h
)

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
    SOURCES ${board_root}/${board}/audio_examples/common/linker/startup_armgcc.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
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

mcux_add_macro(
    TOOLCHAINS armgcc mcux
    CC "-DPLATFORM_ID=LVM_IMXRT1152"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/iar/MIMXRT1152xxxxx_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/arm/MIMXRT1152xxxxx_flexspi_nor.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/gcc/MIMXRT1152xxxxx_flexspi_nor.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/audio_examples/common/linker/MIMXRT1152xxxxx_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/audio_examples/common/linker/MIMXRT1152xxxxx_flexspi_nor.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/audio_examples/common/linker/MIMXRT1152xxxxx_flexspi_nor.ld
)