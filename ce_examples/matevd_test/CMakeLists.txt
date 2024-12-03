cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ce_matevd_test LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ce_examples/matevd_test)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ce_examples/matevd_test/ce_matevd_test.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/evd_input.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/evd_input_comp_loc_40.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/evd_input_comp_wgn_40.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/evd_input_loc.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/evd_input_loc_18.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/evd_input_loc_40.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/evd_input_rm_wgn_40.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/evd_input_wgn_17.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/evd_input_wgn_18.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/evd_input_wgn_40.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/qr_output_T_loc_18.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/qr_output_T_loc_40.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/qr_output_T_wgn_17.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/qr_output_T_wgn_18.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/qr_output_T_wgn_40.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/qr_output_U_loc_18.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/qr_output_U_loc_40.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/qr_output_U_wgn_17.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/qr_output_U_wgn_18.txt
    SOURCES examples/_boards/${board}/ce_examples/matevd_test/qr_output_U_wgn_40.txt
)

include(${SdkRootDirPath}/examples/_boards/${board}/ce_examples/matevd_test/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)