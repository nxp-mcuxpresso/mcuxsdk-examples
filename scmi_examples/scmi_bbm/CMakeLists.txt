cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(scmi_bbm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/scmi_examples/scmi_bbm)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/scmi_examples/scmi_bbm/scmi_bbm.c
)

include(${SdkRootDirPath}/${board_root}/${board}/scmi_examples/scmi_bbm/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
