
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(central_tip LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/edgefast_bluetooth_examples/central_tip)

mcux_add_include(
 	BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/central_tip
	TARGET edgefast_bluetooth_app.h
)
mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/central_tip/${multicore_foldername}/edgefast_bluetooth_app.h
    CONFIG True
    PREINCLUDE True
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    middleware/edgefast_bluetooth/examples/central_tip/central_tip.c
    middleware/edgefast_bluetooth/examples/central_tip/central_tip.h
    middleware/edgefast_bluetooth/examples/central_tip/main.c
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES middleware/edgefast_bluetooth/examples/central_tip
)

mcux_add_macro(
  CC "-DCFG_BLE\
      -DLPUART_RING_BUFFER_SIZE=1024U\
      -DPRINTF_ADVANCED_ENABLE=1\
      -DSDK_DEBUGCONSOLE_UART=1\
      -DSDK_OS_FREE_RTOS\
      -DUSE_RTOS=1"
)

mcux_add_iar_configuration(
    LD "--semihosting"
)

mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)

include(${SdkRootDirPath}/examples/_boards/${board}/edgefast_bluetooth_examples/central_tip/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)