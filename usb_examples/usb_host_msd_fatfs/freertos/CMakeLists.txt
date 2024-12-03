cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  host_msd_fatfs_freertos
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "examples/_boards/${board}/usb_examples/usb_host_msd_fatfs/freertos"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_iar_configuration(
  LD "--semihosting"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/usb_host_msd_fatfs/freertos"
)

mcux_add_macro(
  CC "-DPRINTF_ADVANCED_ENABLE=1 -DUSB_STACK_FREERTOS"
)

mcux_add_mdk_configuration(
  LD "--library_type=microlib"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/usb_host_msd_fatfs/freertos/app.c"
    "middleware/usb/example/usb_host_msd_fatfs/freertos/app.h"
    "middleware/usb/example/usb_host_msd_fatfs/freertos/host_msd_fatfs.c"
    "middleware/usb/example/usb_host_msd_fatfs/freertos/host_msd_fatfs.h"
)

include("${SdkRootDirPath}/examples/_boards/${board}/usb_examples/usb_host_msd_fatfs/freertos/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
