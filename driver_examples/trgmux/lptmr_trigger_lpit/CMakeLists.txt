cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(trgmux_lptmr_trigger_lpit LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/trgmux/lptmr_trigger_lpit)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/driver_examples/trgmux/lptmr_trigger_lpit/trgmux_lptmr_trigger_lpit.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/trgmux/lptmr_trigger_lpit
             examples/_boards/${board}/driver_examples/trgmux/lptmr_trigger_lpit/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/trgmux/lptmr_trigger_lpit/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)