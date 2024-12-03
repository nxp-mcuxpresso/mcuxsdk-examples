cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  dev_suspend_resume_hid_mouse_bm
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "examples/_boards/${board}/usb_examples/usb_suspend_resume_device_hid_mouse/bm"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_iar_configuration(
  LD "--semihosting"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/usb_suspend_resume_device_hid_mouse/bm"
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
    "middleware/usb/example/usb_suspend_resume_device_hid_mouse/bm/mouse.c"
    "middleware/usb/example/usb_suspend_resume_device_hid_mouse/bm/mouse.h"
    "middleware/usb/example/usb_suspend_resume_device_hid_mouse/bm/usb_device_descriptor.c"
    "middleware/usb/example/usb_suspend_resume_device_hid_mouse/bm/usb_device_descriptor.h"
)

include("${SdkRootDirPath}/examples/_boards/${board}/usb_examples/usb_suspend_resume_device_hid_mouse/bm/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
