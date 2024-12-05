cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(anactrl_measure_frequency LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/anactrl/measure_frequency)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/anactrl/measure_frequency/anactrl_measure_frequency.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/anactrl/measure_frequency
             ${board_root}/${board}/driver_examples/anactrl/measure_frequency/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/anactrl/measure_frequency/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)