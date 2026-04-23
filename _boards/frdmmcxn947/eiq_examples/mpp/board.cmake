# lists board specific files for SDK

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp
    INCLUDES inc
             inc/${core_id}
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/eiq/mpp/hal/hal_${board}.c
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1 \
        -DSDK_DEBUGCONSOLE_UART \
        -DARM_MATH_CM33"
    CX "-DSDK_DEBUGCONSOLE_UART \
        -DARM_MATH_CM33"
)

# core specific cmake
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/src/${core_id}/reconfig.cmake)
