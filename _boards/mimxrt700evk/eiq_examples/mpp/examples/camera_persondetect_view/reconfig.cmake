# board+application specific cmake

mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES ${board_root}/${board}/eiq_examples/mpp/src/host_video.c
                ${board_root}/${board}/eiq_examples/mpp/src/image_process.c
                ${board_root}/${board}/eiq_examples/mpp/inc/host_video.h
                ${board_root}/${board}/eiq_examples/mpp/inc/image_process.h
)

mcux_add_macro(
    CC "-DRTOS_HEAP_SIZE=2500 \
    -DHAL_TIMER_PRECISION_1_US \
    -DconfigGENERATE_RUN_TIME_STATS=1 \
    -DUSE_UNCACHED_JPG_BUFFERS"
    CX "DconfigGENERATE_RUN_TIME_STATS=1 \
    -DHAL_TIMER_PRECISION_1_US \
    -DUSE_UNCACHED_JPG_BUFFERS"
)

#increase __ncache_size__ also increases the size of npu_ncache section accessible to NPU
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__ncache_size__=0x00380000"
)
