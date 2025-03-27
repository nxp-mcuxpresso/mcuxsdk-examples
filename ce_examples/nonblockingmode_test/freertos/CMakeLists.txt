cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ce_nonblockingmode_test_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/ce_examples/nonblockingmode_test/freertos)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ce_examples/nonblockingmode_test/freertos/ce_nbmode_test.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/ce_examples/nonblockingmode_test/freertos/mat_mult_input_A.txt
    SOURCES ${board_root}/${board}/ce_examples/nonblockingmode_test/freertos/mat_mult_input_B.txt
    SOURCES ${board_root}/${board}/ce_examples/nonblockingmode_test/freertos/mat_multcf32_40x40x40_C_ref.txt
    SOURCES ${board_root}/${board}/ce_examples/nonblockingmode_test/freertos/mat_multf32_60x60x60_C_ref.txt
)

include(${SdkRootDirPath}/${board_root}/${board}/ce_examples/nonblockingmode_test/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)