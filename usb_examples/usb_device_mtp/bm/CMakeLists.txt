cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  dev_mtp_bm
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "examples/_boards/${board}/usb_examples/usb_device_mtp/bm"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_iar_configuration(
  LD "--semihosting"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/usb_device_mtp/bm"
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
    "middleware/usb/example/usb_device_mtp/bm/mtp.c"
    "middleware/usb/example/usb_device_mtp/bm/mtp.h"
    "middleware/usb/example/usb_device_mtp/bm/mtp_file_system_adapter.c"
    "middleware/usb/example/usb_device_mtp/bm/mtp_file_system_adapter.h"
    "middleware/usb/example/usb_device_mtp/bm/mtp_object_handle.c"
    "middleware/usb/example/usb_device_mtp/bm/mtp_object_handle.h"
    "middleware/usb/example/usb_device_mtp/bm/mtp_operation.c"
    "middleware/usb/example/usb_device_mtp/bm/mtp_operation.h"
    "middleware/usb/example/usb_device_mtp/bm/usb_device_descriptor.c"
    "middleware/usb/example/usb_device_mtp/bm/usb_device_descriptor.h"
)

include("${SdkRootDirPath}/examples/_boards/${board}/usb_examples/usb_device_mtp/bm/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
