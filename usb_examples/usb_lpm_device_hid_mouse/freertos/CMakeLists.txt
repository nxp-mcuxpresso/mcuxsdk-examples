cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  dev_lpm_hid_mouse_freertos
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "${board_root}/${board}/usb_examples/usb_lpm_device_hid_mouse/freertos"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_iar_configuration(
  LD "--semihosting"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "examples/usb_examples/usb_lpm_device_hid_mouse/freertos"
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
    "examples/usb_examples/usb_lpm_device_hid_mouse/freertos/mouse.c"
    "examples/usb_examples/usb_lpm_device_hid_mouse/freertos/mouse.h"
    "examples/usb_examples/usb_lpm_device_hid_mouse/freertos/usb_device_descriptor.c"
    "examples/usb_examples/usb_lpm_device_hid_mouse/freertos/usb_device_descriptor.h"
)

include("${SdkRootDirPath}/${board_root}/${board}/usb_examples/usb_lpm_device_hid_mouse/freertos/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
