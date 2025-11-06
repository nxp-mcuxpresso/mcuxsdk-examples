cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(coex_wifi_central_ht LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/coex_examples/coex_wifi_central_ht)

include(${SdkRootDirPath}/examples/coex_examples/coex_wifi_central_ht/app_config.cmake)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/coex_examples/coex_wifi_central_ht
    TARGET edgefast_bluetooth_app.h
)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/coex_examples/coex_wifi_central_ht/${multicore_foldername}/edgefast_bluetooth_app.h
    CONFIG True
    PREINCLUDE True
    PREINCLUDE_TYPE c_include cpp_include
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/coex_examples/coex_wifi_central_ht/main.c
          examples/edgefast_bluetooth_examples/central_ht/central_ht.c
          examples/edgefast_bluetooth_examples/central_ht/central_ht.h
          examples/edgefast_bluetooth_examples/config/mbedtls_user_config.h
          middleware/wireless/coex/src/common/controller_coex_nxp.c
          middleware/wireless/coex/src/cli/coex_cli.c
          middleware/wireless/coex/src/cli/coex_cli.h
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES examples/coex_examples/coex_wifi_central_ht
           examples/edgefast_bluetooth_examples/central_ht
           examples/edgefast_bluetooth_examples/config
           middleware/wireless/coex/src/cli
)

mcux_add_macro(
  CC "-DCFG_BLE\
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

include(${SdkRootDirPath}/${board_root}/${board}/coex_examples/coex_wifi_central_ht/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/middleware/wireless/coex/third_party/third_party.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
