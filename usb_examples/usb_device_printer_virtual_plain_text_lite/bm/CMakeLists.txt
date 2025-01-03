cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  dev_printer_virtual_plain_text_lite_bm
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "${board_root}/${board}/usb_examples/usb_device_printer_virtual_plain_text_lite/bm"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_iar_configuration(
  LD "--semihosting"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "examples/usb_examples/usb_device_printer_virtual_plain_text_lite/bm"
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
    "examples/usb_examples/usb_device_printer_virtual_plain_text_lite/bm/device_printer.c"
    "examples/usb_examples/usb_device_printer_virtual_plain_text_lite/bm/device_printer.h"
    "examples/usb_examples/usb_device_printer_virtual_plain_text_lite/bm/usb_device_ch9.c"
    "examples/usb_examples/usb_device_printer_virtual_plain_text_lite/bm/usb_device_ch9.h"
    "examples/usb_examples/usb_device_printer_virtual_plain_text_lite/bm/usb_device_descriptor.c"
    "examples/usb_examples/usb_device_printer_virtual_plain_text_lite/bm/usb_device_descriptor.h"
    "examples/usb_examples/usb_device_printer_virtual_plain_text_lite/bm/usb_device_printer.c"
    "examples/usb_examples/usb_device_printer_virtual_plain_text_lite/bm/usb_device_printer.h"
)

include("${SdkRootDirPath}/${board_root}/${board}/usb_examples/usb_device_printer_virtual_plain_text_lite/bm/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
