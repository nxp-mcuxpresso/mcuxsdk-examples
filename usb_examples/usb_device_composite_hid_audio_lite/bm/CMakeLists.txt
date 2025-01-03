cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  dev_composite_hid_audio_lite_bm
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "${board_root}/${board}/usb_examples/usb_device_composite_hid_audio_lite/bm"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_iar_configuration(
  LD "--semihosting"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "examples/usb_examples/usb_device_composite_hid_audio_lite/bm"
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
    "examples/usb_examples/usb_device_composite_hid_audio_lite/bm/audio_generator.c"
    "examples/usb_examples/usb_device_composite_hid_audio_lite/bm/audio_generator.h"
    "examples/usb_examples/usb_device_composite_hid_audio_lite/bm/composite.c"
    "examples/usb_examples/usb_device_composite_hid_audio_lite/bm/composite.h"
    "examples/usb_examples/usb_device_composite_hid_audio_lite/bm/mouse.c"
    "examples/usb_examples/usb_device_composite_hid_audio_lite/bm/mouse.h"
    "examples/usb_examples/usb_device_composite_hid_audio_lite/bm/usb_device_audio.c"
    "examples/usb_examples/usb_device_composite_hid_audio_lite/bm/usb_device_audio.h"
    "examples/usb_examples/usb_device_composite_hid_audio_lite/bm/usb_device_ch9.c"
    "examples/usb_examples/usb_device_composite_hid_audio_lite/bm/usb_device_ch9.h"
    "examples/usb_examples/usb_device_composite_hid_audio_lite/bm/usb_device_descriptor.c"
    "examples/usb_examples/usb_device_composite_hid_audio_lite/bm/usb_device_descriptor.h"
    "examples/usb_examples/usb_device_composite_hid_audio_lite/bm/usb_device_hid.c"
    "examples/usb_examples/usb_device_composite_hid_audio_lite/bm/usb_device_hid.h"
)

include("${SdkRootDirPath}/${board_root}/${board}/usb_examples/usb_device_composite_hid_audio_lite/bm/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
