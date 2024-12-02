mcux_add_iar_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "--config_def=__stack_size__=0x1000\
        --config_def=__heap_size__=0x1000"
)
mcux_add_mdk_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "--predefine=\"-D__stack_size__=0x1000\"\
        --predefine=\"-D__heap_size__=0x1000\"\
        --keep=*(*core1_code)"
)
mcux_add_armgcc_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "-Xlinker --defsym=__stack_size__=0x1000\
        -Xlinker --defsym=__heap_size__=0x1000"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/power_mode_comp_only/${core_id}/clock_config.c
            examples/_boards/${board}/demo_apps/power_mode_comp_only/${core_id}/clock_config.h
            examples/_boards/${board}/demo_apps/power_mode_comp_only/${core_id}/power_mode_comp_only_core0.c
            examples/_boards/${board}/demo_apps/power_mode_comp_only/power_demo_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/demo_apps/power_mode_comp_only/${core_id}
             examples/_boards/${board}/demo_apps/power_mode_comp_only/
)

mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../power_mode_comp_only_secondary/iar/core1_image.bin,_core1_image,__core1_image,4\
        --keep=_core1_image"
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../power_mode_comp_only_secondary/${CONFIG_TOOLCHAIN}/
)

# Use demo specific linker
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core0_ram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/arm/MIMXRT798Sxxxx_cm33_core0_ram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_ram.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_mode_comp_only/${multicore_foldername}/linker/MIMXRT798Sxxxx_cm33_core0_ram.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_mode_comp_only/${multicore_foldername}/linker/MIMXRT798Sxxxx_cm33_core0_ram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_mode_comp_only/${multicore_foldername}/linker/MIMXRT798Sxxxx_cm33_core0_ram.ld
)
