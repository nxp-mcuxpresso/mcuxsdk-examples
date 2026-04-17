cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(nbu_ble LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        middleware/wireless/ble_controller/src/KW4x
    CUSTOM_PRJ_CONF_PATH
        ${board_root}/${board}/wireless_examples/ble_controller/nbu_ble
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/ble_controller/
    SOURCES src/KW4x/nbu_ble.c
            src/KW4x/app.h
            src/KW4x/board.h
            src/KW4x/board.c
)
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
    CX "        -DgBleBondIdentityHeaderSize_c=56 \
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
    AS "-DINIT_BLE_RL_DAT"
)

mcux_add_iar_configuration(
    TARGETS debug
    AS "-r"
    CC "-On --debug --no_cse --no_unroll --no_inline --no_code_motion --no_tbaa --no_clustering --no_scheduling"
    CX "--debug --no_cse --no_unroll --no_code_motion --no_tbaa --no_clustering --no_scheduling"
    LD "--semihosting"
)

mcux_add_iar_configuration(
    TARGETS  release
    CC "-Oh"
    CX "-Oh"
)

mcux_add_armgcc_configuration(
    TARGETS debug
    AS "-g"
    CC "-g -Og"
    CX "-g -Og"
    LD "-g"
)

mcux_add_armgcc_configuration(
    TARGETS release
    CC "-Os"
    CX "-Os"
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

# =============================================================================
# Build Timestamp
# =============================================================================
string(TIMESTAMP BUILD_DATE "%Y/%m/%d")
string(TIMESTAMP BUILD_TIME "%H:%M:%S")
# =============================================================================
# Manifest Repository Commit SHA
# =============================================================================
# Retrieve the short SHA from the west manifest repository.
# Falls back to "unknown"/"none" if west command fails or not in a west workspace.
# =============================================================================
execute_process(
  COMMAND west forall manifest -c "git rev-parse --short HEAD"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE GITCOMMIT_RAW
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
  RESULT_VARIABLE WEST_RESULT
)
if(WEST_RESULT EQUAL 0 AND GITCOMMIT_RAW)
  # Extract SHA: everything after the last colon
  string(REGEX REPLACE ".*:" "" GITCOMMIT "${GITCOMMIT_RAW}")
  string(STRIP "${GITCOMMIT}" GITCOMMIT)
  if(GITCOMMIT MATCHES "^[0-9a-fA-F]+$")
    # Valid SHA - keep GITCOMMIT as is
  else()
    set(GITCOMMIT "unknown")
  endif()
else()
  set(GITCOMMIT "none")
endif()
# =============================================================================
# Build User
# =============================================================================
set(BUILD_USER "unknown")
# =============================================================================
# Pass individual components
# =============================================================================
mcux_add_macro(COMMIT_INFO=\\\"${GITCOMMIT}\\\")
mcux_add_macro(BUILD_USER=\\\"${BUILD_USER}\\\")
mcux_add_macro(BUILD_DATE=\\\"${BUILD_DATE}\\\")
mcux_add_macro(BUILD_TIME=\\\"${BUILD_TIME}\\\")
# =============================================================================
# Build Output
# =============================================================================
message(STATUS "Build Date: ${BUILD_DATE} ${BUILD_TIME}")
message(STATUS "Manifest Commit: ${GITCOMMIT}")
