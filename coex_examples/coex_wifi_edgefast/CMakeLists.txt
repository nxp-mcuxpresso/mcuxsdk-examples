cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(coex_wifi_edgefast LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/coex_examples/coex_wifi_edgefast)

include(${SdkRootDirPath}/examples/coex_examples/coex_wifi_edgefast/app_config.cmake)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/coex/examples/coex_app/main.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wireless/coex/examples/coex_app
)

# Add MCUXPRESSO IDE support
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/coex_examples/coex_wifi_edgefast/mcuxpresso_config
             middleware/wireless/coex/third_party/mcuxpresso_support
             middleware/wireless/coex/third_party/mcuxpresso_support/lib
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/coex_examples/coex_wifi_edgefast/mcuxpresso_config/app_mcuxpresso_config.h
)

include(${SdkRootDirPath}/${board_root}/${board}/coex_examples/coex_wifi_edgefast/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/middleware/wireless/coex/third_party/third_party.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
