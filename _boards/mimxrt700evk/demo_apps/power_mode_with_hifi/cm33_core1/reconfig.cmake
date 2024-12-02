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
    SOURCES examples/_boards/${board}/demo_apps/power_mode_with_hifi/${core_id}/clock_config.c
            examples/_boards/${board}/demo_apps/power_mode_with_hifi/${core_id}/clock_config.h
            examples/_boards/${board}/demo_apps/power_mode_with_hifi/power_demo_config.h
            examples/_boards/${board}/demo_apps/power_mode_with_hifi/power_mode_with_hifi.c
            examples/_boards/${board}/demo_apps/power_mode_with_hifi/dsp_support.c
            examples/_boards/${board}/demo_apps/power_mode_with_hifi/dsp_support.h
            examples/_boards/${board}/demo_apps/power_mode_with_hifi/dsp_config.h
            examples/_boards/${board}/demo_apps/power_mode_with_hifi/hifi1/binary/*.bin
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/demo_apps/power_mode_with_hifi/${core_id}
             examples/_boards/${board}/demo_apps/power_mode_with_hifi/
             examples/_boards/${board}/demo_apps/power_mode_with_hifi/hifi1/binary/
)

mcux_add_iar_configuration(
    LD "--image_input=${SdkRootDirPath}/examples/_boards/${board}/demo_apps/power_mode_with_hifi/hifi1/binary/dsp_vector_release.bin,__dsp_vector_bin,__dsp_vector_section,4\
        --keep=__dsp_vector_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/demo_apps/power_mode_with_hifi/hifi1/binary/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
        --keep=__dsp_text_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/demo_apps/power_mode_with_hifi/hifi1/binary/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
        --keep=__dsp_data_bin"
)

# Change binary name
mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/core1_image.bin)
