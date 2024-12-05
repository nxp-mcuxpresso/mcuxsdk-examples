cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lcdifv3_rgb565 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/lcdifv3/rgb565)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lcdifv3/rgb565/lcdifv3_rgb565.c
            ${board_root}/${board}/driver_examples/lcdifv3/lcdifv3_support.c
            ${board_root}/${board}/driver_examples/lcdifv3/lcdifv3_support.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/lcdifv3
             ${board_root}/${board}/driver_examples/lcdifv3/rgb565/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/lcdifv3/rgb565/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)