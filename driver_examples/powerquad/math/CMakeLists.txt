cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(powerquad_math LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/powerquad/math)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/powerquad/math/powerquad_math.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/powerquad/math/${core_id}
             ${board_root}/${board}/driver_examples/powerquad/math
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

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/powerquad/math/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
