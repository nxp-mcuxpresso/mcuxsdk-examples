cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  dev_composite_hid_audio_lite_bm
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "examples/_boards/${board}/usb_examples/usb_device_composite_hid_audio_lite/bm"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_iar_configuration(
  LD "--semihosting"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/usb_device_composite_hid_audio_lite/bm"
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
    "middleware/usb/example/usb_device_composite_hid_audio_lite/bm/audio_generator.c"
    "middleware/usb/example/usb_device_composite_hid_audio_lite/bm/audio_generator.h"
    "middleware/usb/example/usb_device_composite_hid_audio_lite/bm/composite.c"
    "middleware/usb/example/usb_device_composite_hid_audio_lite/bm/composite.h"
    "middleware/usb/example/usb_device_composite_hid_audio_lite/bm/mouse.c"
    "middleware/usb/example/usb_device_composite_hid_audio_lite/bm/mouse.h"
    "middleware/usb/example/usb_device_composite_hid_audio_lite/bm/usb_device_audio.c"
    "middleware/usb/example/usb_device_composite_hid_audio_lite/bm/usb_device_audio.h"
    "middleware/usb/example/usb_device_composite_hid_audio_lite/bm/usb_device_ch9.c"
    "middleware/usb/example/usb_device_composite_hid_audio_lite/bm/usb_device_ch9.h"
    "middleware/usb/example/usb_device_composite_hid_audio_lite/bm/usb_device_descriptor.c"
    "middleware/usb/example/usb_device_composite_hid_audio_lite/bm/usb_device_descriptor.h"
    "middleware/usb/example/usb_device_composite_hid_audio_lite/bm/usb_device_hid.c"
    "middleware/usb/example/usb_device_composite_hid_audio_lite/bm/usb_device_hid.h"
)

include("${SdkRootDirPath}/examples/_boards/${board}/usb_examples/usb_device_composite_hid_audio_lite/bm/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
