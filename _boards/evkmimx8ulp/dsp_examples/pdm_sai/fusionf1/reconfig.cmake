mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/dsp_examples/pdm_sai
    INCLUDES ./
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/dsp_examples/pdm_sai/./dsp_config.h
)

# Optimize compiler settings to reduce code size for IRAM memory constraints
mcux_remove_configuration(
    TARGETS debug
    CC "-O0 -g"
    CX "-O0 -g"
)

mcux_add_xtensa_configuration(
    TARGETS debug
    CC "-O1 -g"
    CX "-O1 -g"
)