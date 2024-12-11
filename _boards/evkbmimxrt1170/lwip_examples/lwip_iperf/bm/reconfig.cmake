mcux_remove_macro(
    CC "-DCHECKSUM_GEN_UDP=1\
        -DCHECKSUM_GEN_TCP=1\
        -DCHECKSUM_GEN_ICMP=1\
        -DCHECKSUM_CHECK_UDP=1\
        -DCHECKSUM_CHECK_TCP=1\
        -DCHECKSUM_CHECK_ICMP=1"
)

mcux_add_macro(
    CC "-DCHECKSUM_GEN_UDP=0\
        -DCHECKSUM_GEN_TCP=0\
        -DCHECKSUM_GEN_ICMP=0\
        -DCHECKSUM_CHECK_UDP=0\
        -DCHECKSUM_CHECK_TCP=0\
        -DCHECKSUM_CHECK_ICMP=0"
)

include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/reconfig.cmake OPTIONAL)