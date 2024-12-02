mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
)

# Add additional configuration
mcux_add_macro(
    CC "-DPLATFORM_MCXN947\
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

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x1000 --config_def=__heap_size__=0xF000 --semihosting"
)