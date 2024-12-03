cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(cadc_expansion_mux_aux_opamp LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/cadc/expansion_mux_aux_opamp)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/dsc_cadc/expansion_mux_aux_opamp/cadc_expansion_mux_aux_opamp.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/cadc/expansion_mux_aux_opamp
             examples/_boards/${board}/driver_examples/cadc/expansion_mux_aux_opamp/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/cadc/expansion_mux_aux_opamp/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)