mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/dsp_examples/pdm_sai
    INCLUDES ./
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/dsp_examples/pdm_sai/./dsp_config.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/dsp_examples/pdm_sai/fusionf1/./hardware_init.c
            ${board_root}/evkmimx8ulp/dsp_examples/pdm_sai/fusionf1/./app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/dsp_examples/pdm_sai/fusionf1/.
             ${board_root}/evkmimx8ulp
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