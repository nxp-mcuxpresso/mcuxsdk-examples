mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/lwip_examples/ipv4_ipv6_echo_common
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/lwip_examples/ipv4_ipv6_echo_common/shell_task.c
        examples/lwip_examples/ipv4_ipv6_echo_common/shell_task.h
        examples/lwip_examples/ipv4_ipv6_echo_common/shell_task_mode.h
        examples/lwip_examples/ipv4_ipv6_echo_common/socket_task.c
        examples/lwip_examples/ipv4_ipv6_echo_common/socket_task.h
)