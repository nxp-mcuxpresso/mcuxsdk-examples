cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(cmsis_ecspi_sdma_loopback_transfer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/cmsis_driver_examples/ecspi/sdma_loopback_transfer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/cmsis_driver_examples/ecspi/sdma_loopback_transfer/cmsis_ecspi_sdma_loopback_transfer.c
            ${board_root}/${board}/cmsis_driver_examples/ecspi/sdma_loopback_transfer/RTE_Device.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/cmsis_driver_examples/ecspi/sdma_loopback_transfer
             ${board_root}/${board}/cmsis_driver_examples/ecspi/sdma_loopback_transfer/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/cmsis_driver_examples/ecspi/sdma_loopback_transfer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)