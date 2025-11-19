cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(camera_usb_final_fr_app_view_core1 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH middleware/eiq/mpp/boards/${board}/examples/camera_usb_final_fr_app_view_core1)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/mpp/camera_usb_final_fr_app_view_core1/camera_usb_final_fr_app_view_core1.c
            middleware/eiq/mpp/internal/images/kaggle_img_rgb_320_yuyv_jpg.h
            middleware/eiq/mpp/internal/images/kaggle_img_ir_320_yuyv_jpg.h
    )

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES m${board_root}/${board}/eiq_examples/mpp/examples/camera_usb_final_fr_app_view_core1/
             middleware/eiq/mpp/internal/
             middleware/eiq/mpp/models/
             middleware/eiq/mpp/hal/include/
             middleware/eiq/mpp/internal/images/
)

# core-specific files
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/${core_id}/reconfig.cmake)

# top level
include(${SdkRootDirPath}/CMakeLists.txt)
# board & app specific cmake
include(${SdkRootDirPath}${board_root}/${board}/eiq_examples/mpp/examples/camera_usb_final_fr_app_view_core1/reconfig.cmake OPTIONAL)

mcux_convert_binary(
    BINARY ${SdkRootDirPath}/middleware/eiq/mpp/boards/${board}/drivers/core1_image.bin
)
