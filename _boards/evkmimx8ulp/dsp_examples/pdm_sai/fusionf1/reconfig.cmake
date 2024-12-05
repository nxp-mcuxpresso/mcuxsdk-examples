mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/dsp_examples/pdm_sai
    INCLUDES ./
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/dsp_examples/pdm_sai/./dsp_config.h
)

