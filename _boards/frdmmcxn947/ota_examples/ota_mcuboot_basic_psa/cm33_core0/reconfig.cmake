# Add additional configuration
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

# Linker file reconfiguration
include(${SdkRootDirPath}/examples/_boards/${board}/ota_examples/linker_common_ifr/linker_reconfig.cmake OPTIONAL)
