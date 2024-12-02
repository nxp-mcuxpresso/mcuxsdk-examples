cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(netc_psi_vsi_message_primary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/netc/psi_vsi_message)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/netc/psi_vsi_message/netc_psi_vsi_message_core0.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/netc/psi_vsi_message/${core_id}
)

mcux_add_macro(
    CC "-DMCMGR_HANDLE_EXCEPTIONS=1\
       -DSDK_NETC_USED=1\
       -D__SEMIHOST_HARDFAULT_DISABLE=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/netc/psi_vsi_message/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
# Settings to link to secondary core project
mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../netc_psi_vsi_message_secondary_core/iar/core1_image.bin,_core1_image,__core1_image,4 "
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../netc_psi_vsi_message_secondary_core/${CONFIG_TOOLCHAIN}/
)