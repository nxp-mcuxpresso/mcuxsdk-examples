cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mqs_sai LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/sai/mqs_sai)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/sai/mqs_sai/mqs_sai.c
            examples/driver_examples/sai/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/sai/mqs_sai
             examples/driver_examples/sai
             ${board_root}/${board}/driver_examples/sai/mqs_sai/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/sai/mqs_sai/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)