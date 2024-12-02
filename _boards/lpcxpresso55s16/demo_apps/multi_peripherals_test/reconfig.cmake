
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/multi_peripherals_test/accelerometer.c
            examples/demo_apps/multi_peripherals_test/audio.c
            examples/demo_apps/multi_peripherals_test/music.h
            examples/demo_apps/multi_peripherals_test/led_blinky.c
            examples/demo_apps/multi_peripherals_test/mouse.c
            examples/demo_apps/multi_peripherals_test/mouse.h
            examples/demo_apps/multi_peripherals_test/rtc.c
            examples/demo_apps/multi_peripherals_test/usb_device_descriptor.c
            examples/demo_apps/multi_peripherals_test/usb_device_descriptor.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
            examples/_boards/${board}/demo_apps/multi_peripherals_test/demo_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/multi_peripherals_test
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_CODEC=1\
       -DCODEC_WM8904_ENABLE\
       -DUSB_STACK_FREERTOS\
       -DSDK_OS_FREE_RTOS\
       -DUSB_STACK_USE_DEDICATED_RAM=1\
       -DFSL_OSA_BM_TASK_ENABLE=0\
       -DSDK_DEBUGCONSOLE=1"
)
