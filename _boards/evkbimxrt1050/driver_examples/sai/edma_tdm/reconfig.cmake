
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/driver_examples/sai/edma_tdm
)

mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
