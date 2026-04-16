
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/romapi/otp/romapi_otp.c
            ${board_root}/${board}/driver_examples/romapi/common/hardware_init.c
            ${board_root}/${board}/driver_examples/romapi/common/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/romapi/common
)