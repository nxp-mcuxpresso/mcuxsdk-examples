# Add additional configuration
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

# Linker file reconfiguration
include(${SdkRootDirPath}/${board_root}/${board}/ota_examples/ota_linker/linker_reconfig.cmake OPTIONAL)
