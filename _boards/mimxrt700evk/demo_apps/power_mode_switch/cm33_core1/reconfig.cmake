mcux_add_configuration(
    CC "-DBOARD_PMIC_CONFIG_USE_SEMA4=1"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x1000\
        --config_def=__heap_size__=0x1000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x1000\"\
        --predefine=\"-D__heap_size__=0x1000\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000\
        -Xlinker --defsym=__heap_size__=0x1000"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/power_mode_switch/${core_id}/clock_config.c
            examples/_boards/${board}/demo_apps/power_mode_switch/${core_id}/clock_config.h
            examples/_boards/${board}/demo_apps/power_mode_switch/power_demo_config.h
            examples/_boards/${board}/demo_apps/power_mode_switch/power_mode_switch.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/demo_apps/power_mode_switch/${core_id}
             examples/_boards/${board}/demo_apps/power_mode_switch/
)

# Use demo specific linker
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core1_ram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/arm/MIMXRT798Sxxxx_cm33_core1_ram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core1_ram.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_mode_switch/${multicore_foldername}/linker/MIMXRT798Sxxxx_cm33_core1_ram.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_mode_switch/${multicore_foldername}/linker/MIMXRT798Sxxxx_cm33_core1_ram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_mode_switch/${multicore_foldername}/linker/MIMXRT798Sxxxx_cm33_core1_ram.ld
)
