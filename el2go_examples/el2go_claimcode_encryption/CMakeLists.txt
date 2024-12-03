cmake_minimum_required(VERSION 3.30.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(el2go_claimcode_encryption LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/el2go_examples/el2go_claimcode_encryption)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/el2go_examples/el2go_claimcode_encryption/reconfig.cmake OPTIONAL)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/nxp_iot_agent/ex/src/apps/el2go_claimcode_encryption.c
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)