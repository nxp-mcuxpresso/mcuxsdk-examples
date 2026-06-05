cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(static_image_switch_ultraface_antispoofing_mobilefacenet_view LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/eiq_examples/mpp/examples/static_image_switch_ultraface_antispoofing_mobilefacenet_view)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/mpp/static_image_switch_ultraface_antispoofing_mobilefacenet_view/static_image_switch_ultraface_antispoofing_mobilefacenet_view.c
            ${board_root}/${board}/eiq_examples/mpp/examples/static_image_switch_ultraface_antispoofing_mobilefacenet_view/mpp_config.h
            examples/eiq_examples/mpp/static_image_switch_ultraface_antispoofing_mobilefacenet_view/event_listener.c
            examples/eiq_examples/mpp/static_image_switch_ultraface_antispoofing_mobilefacenet_view/*.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/eiq_examples/mpp/examples/static_image_switch_ultraface_antispoofing_mobilefacenet_view/
             middleware/eiq/mpp/examples/static_image_switch_ultraface_antispoofing_mobilefacenet_view/
)

# board files
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/board.cmake)
# model and post-processing
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/eiq/mpp
    INCLUDES    models
                models/ultraface_slim_quant_int8
                models/mobilefacenet
                models/antispoofing
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/eiq/mpp
    SOURCES models/get_top_n.cpp
            models/utils.cpp
            models/ultraface_slim_quant_int8/ultraface_output_postproc.c
            models/ultraface_slim_quant_int8/*.h
            models/get_top_n.h
            models/utils.h
            models/shell_database_utils.h
            models/shell_database_utils.c
            models/mobilefacenet/mobilefacenet_output_postproc_quantized.cpp
            models/mobilefacenet/*.h
            models/antispoofing/antispoofing_output_postproc_quantized.cpp
            models/face_box_utils.c
            models/face_box_utils.h
            models/antispoofing/*.h
	        images/jianfeng_qin_rgb.h 
            images/antispoofing_fake_rgb.h
)

# top level
include(${SdkRootDirPath}/CMakeLists.txt)
# board & app specific cmake
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/examples/static_image_switch_ultraface_antispoofing_mobilefacenet_view/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

