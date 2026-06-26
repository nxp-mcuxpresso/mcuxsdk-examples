cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flashloader LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/bootloader_examples/flashloader)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/${board_root}/${board}/bootloader_examples/flashloader/reconfig.cmake OPTIONAL)

mcux_add_macro(
    CC "-DPINT_USE_SIMPLE_CALLBACK=1"
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
