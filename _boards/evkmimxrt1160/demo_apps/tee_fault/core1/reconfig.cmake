
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/tee_fault/tee_fault_common.h
            examples/_boards/${board}/demo_apps/tee_fault/core1/resource_config.h
            examples/_boards/${board}/demo_apps/tee_fault/core1/resource_config.c
)

mcux_add_macro(
    CC "-DFLEXSPI_IN_USE\
       -D__SEMIHOST_HARDFAULT_DISABLE=1"
)
