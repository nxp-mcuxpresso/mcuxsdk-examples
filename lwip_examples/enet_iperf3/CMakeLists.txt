cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lwip_enet_iperf3 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/lwip_examples/enet_iperf3)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/lwip_examples/iperf3/main.c
            examples/lwip_examples/iperf3/port/lwipopts.h
            examples/lwip_examples/iperf3/main.c
            examples/lwip_examples/iperf3/port/iperf_lwip.c
            examples/lwip_examples/iperf3/iperf_api.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/lwip_examples/iperf3/port
             examples/lwip_examples/iperf3
             examples/lwip_examples/iperf3/config_enet
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x1000\
             __heap_size__=0x1000"
)

mcux_add_macro(
    CC "-DIPERF3_ENET=1\
       -DSDK_DEBUGCONSOLE=1\
       -DSDK_DEBUGCONSOLE_UART=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSCANF_ADVANCED_ENABLE=1\
       -DDEBUG_CONSOLE_ENABLE_ECHO=1\
       -DUSE_RTOS=1\
       -DLWIP_NETIF_API=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/lwip_examples/enet_iperf3/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)