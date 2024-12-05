cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(otfad_decrypt_flexspi_nor LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/otfad/decrypt)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/otfad/decrypt/flexspi_nor/otfad_decrypt.c
            examples/driver_examples/flexspi/nor/polling_transfer/flexspi_nor_flash_ops.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/otfad/decrypt/${core_id}
             ${board_root}/${board}/driver_examples/otfad/decrypt
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/otfad/decrypt/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)