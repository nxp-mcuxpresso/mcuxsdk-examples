cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(cadc_expansion_mux_aux_opamp_err051396 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/cadc/expansion_mux_aux_opamp_err051396)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/dsc_cadc/expansion_mux_aux_opamp_err051396/cadc_expansion_mux_aux_opamp_err051396.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/cadc/expansion_mux_aux_opamp_err051396
             examples/_boards/${board}/driver_examples/cadc/expansion_mux_aux_opamp_err051396/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/cadc/expansion_mux_aux_opamp_err051396/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)