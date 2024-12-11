
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lwip_iperf_enet_qos_bm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/lwip_examples/lwip_iperf_enet_qos/bm)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/lwip_examples/lwip_iperf/bm/lwip_iperf_bm.c
            examples/lwip_examples/lwip_iperf/bm/lwipopts.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/lwip_examples/lwip_iperf/bm
)

mcux_remove_macro(
    CC "-DCHECKSUM_GEN_UDP=1\
        -DCHECKSUM_GEN_TCP=1\
        -DCHECKSUM_GEN_ICMP=1\
        -DCHECKSUM_GEN_ICMP6=1\
        -DCHECKSUM_CHECK_IP=1\
        -DCHECKSUM_CHECK_UDP=1\
        -DCHECKSUM_CHECK_TCP=1\
        -DCHECKSUM_CHECK_ICMP=1\
        -DCHECKSUM_CHECK_ICMP6=1"
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
        -DSDK_DEBUGCONSOLE=1\
        -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING\
        -DCHECKSUM_GEN_IP=0\
        -DCHECKSUM_GEN_UDP=0\
        -DCHECKSUM_GEN_TCP=0\
        -DCHECKSUM_GEN_ICMP=0\
        -DCHECKSUM_GEN_ICMP6=0\
        -DCHECKSUM_CHECK_IP=0\
        -DCHECKSUM_CHECK_UDP=0\
        -DCHECKSUM_CHECK_TCP=0\
        -DCHECKSUM_CHECK_ICMP=0\
        -DCHECKSUM_CHECK_ICMP6=0\
        -DENET_RXBUFF_NUM=14\
        -DENET_RXBD_NUM=9"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x1000\
             __heap_size__=0x1000"
)

include(${SdkRootDirPath}/examples/_boards/${board}/lwip_examples/lwip_iperf_enet_qos/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)