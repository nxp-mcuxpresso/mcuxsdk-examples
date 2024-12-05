cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(pdm_sai_sdma LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/pdm/pdm_sai_sdma)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/pdm/pdm_sai_sdma/pdm_sai_sdma.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/pdm/pdm_sai_sdma
             ${board_root}/${board}/driver_examples/pdm/pdm_sai_sdma/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/pdm/pdm_sai_sdma/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)