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
include(${SdkRootDirPath}/${board_root}/${board}/ota_examples/ota_linker/linker_reconfig.cmake OPTIONAL)
