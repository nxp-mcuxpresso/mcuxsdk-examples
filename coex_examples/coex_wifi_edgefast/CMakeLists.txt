cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(coex_wifi_edgefast LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/coex_examples/coex_wifi_edgefast)

include(${SdkRootDirPath}/examples/coex_examples/coex_wifi_edgefast/app_config.cmake)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/coex/examples/coex_app/main.c
            middleware/edgefast_bluetooth/source/shell/shell_common.c
            examples/edgefast_bluetooth_examples/config/mbedtls_user_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wireless/coex/examples/coex_app
             middleware/edgefast_bluetooth/source/shell
             examples/edgefast_bluetooth_examples/config
)

# Add MCUXPRESSO IDE support
mcux_add_include(
    INCLUDES
        mcuxpresso_config
)

mcux_add_source(
    SOURCES mcuxpresso_config/app_mcuxpresso_config.h
)

include(${SdkRootDirPath}/${board_root}/${board}/coex_examples/coex_wifi_edgefast/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/middleware/wireless/coex/third_party/third_party.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)