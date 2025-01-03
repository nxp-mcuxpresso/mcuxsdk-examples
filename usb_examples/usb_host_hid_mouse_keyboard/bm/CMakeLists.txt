cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  host_hid_mouse_keyboard_bm
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "${board_root}/${board}/usb_examples/usb_host_hid_mouse_keyboard/bm"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_iar_configuration(
  LD "--semihosting"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "examples/usb_examples/usb_host_hid_mouse_keyboard/bm"
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
    "examples/usb_examples/usb_host_hid_mouse_keyboard/bm/app.c"
    "examples/usb_examples/usb_host_hid_mouse_keyboard/bm/app.h"
    "examples/usb_examples/usb_host_hid_mouse_keyboard/bm/host_keyboard.c"
    "examples/usb_examples/usb_host_hid_mouse_keyboard/bm/host_keyboard.h"
    "examples/usb_examples/usb_host_hid_mouse_keyboard/bm/host_keyboard_mouse.h"
    "examples/usb_examples/usb_host_hid_mouse_keyboard/bm/host_mouse.c"
    "examples/usb_examples/usb_host_hid_mouse_keyboard/bm/host_mouse.h"
)

include("${SdkRootDirPath}/${board_root}/${board}/usb_examples/usb_host_hid_mouse_keyboard/bm/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
