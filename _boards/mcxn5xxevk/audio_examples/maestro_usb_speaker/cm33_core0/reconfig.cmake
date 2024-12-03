mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/audio_examples/maestro_usb_speaker/RT/*.c
            examples/audio_examples/maestro_usb_speaker/RT/*.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/audio_examples/maestro_usb_speaker/RT
)

# Add additional configuration
mcux_add_macro(
    CC "-DUSB_STACK_FREERTOS\
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
