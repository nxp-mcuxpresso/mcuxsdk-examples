cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tsi_v5_mutualmode LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/tsi_v5/tsi_v5_mutualmode)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/tsi/tsi_v5/tsi_v5_mutualmode/tsi_v5_mutualmode.c
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/tsi_v5/tsi_v5_mutualmode/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)