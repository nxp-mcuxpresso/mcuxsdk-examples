cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(qsci_dma_rb_transfer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/qsci/dma_rb_transfer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/qsci/dma_rb_transfer/qsci_dma_rb_transfer.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/qsci/dma_rb_transfer
             ${board_root}/${board}/driver_examples/qsci/dma_rb_transfer/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/qsci/dma_rb_transfer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)