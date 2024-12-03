
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/lpm.c
            examples/_boards/${board}/lpm.h
            examples/_boards/${board}/demo_apps/power_mode_switch/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
            examples/_boards/${board}/demo_apps/power_mode_switch/rpmsg_config.h
            examples/_boards/${board}/demo_apps/power_mode_switch/clock_scg.c
            examples/_boards/${board}/demo_apps/power_mode_switch/power_mode_switch.c
            examples/_boards/${board}/demo_apps/power_mode_switch/power_mode_switch.h
            examples/_boards/${board}/rsc_table.c
            examples/_boards/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            examples/_boards/${board}/srtm_config.h
            examples/_boards/${board}/app_srtm.c
            examples/_boards/${board}/app_srtm.h
            middleware/issdk/algorithms/pedometer/include/libinclude/KeynetikPedometer.h
            middleware/issdk/algorithms/pedometer/source/pedometer.c
            middleware/issdk/algorithms/pedometer/include/pedometer.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/issdk/algorithms/pedometer/include/libinclude
             middleware/issdk/algorithms/pedometer/source
             middleware/issdk/algorithms/pedometer/include
)

mcux_add_library(
	BASE_PATH ${SdkRootDirPath}
        LIBS middleware/issdk/algorithms/pedometer/lib/libpedometerm4.a
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS armgcc
    SOURCES examples/_boards/${board}/gcc/lpm_asm.S
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS iar
    SOURCES examples/_boards/${board}/iar/lpm_asm.s
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/multicore/remoteproc
)

mcux_add_macro(
    AS "-DNO_CRP"
)
mcux_add_macro(
    TARGETS flash_debug debug
    CC "-DSRTM_DEBUG_MESSAGE_FUNC=DbgConsole_Printf\
       -DSRTM_DEBUG_VERBOSE_LEVEL=SRTM_DEBUG_VERBOSE_WARN"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX7ULP/MCIMX7U5/iar/MCIMX7U5xxxxx_ram.icf
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/i.MX/i.MX7ULP/MCIMX7U5/iar/MCIMX7U5xxxxx_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX7ULP/MCIMX7U5/gcc/MCIMX7U5xxxxx_ram.ld
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/i.MX/i.MX7ULP/MCIMX7U5/gcc/MCIMX7U5xxxxx_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/iar/MCIMX7U5xxxxx_cm4_lpm_ram.icf
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/iar/MCIMX7U5xxxxx_cm4_lpm_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/gcc/MCIMX7U5xxxxx_cm4_lpm_ram.ld
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/gcc/MCIMX7U5xxxxx_cm4_lpm_flash.ld
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x400\
        --config_def=__heap_size__=0x400"
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)
