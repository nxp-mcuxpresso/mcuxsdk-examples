
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tmap_peripheral LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/edgefast_bluetooth_examples/tmap_peripheral)

mcux_add_include(
 	BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/tmap_peripheral
	TARGET edgefast_bluetooth_app.h
)
mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/tmap_peripheral/${multicore_foldername}/edgefast_bluetooth_app.h
    CONFIG True
    PREINCLUDE True
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    middleware/edgefast_bluetooth/examples/tmap_peripheral/bap_unicast_sr.c
    middleware/edgefast_bluetooth/examples/tmap_peripheral/ccp_call_ctrl.c
    middleware/edgefast_bluetooth/examples/tmap_peripheral/csip_set_member.c
    middleware/edgefast_bluetooth/examples/tmap_peripheral/main.c
    middleware/edgefast_bluetooth/examples/tmap_peripheral/mcp_ctlr.c
    middleware/edgefast_bluetooth/examples/tmap_peripheral/tmap_peripheral.c
    middleware/edgefast_bluetooth/examples/tmap_peripheral/tmap_peripheral.h
    middleware/edgefast_bluetooth/examples/tmap_peripheral/vcp_vol_renderer.c
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES middleware/edgefast_bluetooth/examples/tmap_peripheral
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

include(${SdkRootDirPath}/examples/_boards/${board}/edgefast_bluetooth_examples/tmap_peripheral/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)