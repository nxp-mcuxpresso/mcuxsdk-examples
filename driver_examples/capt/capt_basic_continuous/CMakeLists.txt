cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(capt_basic_continuous LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/capt/capt_basic_continuous)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/capt/capt_basic_continuous/capt_basic_continuous.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/capt/capt_basic_continuous
             examples/_boards/${board}/driver_examples/capt/capt_basic_continuous/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/capt/capt_basic_continuous/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)