cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(aon_lpuart_wakeup_mcxl_secondary
        LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH
                  ${board_root}/${board}/demo_apps/aon_lpuart_wakeup)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH
  ${SdkRootDirPath}
  SOURCES
  examples/demo_apps/aon_lpuart_wakeup_mcxl/secondary/aon_lpuart_wakeup_secondary.c
)

mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES
                 examples/demo_apps/aon_lpuart_wakeup_mcxl/secondary)

include(
  ${SdkRootDirPath}/${board_root}/${board}/demo_apps/aon_lpuart_wakeup/cm0plus/reconfig.cmake
  OPTIONAL)

mcux_add_linker_symbol(SYMBOLS __use_shmem__=1)

mcux_convert_binary(
  TOOLCHAINS armgcc mdk iar BINARY
  ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/core1_image.bin)

mcux_add_configuration(TOOLCHAINS armgcc iar mdk AS "-D__ENABLE_LP_BOOT=1")
