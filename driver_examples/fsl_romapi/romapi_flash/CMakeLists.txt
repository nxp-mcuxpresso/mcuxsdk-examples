cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(k4w1_romapi_flash LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/fsl_romapi/romapi_flash)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/fsl_romapi/romapi_flash/romapi_flash.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/fsl_romapi/romapi_flash
             examples/_boards/${board}/driver_examples/fsl_romapi/romapi_flash/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/fsl_romapi/romapi_flash/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)