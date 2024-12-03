cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(pdm_sai_interrupt LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/pdm/pdm_sai_interrupt)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/pdm/pdm_sai_interrupt/pdm_sai_interrupt.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/pdm/pdm_sai_interrupt
             examples/_boards/${board}/driver_examples/pdm/pdm_sai_interrupt/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/pdm/pdm_sai_interrupt/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)