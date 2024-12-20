cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mcxromapi_flash_hiding LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/romapi/flashhiding)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/mcx_romapi/flashhiding/flash_hiding.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/romapi/flashhiding
             examples/_boards/${board}/driver_examples/romapi/flashhiding/${core_id}
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0xC00"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0xC00\""
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0xC00"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/romapi/flashhiding/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)