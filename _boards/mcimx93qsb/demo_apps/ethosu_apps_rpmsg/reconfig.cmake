
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/ethos-u-core-software/examples/ethosu_apps_rpmsg/ethosu_apps_rpmsg.cpp
            middleware/ethos-u-core-software/boards/${board}/pin_mux.c
            middleware/ethos-u-core-software/boards/${board}/pin_mux.h
            middleware/ethos-u-core-software/boards/${board}/rpmsg_config.h
            examples/_boards/mcimx93evk/FreeRTOSConfigBoard.h
            middleware/ethos-u-core-software/boards/${board}/FreeRTOSConfig.h
            middleware/ethos-u-core-software/include/ethosu_core_interface.h
            middleware/ethos-u-core-software/boards/${board}/hardware_init.c
            middleware/ethos-u-core-software/boards/${board}/app.h
            middleware/eiq/common/toolchains/iar/retarget.c
            middleware/eiq/common/toolchains/mdk/retarget.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/ethos-u-core-software/include
             middleware/ethos-u-core-software/boards/mcimx93qsb
             middleware/ethos-u-core-software/examples/ethosu_apps_rpmsg
             examples/_boards/mcimx93evk
             middleware/ethos-u-core-software/board/mcimx93qsb
)

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
    CC "-Ol"
)
mcux_add_mdk_configuration(
    LD "--predefine="-D__stack_size__=0x1000"\
       --predefine="-D__heap_size__=0x2000""
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
    LINKER devices/i.MX/i.MX93/MIMX9322/iar/MIMX9322xxxxM_ram.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX93/MIMX9322/gcc/MIMX9322xxxxM_ram.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/ethos-u-core-software/boards/${board}/MIMX9322_cm33_ram.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/ethos-u-core-software/boards/${board}/MIMX9322_cm33_ram.ld
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000\
        -Xlinker --defsym=__heap_size__=0x2000"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x1000\
        --config_def=__heap_size__=0x2000"
)
