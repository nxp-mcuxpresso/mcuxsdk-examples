cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  dev_composite_hid_audio_bm
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "examples/_boards/${board}/usb_examples/usb_device_composite_hid_audio/bm"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_iar_configuration(
  LD "--semihosting"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/usb_device_composite_hid_audio/bm"
)

mcux_add_macro(
  CC "-DUSB_STACK_BM"
)

mcux_add_mdk_configuration(
  LD "--library_type=microlib"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/usb_device_composite_hid_audio/bm/audio_generator.c"
    "middleware/usb/example/usb_device_composite_hid_audio/bm/audio_generator.h"
    "middleware/usb/example/usb_device_composite_hid_audio/bm/composite.c"
    "middleware/usb/example/usb_device_composite_hid_audio/bm/composite.h"
    "middleware/usb/example/usb_device_composite_hid_audio/bm/mouse.c"
    "middleware/usb/example/usb_device_composite_hid_audio/bm/mouse.h"
    "middleware/usb/example/usb_device_composite_hid_audio/bm/usb_device_descriptor.c"
    "middleware/usb/example/usb_device_composite_hid_audio/bm/usb_device_descriptor.h"
)

include("${SdkRootDirPath}/examples/_boards/${board}/usb_examples/usb_device_composite_hid_audio/bm/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
