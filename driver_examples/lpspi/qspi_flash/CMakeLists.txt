cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lpspi_qspi_flash LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/lpspi/qspi_flash)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lpspi/qspi_flash/lpspi_qspi_flash.c
            examples/driver_examples/lpspi/qspi_flash/spi_dev_lpspi.c
            examples/driver_examples/lpspi/qspi_flash/spi_dev_lpspi.h
            examples/driver_examples/lpspi/qspi_flash/spi_dev.h
            examples/driver_examples/lpspi/qspi_flash/spi_flash.h
            examples/driver_examples/lpspi/qspi_flash/spi_flash.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/lpspi/qspi_flash
             examples/_boards/${board}/driver_examples/lpspi/qspi_flash/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/lpspi/qspi_flash/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)