cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(camera_usb_final_fr_app_view LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH middleware/eiq/mpp/boards/${board}/examples/camera_usb_final_fr_app_view/${core_id})

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/mpp/camera_usb_final_fr_app_view/core1/camera_usb_final_fr_app_view.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/eiq_examples/mpp/examples/camera_usb_final_fr_app_view/${core_id}/
             middleware/eiq/mpp/internal/
             middleware/eiq/mpp/models/
             middleware/eiq/mpp/hal/include/
             middleware/eiq/mpp/internal/images/
             middleware/eiq/mpp/images/
)

# board & core-specific files
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp
    INCLUDES drivers
             inc/${core_id}
)
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/src/${core_id}/reconfig.cmake)

# top level
include(${SdkRootDirPath}/CMakeLists.txt)
# board & app specific cmake
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/examples/camera_usb_final_fr_app_view/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/../camera_usb_final_fr_app_view/${CONFIG_TOOLCHAIN}/core1_image.bin)
