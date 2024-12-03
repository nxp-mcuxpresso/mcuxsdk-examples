cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(meterlibFFT1ph_test LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/meterlibFFT1ph_test)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_macro(
  CC "SDK_DEBUGCONSOLE_UART"
  )

mcux_add_configuration(
    TOOLCHAINS armgcc mdk
    CC "-ffreestanding"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/_boards/${board}/demo_apps/meterlibFFT1ph_test/${multicore_foldername}/meterlibfft1ph_test.c
        examples/_boards/${board}/demo_apps/meterlibFFT1ph_test/${multicore_foldername}/inputdata.h
        examples/_boards/${board}/demo_apps/meterlibFFT1ph_test/${multicore_foldername}/board.c
        examples/_boards/${board}/demo_apps/meterlibFFT1ph_test/${multicore_foldername}/board.h
        examples/_boards/${board}/demo_apps/meterlibFFT1ph_test/${multicore_foldername}/clock_config.c
        examples/_boards/${board}/demo_apps/meterlibFFT1ph_test/${multicore_foldername}/clock_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/demo_apps/meterlibFFT1ph_test/${multicore_foldername}
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/meterlibFFT1ph_test/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
