# Add additional configuration
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

# Add additional configuration
mcux_add_macro(
    CC "-DMCUBOOT_OTA_SB3_SUPPORT"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ota_examples/_common/sb3_api/sb3_api.h
            examples/ota_examples/_common/sb3_api/sb3_api_mcxn10.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ota_examples/_common/sb3_api
)

# Linker file reconfiguration
include(${SdkRootDirPath}/${board_root}/${board}/ota_examples/ota_linker/linker_reconfig.cmake OPTIONAL)
