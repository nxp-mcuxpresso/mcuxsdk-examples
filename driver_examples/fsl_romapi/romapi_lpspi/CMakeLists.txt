cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(k4w1_romapi_lpspi LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/fsl_romapi/romapi_lpspi)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/fsl_romapi/romapi_lpspi/romapi_lpspi.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/fsl_romapi/romapi_lpspi
             ${board_root}/${board}/driver_examples/fsl_romapi/romapi_lpspi/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/fsl_romapi/romapi_lpspi/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)