mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-g"
    CX "-g"
)
mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Oz"
)

# Linker file reconfiguration
include(${SdkRootDirPath}/examples/_boards/${board}/ota_examples/linker_common_ifr/linker_reconfig.cmake OPTIONAL)
