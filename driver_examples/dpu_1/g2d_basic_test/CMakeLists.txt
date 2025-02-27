
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dpu_1_g2d_basic_test LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/dpu/g2d_basic_test)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(SOURCES
    g2d_basic_test.c
)

# link the G2D-DPU library
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libg2d.a
    m
)

mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/i.MX/i.MX95/MIMX9596/gcc/MIMX9596xxxxN_cm7_ram.ld
)

# add a custom linker with increased heap size
mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER examples/_boards/${board}/driver_examples/dpu/g2d_basic_test/cm7/linker_g2d.ld
)

target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PRIVATE
    BOARD_USE_ADP5585=1
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
