mcux_add_configuration(
    CC "-DPRINTF_FLOAT_ENABLE=1"
)

mcux_add_configuration(
    CC "-DBOARD_PMIC_CONFIG_USE_SEMA4=1"
)

mcux_add_macro(
    AS "-DDSP_IMAGE_COPY_TO_RAM=1"
    CC "-DDSP_IMAGE_COPY_TO_RAM=1"
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
    SOURCES ${board_root}/${board}/demo_apps/dvs_pvt_with_hifi/${core_id}/clock_config.c
            ${board_root}/${board}/demo_apps/dvs_pvt_with_hifi/${core_id}/clock_config.h
            ${board_root}/${board}/demo_apps/dvs_pvt_with_hifi/pvt_app_config.h
            ${board_root}/${board}/demo_apps/dvs_pvt_with_hifi/${core_id}/dvs_pvt_with_hifi_secondary.c
            ${board_root}/${board}/demo_apps/dsp_support.c
            ${board_root}/${board}/demo_apps/dsp_support.h
            ${board_root}/${board}/lpm.h
            ${board_root}/${board}/lpm.c
            ${board_root}/${board}/demo_apps/dvs_pvt_with_hifi/dsp_config.h
            ${board_root}/${board}/demo_apps/dvs_pvt_with_hifi/hifi1/binary/*.bin
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/dvs_pvt_with_hifi/${core_id}
             ${board_root}/${board}/demo_apps/dvs_pvt_with_hifi/
             ${board_root}/${board}/demo_apps/dvs_pvt_with_hifi/hifi1/binary/
             ${board_root}/${board}/demo_apps/
)

mcux_add_iar_configuration(
    LD "--image_input=${SdkRootDirPath}/${board_root}/${board}/demo_apps/dvs_pvt_with_hifi/hifi1/binary/dsp_vector_release.bin,__dsp_vector_bin,__dsp_vector_section,4\
        --keep=__dsp_vector_bin\
        --image_input=${SdkRootDirPath}/${board_root}/${board}/demo_apps/dvs_pvt_with_hifi/hifi1/binary/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
        --keep=__dsp_text_bin\
        --image_input=${SdkRootDirPath}/${board_root}/${board}/demo_apps/dvs_pvt_with_hifi/hifi1/binary/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
        --keep=__dsp_data_bin"
)

# Change binary name
mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/core1_image.bin)
