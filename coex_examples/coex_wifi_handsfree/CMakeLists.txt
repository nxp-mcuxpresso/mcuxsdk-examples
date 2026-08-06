cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(coex_wifi_handsfree LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/coex_examples/coex_wifi_handsfree)

include(${SdkRootDirPath}/examples/coex_examples/coex_wifi_handsfree/app_config.cmake)

include(${SdkRootDirPath}/CMakeLists.txt)

# Application sources: edgefast_open HFP HF example (read-only reuse) + coex
# app-local main / shell. The stock handsfree main.c and app_shell.c are NOT
# pulled - the coex app provides its own (app-owned Clean Model B shell).
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        middleware/edgefast_open/examples/handsfree/app_connect.c
        middleware/edgefast_open/examples/handsfree/app_connect.h
        middleware/edgefast_open/examples/handsfree/app_handsfree.c
        middleware/edgefast_open/examples/handsfree/app_handsfree.h
        middleware/edgefast_open/examples/handsfree/ringtone.h
        middleware/edgefast_open/examples/handsfree/sco_audio_pl_ext.c
        middleware/edgefast_open/examples/handsfree/sco_audio_pl_ext.h
        examples/coex_examples/coex_wifi_handsfree/app_shell.c
        examples/coex_examples/coex_wifi_handsfree/app_shell.h
        examples/coex_examples/coex_wifi_handsfree/main.c
)

# The coex app dir MUST come before the stock handsfree dir so that the
# app-local app_shell.h shadows the stock one.
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
        examples/coex_examples/coex_wifi_handsfree
        middleware/edgefast_open/examples/handsfree
)

include(${SdkRootDirPath}/${board_root}/${board}/coex_examples/coex_wifi_handsfree/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
