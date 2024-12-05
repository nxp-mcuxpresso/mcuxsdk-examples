cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sai_edma_tdm_record_playback LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/sai/edma_tdm_record_playback)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/sai/edma_tdm_record_playback/sai_edma_tdm_record_playback.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/sai/edma_tdm_record_playback
             ${board_root}/${board}/driver_examples/sai/edma_tdm_record_playback/${core_id}
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/sai/edma_tdm_record_playback/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)