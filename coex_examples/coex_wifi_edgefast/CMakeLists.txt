cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(coex_wifi_edgefast LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/coex_examples/coex_wifi_edgefast)

include(${SdkRootDirPath}/examples/coex_examples/coex_wifi_edgefast/app_config.cmake)

# coex defines
mcux_add_macro(
    CONFIG_WIFI_BLE_COEX_APP=${CONFIG_WIFI}
    CONFIG_DISABLE_BLE=!${CONFIG_BLE}
    CONFIG_OT_CLI=${CONFIG_OT}
    CONFIG_WPA_SUPP_MBEDTLS=${CONFIG_WPA_SUPPLICANT}
)

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

include(${SdkRootDirPath}/${board_root}/${board}/coex_examples/coex_wifi_edgefast/reconfig.cmake OPTIONAL)

if(${CONFIG_OT})
message(STATUS "Build with external ot-cli libs, CONFIG_OT=${CONFIG_OT}")
include(${SdkRootDirPath}/middleware/wireless/coex/third_party/third_party.cmake)
endif()

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)