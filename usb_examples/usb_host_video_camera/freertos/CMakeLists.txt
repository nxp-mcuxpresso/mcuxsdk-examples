cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  host_video_camera_freertos
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "examples/_boards/${board}/usb_examples/usb_host_video_camera/freertos"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_iar_configuration(
  LD "--semihosting"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/usb_host_video_camera/freertos"
)

mcux_add_macro(
  CC "-DUSB_STACK_FREERTOS"
)

mcux_add_mdk_configuration(
  LD "--library_type=microlib"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/usb_host_video_camera/freertos/app.c"
    "middleware/usb/example/usb_host_video_camera/freertos/app.h"
    "middleware/usb/example/usb_host_video_camera/freertos/host_video.c"
    "middleware/usb/example/usb_host_video_camera/freertos/host_video.h"
)

include("${SdkRootDirPath}/examples/_boards/${board}/usb_examples/usb_host_video_camera/freertos/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
