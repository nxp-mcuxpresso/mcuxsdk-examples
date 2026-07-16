cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(coex_wifi_a2dp_source LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/coex_examples/coex_wifi_a2dp_source)

include(${SdkRootDirPath}/examples/coex_examples/coex_wifi_a2dp_source/app_config.cmake)

include(${SdkRootDirPath}/CMakeLists.txt)

# Application sources: edgefast_open A2DP source example + coex app-local main / shell
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        middleware/edgefast_open/examples/a2dp_source/a2dp_media_data.h
        middleware/edgefast_open/examples/a2dp_source/app_a2dp_source.c
        middleware/edgefast_open/examples/a2dp_source/app_a2dp_source.h
        middleware/edgefast_open/examples/a2dp_source/app_connect.c
        middleware/edgefast_open/examples/a2dp_source/app_connect.h
        middleware/edgefast_open/examples/a2dp_source/app_discover.c
        middleware/edgefast_open/examples/a2dp_source/app_discover.h
        examples/coex_examples/coex_wifi_a2dp_source/app_shell.c
        examples/coex_examples/coex_wifi_a2dp_source/main.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
        examples/coex_examples/coex_wifi_a2dp_source
        middleware/edgefast_open/examples/a2dp_source
)

# Add MCUXPRESSO IDE support
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/coex_examples/coex_wifi_a2dp_source/mcuxpresso_config
             middleware/wireless/coex/third_party/mcuxpresso_support
             middleware/wireless/coex/third_party/mcuxpresso_support/lib
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/coex_examples/coex_wifi_a2dp_source/mcuxpresso_config/app_mcuxpresso_config.h
)

include(${SdkRootDirPath}/${board_root}/${board}/coex_examples/coex_wifi_a2dp_source/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/middleware/wireless/coex/third_party/third_party.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
