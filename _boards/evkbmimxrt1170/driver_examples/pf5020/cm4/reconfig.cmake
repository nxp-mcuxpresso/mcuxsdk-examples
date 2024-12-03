
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/pf5020
)

mcux_add_macro(
    CC "-DSKIP_DCDC_CONFIGURATION=1"
)
