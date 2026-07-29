cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(coex_wifi_umr_open LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/coex_examples/coex_wifi_umr_open)

include(${SdkRootDirPath}/examples/coex_examples/coex_wifi_umr_open/app_config.cmake)

mcux_add_include(
 	BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/coex_examples/coex_wifi_umr_open
	TARGET edgefast_open_app.h
)
mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/coex_examples/coex_wifi_umr_open/${multicore_foldername}/edgefast_open_app.h
    CONFIG True
    PREINCLUDE True
    PREINCLUDE_TYPE c_include cpp_include
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    middleware/edgefast_open/examples/unicast_media_receiver/unicast_media_receiver.c
    middleware/edgefast_open/examples/unicast_media_receiver/unicast_media_receiver.h
    middleware/edgefast_open/examples/unicast_media_receiver/le_audio_common.c
    middleware/edgefast_open/examples/unicast_media_receiver/le_audio_common.h
    middleware/edgefast_open/examples/unicast_media_receiver/le_audio_service.c
    middleware/edgefast_open/examples/unicast_media_receiver/le_audio_service.h
    middleware/edgefast_open/examples/unicast_media_receiver/le_audio_shell.c
    middleware/edgefast_open/examples/unicast_media_receiver/le_audio_shell.h
    middleware/edgefast_open/examples/unicast_media_receiver/le_audio_sync.c
    middleware/edgefast_open/examples/unicast_media_receiver/le_audio_sync.h
    middleware/edgefast_open/examples/unicast_media_receiver/modules/audio_i2s.c
    middleware/edgefast_open/examples/unicast_media_receiver/modules/audio_i2s.h
    middleware/edgefast_open/examples/unicast_media_receiver/modules/hw_codec.c
    middleware/edgefast_open/examples/unicast_media_receiver/modules/hw_codec.h
    examples/coex_examples/coex_wifi_umr_open/main.c
    examples/_boards/${board}/coex_examples/coex_wifi_umr_open/${multicore_foldername}/coex_bt_encrypt.c
    middleware/wireless/coex/src/common/controller_coex_nxp.c
    middleware/wireless/coex/src/cli/coex_cli.c
    middleware/wireless/coex/src/cli/coex_cli.h
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    middleware/edgefast_open/examples/unicast_media_receiver
    middleware/edgefast_open/examples/unicast_media_receiver/modules
    examples/coex_examples/coex_wifi_umr_open
    middleware/wireless/coex/src/common
    middleware/wireless/coex/src/cli
)

mcux_add_macro(
  CC "-DCFG_BLE\
      -DLPUART_RING_BUFFER_SIZE=1024U\
      -DPRINTF_ADVANCED_ENABLE=1\
      -DPRINTF_FLOAT_ENABLE=1\
      -DSDK_DEBUGCONSOLE_UART=1\
      -DSDK_OS_FREE_RTOS\
      -DSHELL_TASK_PRIORITY=configMAX_PRIORITIES-2\
      -DSHELL_TASK_STACK_SIZE=2048\
      -DSHELL_USE_COMMON_TASK=0\
      -DLE_AUDIO_SYNC_ENABLE=1\
      -DUSE_RTOS=1"
)

mcux_add_iar_configuration(
    LD "--semihosting"
)

mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)

include(${SdkRootDirPath}/${board_root}/${board}/coex_examples/coex_wifi_umr_open/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
