cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(coex_wifi_a2dp_source LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/coex_examples/coex_wifi_a2dp_source)

include(${SdkRootDirPath}/examples/coex_examples/coex_wifi_a2dp_source/app_config.cmake)

mcux_add_include(
 	BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/coex_examples/coex_wifi_a2dp_source
	TARGET edgefast_bluetooth_app.h
)
mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/coex_examples/coex_wifi_a2dp_source/${multicore_foldername}/edgefast_bluetooth_app.h
    CONFIG True
    PREINCLUDE True
    PREINCLUDE_TYPE c_include cpp_include
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    examples/edgefast_bluetooth_examples/a2dp_source/a2dp_pl_media_48KHz.h
    examples/edgefast_bluetooth_examples/a2dp_source/app_a2dp_source.c
    examples/edgefast_bluetooth_examples/a2dp_source/app_a2dp_source.h
    examples/edgefast_bluetooth_examples/a2dp_source/app_connect.c
    examples/edgefast_bluetooth_examples/a2dp_source/app_connect.h
    examples/edgefast_bluetooth_examples/a2dp_source/app_discover.c
    examples/edgefast_bluetooth_examples/a2dp_source/app_discover.h
    examples/coex_examples/coex_wifi_a2dp_source/app_shell.c
    examples/coex_examples/coex_wifi_a2dp_source/app_shell.h
    examples/coex_examples/coex_wifi_a2dp_source/main.c
    middleware/wireless/coex/src/common/controller_coex_nxp.c
    middleware/wireless/coex/src/cli/coex_cli.c
    middleware/wireless/coex/src/cli/coex_cli.h
    examples/edgefast_bluetooth_examples/config/mbedtls_user_config.h
    #middleware/wireless/coex/build/${board}/common/app.h
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES examples/edgefast_bluetooth_examples/a2dp_source
           examples/coex_examples/coex_wifi_a2dp_source
           middleware/wireless/coex/src/common
           middleware/wireless/coex/src/cli
           examples/edgefast_bluetooth_examples/config
           #middleware/wireless/coex/build/${board}/common
)

mcux_add_macro(
  CC "-DLPUART_RING_BUFFER_SIZE=1024U\
      -DPRINTF_ADVANCED_ENABLE=1\
      -DSDK_DEBUGCONSOLE_UART=1\
      -DSDK_OS_FREE_RTOS\
      -DSHELL_TASK_PRIORITY=configMAX_PRIORITIES-2\
      -DSHELL_USE_COMMON_TASK=0\
      -DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_user_config.h\\\"\
      -DUSE_RTOS=1"
)

mcux_add_iar_configuration(
    LD "--semihosting"
)

mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)

include(${SdkRootDirPath}/${board_root}/${board}/coex_examples/coex_wifi_a2dp_source/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
