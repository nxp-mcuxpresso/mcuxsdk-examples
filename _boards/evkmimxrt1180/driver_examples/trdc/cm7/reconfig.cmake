
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/multicore_examples
)

mcux_add_macro(
    CC "-DUSE_HYPERRAM=1"
)
