# wifi
if(${CONFIG_WIFI})
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/coex_examples/coex_zigbee/wifi/config/lwipopts.h
            examples/_boards/${board}/coex_examples/coex_zigbee/wifi/config/lwiphooks.h
            examples/_boards/${board}/coex_examples/coex_zigbee/wifi/config/app_config.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    PREINCLUDE TRUE
    SOURCES examples/_boards/${board}/coex_examples/coex_zigbee/wifi/config/wifi_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/coex_examples/coex_zigbee/wifi/config
)

mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DCONFIG_MONOLITHIC_WIFI=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)

endif()