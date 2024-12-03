cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flexspi_octal_edma_transfer_unify LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/flexspi/octal/edma_transfer_unify)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexspi/octal/edma_transfer_unify/flexspi_octal_edma_transfer_unify.c
            examples/driver_examples/flexspi/common/nor_flash_common.c
            examples/driver_examples/flexspi/common/nor_flash_adesto.c
            examples/driver_examples/flexspi/common/nor_flash_gigadevice.c
            examples/driver_examples/flexspi/octal/edma_transfer_unify/flexspi_octal_flash_ops.c
            examples/driver_examples/flexspi/octal/edma_transfer_unify/flexspi_octal_flash_ops.h
            examples/driver_examples/flexspi/common/nor_flash.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/driver_examples/flexspi/common
             examples/_boards/${board}/driver_examples/flexspi
             examples/_boards/${board}/driver_examples/flexspi/octal/edma_transfer_unify/${core_id}
             examples/_boards/${board}/driver_examples/flexspi/octal/edma_transfer_unify
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/flexspi/octal/edma_transfer_unify/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)