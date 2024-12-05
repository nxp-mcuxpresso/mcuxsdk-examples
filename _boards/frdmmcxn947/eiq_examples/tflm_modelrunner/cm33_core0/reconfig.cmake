
mcux_remove_iar_configuration(
	CC "-Oh -On"
)


mcux_add_mdk_configuration(
	CX "-std=gnu++17"
)
mcux_add_iar_configuration(
    CC "--dlib_config full\
       -Ohs\
       --diag_suppress=Pe068,Pa025\
       --diag_suppress=Pe260,Pe1031"
    LD "--redirect _Printf=_PrintfFull"
)
mcux_add_mdk_configuration(
    LD "--predefine=-D__stack_size__=0x16000\
       --predefine=-D__heap_size__=0x40000\
       --library_type=standardlib\
       --diag_suppress=6439,6776"
)
mcux_add_mdk_configuration(
    TARGETS release
    CC "-Oz\
       -O3"
)
mcux_add_armgcc_configuration(
    TARGETS release
    CC "-O3"
    CX "-O3"
)

mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
       -DARM_MATH_CM33\
       -DUSE_NPU=1\
       -DMODEL_SIZE=100*1024\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DUSE_RTOS=1\
       -DMODELRUNNER_HTTP=1\
       -D__FPU_PRESENT=1"
    CX "-DSDK_DEBUGCONSOLE_UART\
       -DARM_MATH_CM33\
       -DUSE_NPU=1\
       -DMODEL_SIZE=100*1024\
       -DMODELRUNNER_HTTP=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DUSE_RTOS=1\
       -D__FPU_PRESENT=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    CC "-DLWIP_TIMEVAL_PRIVATE=0"
    CX "-DLWIP_TIMEVAL_PRIVATE=0"
)

mcux_remove_iar_configuration(
    TARGETS debug
    CC "--no_scheduling"
    CX "--no_scheduling"
)
mcux_remove_iar_configuration(
    TARGETS release
    CC "-Om"
)
mcux_remove_mdk_configuration(
    CC "-fshort-wchar"
    CX "-fshort-wchar"
)
mcux_remove_mdk_configuration(
    TARGETS release
    CC "-O3\
       -Oz"
)
mcux_remove_armgcc_configuration(
    LD "--specs=nano.specs"
)
mcux_remove_armgcc_configuration(
    TARGETS release
    CC "-Os"
    CX "-Os"
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/MCX/MCXN/MCXN947/iar/MCXN947_cm33_core0_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/MCX/MCXN/MCXN947/arm/MCXN947_cm33_core0_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/MCX/MCXN/MCXN947/gcc/MCXN947_cm33_core0_flash.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/eiq_examples/tflm_modelrunner/cm33_core0/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/eiq_examples/tflm_modelrunner/cm33_core0/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/eiq_examples/tflm_modelrunner/cm33_core0/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x16000\
        --config_def=__heap_size__=0x40000"
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__heap_size__=0x40000\
        -Xlinker --defsym=__stack_size__=0x16000"
)
