# board+application specific cmake

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
    CC "-DRTOS_HEAP_SIZE=2000 \
    -DconfigGENERATE_RUN_TIME_STATS=1 \
    -DUSE_UNCACHED_JPG_BUFFERS \
    -DUSB_HOST_VIDEO_INST_NUM=2"
    CX "DconfigGENERATE_RUN_TIME_STATS=1 \
    -DUSE_UNCACHED_JPG_BUFFERS \
    -DUSB_HOST_VIDEO_INST_NUM=2"
)

#increase __ncache_size__ also increases the size of npu_ncache section accessible to NPU
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__ncache_size__=0x00400000"
)
