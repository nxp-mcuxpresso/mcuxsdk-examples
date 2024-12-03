
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(nbu_bb_bm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/bt_ble_examples/nbu_bb_bm)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/bt_ble_examples/nbu_bb_bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)