cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(coex_wifi_edgefast LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/coex_examples/coex_wifi_edgefast)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/coex/examples/coex_app/main.c
            middleware/edgefast_bluetooth/source/shell/shell_common.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wireless/coex/examples/coex_app
)

include(${SdkRootDirPath}/examples/_boards/${board}/coex_examples/coex_wifi_edgefast/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)