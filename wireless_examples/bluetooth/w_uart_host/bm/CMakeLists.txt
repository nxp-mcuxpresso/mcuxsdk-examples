cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(w_uart_host_bm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH examples/wireless_examples/bluetooth/
        examples/_boards/${board}/wireless_examples/bluetooth/
        examples/_boards/${board}/wireless_examples/bluetooth/w_uart_host/${core_id}/bm
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/bluetooth/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    SOURCES
        bluetooth/examples/w_uart_host/app_config.c
        bluetooth/examples/w_uart_host/dynamic_database.c
        bluetooth/examples/w_uart_host/dynamic_database.h
        bluetooth/examples/w_uart_host/w_uart_application.c
        bluetooth/examples/w_uart_host/w_uart_application.h
        bluetooth/examples/w_uart_host/main.c
        ../../examples/wireless_examples/bluetooth/w_uart_host/readme.md
        ../../examples/_boards/${board}/wireless_examples/bluetooth/w_uart_host/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES 
        bluetooth/application/common
        bluetooth/examples/w_uart_host
)

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/bluetooth/boards/${board}/bluetooth/w_uart_host/app_preinclude.h
)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/bluetooth/w_uart_host/${multicore_foldername}/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/w_uart_host_${board}.srec
)