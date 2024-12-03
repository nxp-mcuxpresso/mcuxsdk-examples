cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(nand_flash_management LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/nand_flash_management/semc)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/nand_flash_management/semc/nand_flash_management.c
            examples/demo_apps/nand_flash_management/semc/dhara_nand_device.c
            examples/demo_apps/nand_flash_management/semc/dhara_nand_config.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/nand_flash_management/semc/
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/nand_flash_management/semc/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)