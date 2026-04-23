cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(camera_usb_final_fr_app_view LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/eiq_examples/mpp/examples/camera_usb_final_fr_app_view/${core_id})

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/mpp/camera_usb_final_fr_app_view/camera_usb_final_fr_app_view.c
            ${board_root}/${board}/eiq_examples/mpp/examples/camera_usb_final_fr_app_view/${core_id}/mpp_config.h
            examples/eiq_examples/mpp/camera_usb_final_fr_app_view/event_listener.c
            examples/eiq_examples/mpp/camera_usb_final_fr_app_view/main.c
            examples/eiq_examples/mpp/camera_usb_final_fr_app_view/*.h
            examples/eiq_examples/mpp/camera_usb_final_fr_app_view/mpp_app_task_notify.c
            examples/eiq_examples/mpp/camera_usb_final_fr_app_view/mpp_app_task_notify.h
            middleware/eiq/mpp/common/draw_text.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/eiq_examples/mpp/examples/camera_usb_final_fr_app_view/${core_id}/
              examples/eiq_examples/mpp/camera_usb_final_fr_app_view/
)

# board files
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/board.cmake)

# model and post-processing
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/eiq/mpp
    INCLUDES    models
                internal/models
                internal/models/mobilefacenet
                internal/models/antispoofing
                internal/models/scrfd_kps_500m_full_integer_quant
                internal/face_recognition/utils
                internal/face_recognition
                middleware/eiq/mpp/common/
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/eiq/mpp
    SOURCES models/get_top_n.cpp
            models/utils.cpp
            models/get_top_n.h
            models/utils.h
            models/database_utils.h
            models/database_utils.c
            internal/models/mobilefacenet/mobilefacenet_output_postproc_quantized.cpp
            internal/models/mobilefacenet/*.h
            internal/models/antispoofing/antispoofing_output_postproc_quantized.cpp
            internal/face_recognition/utils/face_box_utils.c    
            internal/face_recognition/utils/face_box_utils.h
            internal/face_recognition/face_database.h
            internal/models/antispoofing/*.h
            internal/models/scrfd_kps_500m_full_integer_quant/scrfd_kps_output_postproc.c
            internal/models/scrfd_kps_500m_full_integer_quant/*.h
)

# top level
include(${SdkRootDirPath}/CMakeLists.txt)
# board & app specific cmake
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/examples/camera_usb_final_fr_app_view/${core_id}/reconfig.cmake OPTIONAL)

# When sysbuild is not enabled, use pre-built core1 image that drives the USB camera
if(NOT SYSBUILD)
   file(COPY ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/examples/camera_usb_final_fr_app_view/${core_id}/core1_image_QVGA_VGA.bin
        DESTINATION ${SdkRootDirPath}/examples/eiq_examples/mpp/camera_usb_final_fr_app_view/)
   file(RENAME ${SdkRootDirPath}/examples/eiq_examples/mpp/camera_usb_final_fr_app_view/core1_image_QVGA_VGA.bin
               ${SdkRootDirPath}/examples/eiq_examples/mpp/camera_usb_final_fr_app_view/core1_image.bin)
endif()

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

