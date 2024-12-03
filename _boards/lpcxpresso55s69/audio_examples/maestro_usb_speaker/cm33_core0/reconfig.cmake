# Add additional configuration
mcux_add_macro(
    CC "-DUSB_STACK_USE_DEDICATED_RAM=1\
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
    TARGETS debug release
    LINKER devices/LPC/LPC5500/LPC55S69/iar/LPC55S69_cm33_core0_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/LPC/LPC5500/LPC55S69/arm/LPC55S69_cm33_core0_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/LPC/LPC5500/LPC55S69/gcc/LPC55S69_cm33_core0_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/audio_examples/common/linker/LPC55S69_cm33_core0_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/audio_examples/common/linker/LPC55S69_cm33_core0_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/audio_examples/common/linker/LPC55S69_cm33_core0_flash.ld
)