# lists board specific files for SDK

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp
    INCLUDES inc
            }
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/eiq_examples/mpp/inc/FreeRTOSConfig.h
            ${board_root}/${board}/eiq_examples/mpp/inc/fsl_debug_console_conf.h
            ${board_root}/${board}/board.h
            ${board_root}/${board}/board.c
            ${board_root}/${board}/display_support.h
            ${board_root}/${board}/display_support.c
            middleware/eiq/mpp/hal/hal_${board}.c
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc355/0x0_1216 \
        -DCUSTOM_VGLITE_MEMORY_CONFIG=1 \
        -DBOARD_USE_PCAL6524=1 \
        -DSDK_I2C_BASED_COMPONENT_USED=1"
)

# core specific cmake
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/src/reconfig.cmake)
