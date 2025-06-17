cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(nbu_ble LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        middleware/wireless/ble_controller/src/KW4x
    CUSTOM_PRJ_CONF_PATH
        ${board_root}/${board}/wireless_examples/ble_controller/nbu_ble
)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_configuration(
    CC "        -DgBleBondIdentityHeaderSize_c=56 \
                -DFSL_OSA_TASK_ENABLE=1\
                -DgUseHciTransportUpward_d=1\
                -DHCI_FREE_RxBuffer=0\
                -DRF_OSC_26MHZ=0\
                -DTEST_BUILD_COEX=0\
                -DFSL_RTOS_THREADX=1\
                -DgNbu_Hadm_d=1\
                -DTX_INCLUDE_USER_DEFINE_FILE"
)

mcux_add_configuration(
# define to force initialization of sqram_rldat_region after reset
# only required for GCC as IAR already initialize correctly this region
    TOOLCHAINS armgcc
    AS "        -DINIT_BLE_RL_DAT"
            
)
mcux_add_iar_configuration(
    TARGETS  release
    CC "     -Oh "
)
mcux_add_iar_configuration(
    TARGETS  debug
    CC "     --debug "
)
mcux_add_iar_configuration(
    TARGETS  debug
    LD "     --semihosting "
)

mcux_add_linker_symbol(
    SYMBOLS "__use_shmem__=1"
)

mcux_add_include(
    INCLUDES .
             ../
             interface
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/ble_controller/nbu_ble/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/nbu_ble.bin
)