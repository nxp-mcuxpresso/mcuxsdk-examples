cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ecspi_loopback LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/ecspi/ecspi_loopback)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/ecspi/ecspi_loopback/ecspi_loopback.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/ecspi/ecspi_loopback
             ${board_root}/${board}/driver_examples/ecspi/ecspi_loopback/${core_id}
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=0\
       -DPRINTF_FLOAT_ENABLE=0\
       -DSCANF_ADVANCED_ENABLE=0\
       -DSCANF_FLOAT_ENABLE=0"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/ecspi/ecspi_loopback/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)