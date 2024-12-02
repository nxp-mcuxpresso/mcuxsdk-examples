
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimxrt1180/demo_apps/iee_apc/ele_fw_baseline.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/demo_apps/iee_apc/
             examples/_boards/evkmimxrt1180/demo_apps/iee_apc/${core_id}
)

mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=0\
       -DSCANF_FLOAT_ENABLE=0\
       -DPRINTF_ADVANCED_ENABLE=0\
       -DSCANF_ADVANCED_ENABLE=0"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000"
)
