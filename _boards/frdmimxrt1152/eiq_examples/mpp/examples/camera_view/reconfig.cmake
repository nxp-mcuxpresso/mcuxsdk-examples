# board+application specific cmake

# Add host_video sources for USB camera support (compiled only when
# USE_USB_CAMERA is defined via a build-system option or cmake macro).
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/eiq/mpp/hal/camera_usb/host_video.c
            middleware/eiq/mpp/hal/camera_usb/host_video.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/eiq/mpp/hal/camera_usb
)

mcux_add_macro(
    CC "-DconfigGENERATE_RUN_TIME_STATS=1 \
        -DUSE_USB_CAMERA=1 \
        -DRTOS_HEAP_SIZE=8192"
)

# Extend newlib C heap from 32 KB default to 3 MB for libjpeg DCT buffers.
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__heap_size__=0x300000"
)
