
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/lpm.c
            ${board_root}/${board}/lpm.h
            ${board_root}/${board}/demo_apps/power_mode_switch/FreeRTOSConfig.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/drivers/lpi2c/fsl_lpi2c_freertos.c
            ${board_root}/${board}/drivers/lpi2c/fsl_lpi2c_freertos.h
            ${board_root}/${board}/demo_apps/power_mode_switch/rpmsg_config.h
            ${board_root}/${board}/demo_apps/power_mode_switch/clock_scg.c
            ${board_root}/${board}/demo_apps/power_mode_switch/power_mode_switch.c
            ${board_root}/${board}/demo_apps/power_mode_switch/power_mode_switch.h
            ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/${board}/srtm_config.h
            ${board_root}/${board}/app_srtm.c
            ${board_root}/${board}/app_srtm.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS armgcc
    SOURCES ${board_root}/${board}/gcc/lpm_asm.S
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS iar
    SOURCES ${board_root}/${board}/iar/lpm_asm.s
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/multicore/remoteproc
             ${board_root}/${board}/drivers/lpi2c
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
    LINKER ${device_root}/i.MX/i.MX7ULP/MCIMX7U5/iar/MCIMX7U5xxxxx_ram.icf
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/i.MX/i.MX7ULP/MCIMX7U5/iar/MCIMX7U5xxxxx_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX7ULP/MCIMX7U5/gcc/MCIMX7U5xxxxx_ram.ld
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/i.MX/i.MX7ULP/MCIMX7U5/gcc/MCIMX7U5xxxxx_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/iar/MCIMX7U5xxxxx_cm4_lpm_ram.icf
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/iar/MCIMX7U5xxxxx_cm4_lpm_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/gcc/MCIMX7U5xxxxx_cm4_lpm_ram.ld
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/gcc/MCIMX7U5xxxxx_cm4_lpm_flash.ld
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x400\
        --config_def=__heap_size__=0x400"
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)
