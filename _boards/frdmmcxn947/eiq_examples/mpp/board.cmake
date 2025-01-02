# lists board specific files for SDK

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/eiq_examples/mpp
    INCLUDES inc
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/eiq_examples/mpp/src/pin_mux.c
            examples/_boards/${board}/eiq_examples/mpp/src/hardware_init.c
            examples/_boards/${board}/eiq_examples/mpp/src/board.c
            examples/_boards/${board}/eiq_examples/mpp/src/clock_config.c
            examples/_boards/${board}/eiq_examples/mpp/src/utick_config.c
            examples/_boards/${board}/eiq_examples/mpp/inc/pin_mux.h
            examples/_boards/${board}/eiq_examples/mpp/inc/app.h
            examples/_boards/${board}/eiq_examples/mpp/inc/clock_config.h
            examples/_boards/${board}/eiq_examples/mpp/inc/utick_config.h
            examples/_boards/${board}/eiq_examples/mpp/inc/FreeRTOSConfig.h
            middleware/eiq/mpp/hal/hal_${board}.c
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1 \
        -DSSD1963_DATA_WITDH=8 \
        -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
        -DSDK_DEBUGCONSOLE_UART \
        -DARM_MATH_CM33 \
        -D__FPU_PRESENT=1"
    CX "-DSDK_DEBUGCONSOLE_UART\
        -DARM_MATH_CM33\
        -D__FPU_PRESENT=1"
)

mcux_remove_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)

mcux_remove_armgcc_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)

mcux_remove_armgcc_configuration(
  TARGETS flash_release flash_debug
  CC "-Os"
  CX "-Os"
  )

mcux_add_armgcc_configuration(
  TARGETS flash_release flash_debug
  CC "-O3"
  CX "-O3"
  )

mcux_add_armgcc_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/eiq_examples/mpp
    LINKER MCXN947_cm33_core0_flash.ld
)
