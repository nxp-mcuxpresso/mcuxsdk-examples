cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sdu_example LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/sdu)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/sdu/sdu_example.c
            examples/driver_examples/sdu/sdu_example.h
            examples/driver_examples/sdu/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/sdu
             examples/_boards/${board}
             examples/_boards/${board}/driver_examples/sdu/${core_id}
)

mcux_add_macro(
    CC "-DCONFIG_SDIO_TEST_LOOPBACK=1\
       -DOSA_USED=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/sdu/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)