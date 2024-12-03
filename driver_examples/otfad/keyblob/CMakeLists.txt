cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(otfad_keyblob LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/otfad/keyblob)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/otfad/keyblob/otfad_keyblob.c
            examples/driver_examples/flexspi/nor/polling_transfer/flexspi_nor_flash_ops.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/otfad/keyblob/${core_id}
             examples/_boards/${board}/driver_examples/otfad/keyblob
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/otfad/keyblob/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)