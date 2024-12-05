cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mmu_xspi LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/mmu/xspi)


include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/mmu/xspi
             ${board_root}/${board}/driver_examples/mmu/xspi/${core_id}
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/mmu/xspi/mmu_xspi.c
)

mcux_add_configuration(
    CC "-DPRINTF_ADVANCED_ENABLE=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/mmu/xspi/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
