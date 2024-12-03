
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/mmcau_examples/mmcau_api
)

mcux_add_mdk_configuration(
    CC "-Wno-implicit\
       -Os"
)

mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)
