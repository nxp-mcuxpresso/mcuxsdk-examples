cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(powerquad_fir_fast LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/powerquad/fir_fast)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/powerquad/fir_fast/powerquad_fir_fast.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/powerquad/fir_fast/${core_id}
             examples/_boards/${board}/driver_examples/powerquad/fir_fast
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x8000"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x8000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x8000\""
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/powerquad/fir_fast/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
