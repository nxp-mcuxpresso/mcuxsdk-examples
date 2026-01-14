mcux_add_armgcc_configuration(
    CC "-fno-strict-aliasing\
       -Wno-unused-function"
    LD "-Xlinker --no-wchar-size-warning"
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
    CX "-O1"
)
mcux_add_iar_configuration(
    LD "--semihosting"
    CC "--diag_suppress Pe830"
)
mcux_add_iar_configuration(
    TARGETS release
    CX "--no_cse\
       --no_unroll\
       --no_code_motion\
       --no_tbaa\
       --no_clustering\
       --no_scheduling"
)
mcux_add_mdk_configuration(
    LD "--predefine=-D__stack_size__=0x1000
       --predefine=-D__heap_size__=0x2000"
)

mcux_add_macro(
    CC "-DARM_MATH_CM33\
       -D__FPU_PRESENT=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_DEBUGCONSOLE=1\
       -DETHOSU_ARCH=u65\
       -DETHOSU65=1"
    CX "-DARM_MATH_CM33\
       -D__FPU_PRESENT=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_DEBUGCONSOLE=1\
       -DETHOSU_ARCH=u65\
       -DETHOSU65=1"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
    CX "-O0"
)
mcux_remove_iar_configuration(
    TARGETS release
    CC "-Om"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX93W/MIMX93W52/iar/MIMX93W52xxxxxM_ram.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX93W/MIMX93W52/gcc/MIMX93W52xxxxxM_ram.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    TARGETS debug release
    LINKER MIMX93W52_cm33_ram.icf
)
mcux_add_armgcc_linker_script(
    TARGETS debug release
    LINKER MIMX93W52_cm33_ram.ld
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000\
        -Xlinker --defsym=__heap_size__=0x2000"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x1000\
        --config_def=__heap_size__=0x2000"
)
