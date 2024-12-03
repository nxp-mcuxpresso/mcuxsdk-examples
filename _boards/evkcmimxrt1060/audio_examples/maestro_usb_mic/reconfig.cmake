mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_usb_mic/RT/*.c
            examples/audio_examples/maestro_usb_mic/RT/*.h
)

mcux_add_source(
    TOOLCHAINS armgcc
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/audio_examples/common/linker/startup_armgcc.c
)

mcux_add_source(
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    SOURCES app_definitions.h
            app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_usb_mic/RT
)

# Add additional configuration
mcux_add_macro(
    CC "-DUSB_STACK_FREERTOS\
        -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
        -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING\
        -DOSA_USED\
        -DSHELL_TASK_STACK_SIZE=4000"
)
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

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1060/MIMXRT1062/iar/MIMXRT1062xxxxx_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1060/MIMXRT1062/arm/MIMXRT1062xxxxx_flexspi_nor.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1060/MIMXRT1062/gcc/MIMXRT1062xxxxx_flexspi_nor.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/audio_examples/common/linker/MIMXRT1062xxxxx_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/audio_examples/common/linker/MIMXRT1062xxxxx_flexspi_nor.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/audio_examples/common/linker/MIMXRT1062xxxxx_flexspi_nor.ld
)
