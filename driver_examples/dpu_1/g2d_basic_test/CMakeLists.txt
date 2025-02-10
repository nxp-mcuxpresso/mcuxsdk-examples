
cmake_minimum_required(VERSION 3.22.0)

set(SdkRootDirPath /home/joshua/g2d/integration/sdk-next/mcuxsdk-core/mcuxsdk)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(g2d_basic_test LANGUAGES C PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/dpu/g2d_basic_test)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/dpu/g2d_basic_test/g2d_basic_test.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${PROJECT_BOARD_PORT_PATH}/${core_id}
             middleware/dpu/
)

# link the G2D-DPU library
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE 
    ${SdkRootDirPath}/middleware/dpu/cm7/libg2d.a
    -Wl,--start-group
    m c gcc
    -Wl,--end-group
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
        LINKER examples/_boards/${board}/driver_examples/dpu/g2d_basic_test/cm7/MIMX9596_cm7_ram.ld
)

target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PRIVATE
    LPTMR0=LPTMR1
    LPTMR0_IRQn=LPTMR1_IRQn
    CLOCK_GetFreq=CLOCK_GetIpFreq
    kCLOCK_LpoClk=0
    M7_CACHE_SUPPORTED=0
    BOARD_USE_ADP5585=1
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/m7_image.bin)