cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sai_edma_standalone_transfer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/sai/edma_standalone_transfer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/sai/edma_standalone_transfer/sai_edma_standalone_transfer.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/sai/edma_standalone_transfer
             examples/driver_examples/sai
             ${board_root}/${board}/driver_examples/sai/edma_standalone_transfer/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/sai/edma_standalone_transfer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)