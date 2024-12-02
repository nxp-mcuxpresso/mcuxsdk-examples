
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/tsi/tsi_v5/tsi_v5/TSI_key.h
            examples/demo_apps/tsi/tsi_v5/tsi_v5/TSI_key.c
            examples/_boards/${board}/demo_apps/tsi_v5/tsi_v5_selfmode/TSI_config.c
            examples/_boards/${board}/demo_apps/tsi_v5/tsi_v5_selfmode/TSI_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/tsi/tsi_v5/tsi_v5
)

mcux_add_macro(
    CC "-DFRDM_KE17Z512\
       -DFREEDOM"
)
