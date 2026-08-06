cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(coex_wifi_handsfree_ag LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/coex_examples/coex_wifi_handsfree_ag)

include(${SdkRootDirPath}/examples/coex_examples/coex_wifi_handsfree_ag/app_config.cmake)

include(${SdkRootDirPath}/CMakeLists.txt)

# Application sources: edgefast_open HFP AG example (read-only reuse) + coex
# app-local main / shell. The stock handsfree_ag main.c and app_shell.c are NOT
# pulled - the coex app provides its own (app-owned Clean Model B shell).
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        middleware/edgefast_open/examples/handsfree_ag/app_connect.c
        middleware/edgefast_open/examples/handsfree_ag/app_connect.h
        middleware/edgefast_open/examples/handsfree_ag/app_discover.c
        middleware/edgefast_open/examples/handsfree_ag/app_discover.h
        middleware/edgefast_open/examples/handsfree_ag/app_handsfree_ag.c
        middleware/edgefast_open/examples/handsfree_ag/app_handsfree_ag.h
        middleware/edgefast_open/examples/handsfree_ag/ringtone.h
        middleware/edgefast_open/examples/handsfree_ag/sco_audio_pl_ext.c
        middleware/edgefast_open/examples/handsfree_ag/sco_audio_pl_ext.h
        examples/coex_examples/coex_wifi_handsfree_ag/app_shell.c
        examples/coex_examples/coex_wifi_handsfree_ag/app_shell.h
        examples/coex_examples/coex_wifi_handsfree_ag/main.c
)

# The coex app dir MUST come before the stock handsfree_ag dir so that the
# app-local app_shell.h shadows the stock one.
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
        examples/coex_examples/coex_wifi_handsfree_ag
        middleware/edgefast_open/examples/handsfree_ag
)

include(${SdkRootDirPath}/${board_root}/${board}/coex_examples/coex_wifi_handsfree_ag/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
