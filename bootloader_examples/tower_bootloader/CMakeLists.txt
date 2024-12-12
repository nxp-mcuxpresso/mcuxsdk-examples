cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tower_bootloader LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/bootloader_examples/tower_bootloader)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/bootloader_examples/tower_bootloader/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)