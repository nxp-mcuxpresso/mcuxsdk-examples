# board+application specific cmake

mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES ${board_root}/${board}/eiq_examples/mpp/src/host_video.c
                ${board_root}/${board}/eiq_examples/mpp/src/image_process.c
                ${board_root}/${board}/eiq_examples/mpp/inc/host_video.h
                ${board_root}/${board}/eiq_examples/mpp/inc/image_process.h
    )

mcux_add_macro(
    CC "-DRTOS_HEAP_SIZE=200 \
    -DconfigGENERATE_RUN_TIME_STATS=1"
    CX "DconfigGENERATE_RUN_TIME_STATS=1"
)
