cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(casper LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/casper)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/casper/casper.c
            examples/driver_examples/casper/test_ecdoublemul256.h
            examples/driver_examples/casper/test_ecdoublemul384.h
            examples/driver_examples/casper/test_ecdoublemul521.h
            examples/driver_examples/casper/test_ecmul256.h
            examples/driver_examples/casper/test_ecmul384.h
            examples/driver_examples/casper/test_ecmul521.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/casper
             examples/_boards/${board}/driver_examples/casper/${core_id}
)

mcux_add_armgcc_configuration(
    CC "-fno-strict-aliasing"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x800"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x800"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x800\""
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/casper/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)