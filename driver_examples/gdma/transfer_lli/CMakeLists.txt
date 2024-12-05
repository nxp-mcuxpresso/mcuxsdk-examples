cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(gdma_transfer_lli LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/gdma/transfer_lli)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/gdma/transfer_lli/gdma_transfer_lli.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/gdma/transfer_lli
             ${board_root}/${board}/driver_examples/gdma/mem2mem
             ${board_root}/${board}/driver_examples/gdma/transfer_lli/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/gdma/transfer_lli/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)