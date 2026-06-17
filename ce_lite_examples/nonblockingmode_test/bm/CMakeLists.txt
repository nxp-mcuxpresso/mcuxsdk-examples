cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ce_lite_nonblockingmode_test_bm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/ce_lite_examples/nonblockingmode_test/bm)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ce_lite_examples/nonblockingmode_test/bm/ce_nbmode_test.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/ce_lite_examples/nonblockingmode_test/bm/fft32_cf16_input.txt
    SOURCES ${board_root}/${board}/ce_lite_examples/nonblockingmode_test/bm/fft32_cf16_ref_output.txt
    SOURCES ${board_root}/${board}/ce_lite_examples/nonblockingmode_test/bm/fft512_cf32_input.txt
    SOURCES ${board_root}/${board}/ce_lite_examples/nonblockingmode_test/bm/fft512_cf32_ref_output.txt
)

include(${SdkRootDirPath}/${board_root}/${board}/ce_lite_examples/nonblockingmode_test/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
