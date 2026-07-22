cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(static_image_nanodet_persondetect_view LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/eiq_examples/mpp/examples/static_image_nanodet_persondetect_view)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/mpp/static_image_nanodet_persondetect_view/static_image_nanodet_persondetect_view.c
            ${board_root}/${board}/eiq_examples/mpp/examples/static_image_nanodet_persondetect_view/mpp_config.h
            middleware/eiq/mpp/images/skigirl_COCO_320_320_bgra.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/eiq_examples/mpp/examples/static_image_nanodet_persondetect_view/
    middleware/eiq/mpp/images/
)

# board files
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/board.cmake)
# model and post-processing
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/eiq/mpp/models
    INCLUDES    .
                nanodet_m_320_quant_int8
                persondetect
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/eiq/mpp/models
    SOURCES get_top_n.cpp
            utils.cpp
            nanodet_m_320_quant_int8/nanodet_m_output_postproc.cpp
            nanodet_m_320_quant_int8/*.h
            persondetect/persondetect_output_postprocess.c
            persondetect/*.h
            get_top_n.h
            utils.h
)

# top level
include(${SdkRootDirPath}/CMakeLists.txt)
# board & app specific cmake
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/examples/static_image_nanodet_persondetect_view/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

