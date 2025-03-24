mcux_add_configuration(
    CC "-DBOARD_PMIC_CONFIG_USE_SEMA4=1"
)

mcux_add_mdk_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "--keep=*(*core1_code)"
)

mcux_add_iar_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "--keep=_core1_image"
)

mcux_add_armgcc_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
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
    SOURCES ${board_root}/${board}/demo_apps/power_manager_test/bm/${core_id}/clock_config.c
            ${board_root}/${board}/demo_apps/power_manager_test/bm/${core_id}/clock_config.h
            ${board_root}/${board}/demo_apps/power_manager_test/bm/power_demo_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/power_manager_test/bm/${core_id}
             ${board_root}/${board}/demo_apps/power_manager_test/bm
)
