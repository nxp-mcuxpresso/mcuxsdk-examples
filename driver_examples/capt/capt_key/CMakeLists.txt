cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(capt_key LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/capt/capt_key)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES capt_key.c
            glitch_filter.c
            glitch_filter.h
            touch.h
            touch.c
            touch_hal.c
            touch_hal.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/driver_examples/capt/capt_key/app_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/driver_examples/capt/capt_key
             examples/_boards/${board}/driver_examples/capt/capt_key
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/capt/capt_key/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
