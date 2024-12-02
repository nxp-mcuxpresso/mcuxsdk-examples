mcux_add_macro(
    CC "USE_OCOTP_DRIVER_IN_LOAD_SERVICE"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    PREINCLUDE TRUE
    SOURCES examples/_boards/${board}/el2go_examples/common/wifi/app_config.h
)