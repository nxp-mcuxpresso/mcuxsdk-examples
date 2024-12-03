
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/driver_examples/romapi/otp/romapi_otp.c
            examples/_boards/${board}/driver_examples/romapi/common/pin_mux.c
            examples/_boards/${board}/driver_examples/romapi/common/pin_mux.h
            examples/_boards/${board}/driver_examples/romapi/common/hardware_init.c
            examples/_boards/${board}/driver_examples/romapi/common/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/romapi/common
)