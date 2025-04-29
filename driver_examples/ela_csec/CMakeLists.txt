cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ela_csec LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/ela_csec)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/ela_csec/ela_csec.c
    SOURCES examples/driver_examples/ela_csec/ela_csec_utils.c
    SOURCES examples/driver_examples/ela_csec/ela_csec_utils.h
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/ela_csec/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)