
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/tee_fault/tee_fault_common.h
)

mcux_add_macro(
    CC "-D__SEMIHOST_HARDFAULT_DISABLE=1"
)
