cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ce_fft_test LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ce_examples/fft_test)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ce_examples/fft_test/ce_fft_test.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/ce_examples/fft_test/fft32_cf16_input.txt
    SOURCES examples/_boards/${board}/ce_examples/fft_test/fft32_cf16_ref_output.txt
    SOURCES examples/_boards/${board}/ce_examples/fft_test/fft512_cf32_input.txt
    SOURCES examples/_boards/${board}/ce_examples/fft_test/fft512_cf32_ref_output.txt
    SOURCES examples/_boards/${board}/ce_examples/fft_test/ifft1024_cf16_input.txt
    SOURCES examples/_boards/${board}/ce_examples/fft_test/ifft1024_cf16_ref_output.txt
    SOURCES examples/_boards/${board}/ce_examples/fft_test/ifft256_cf32_input.txt
    SOURCES examples/_boards/${board}/ce_examples/fft_test/ifft256_cf32_ref_output.txt
)

include(${SdkRootDirPath}/examples/_boards/${board}/ce_examples/fft_test/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)