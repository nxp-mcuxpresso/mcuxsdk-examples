cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  rom_dev_cdc_bm
  LANGUAGES
    "C"
    "CXX"
    "ASM" 
  PROJECT_BOARD_PORT_PATH "examples/_boards/${board}/usb_examples/usb_rom_device_cdc/bm"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_macro(
    CC "-DUSB_STACK_BM"
)

include("${SdkRootDirPath}/examples/_boards/${board}/usb_examples/usb_rom_device_cdc/bm/reconfig.cmake" OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
