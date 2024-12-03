cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tsc_debugMode LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/tsc/debug/debugMode)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/tsc/debug/debugMode/debugMode.c
            examples/_boards/${board}/driver_examples/tsc/adc_config.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/tsc/debug/debugMode
             examples/_boards/${board}/driver_examples/tsc/debug/debugMode/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/tsc/debug/debugMode/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)