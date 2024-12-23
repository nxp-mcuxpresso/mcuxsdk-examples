# coex defines
mcux_add_macro(
    CONFIG_COEX_ENABLE_BLE=${CONFIG_BLE}
    CONFIG_COEX_ENABLE_ZIGBEE=${CONFIG_ZIGBEE}
    CONFIG_COEX_ENABLE_WIFI=${CONFIG_WIFI}
    CONFIG_WPA_SUPP_MBEDTLS=${CONFIG_WPA_SUPPLICANT}
)

include(${SdkRootDirPath}/examples/_boards/${board}/coex_examples/coex_zigbee/edgefast/edgefast.cmake)

if(${CONFIG_WIFI})
include(${SdkRootDirPath}/examples/_boards/${board}/coex_examples/coex_zigbee/wifi/wifi.cmake)
if(${CONFIG_WPA_SUPPLICANT})
include(${SdkRootDirPath}/examples/_boards/${board}/coex_examples/coex_zigbee/wifi/wpa_supplicant/wpa_supplicant.cmake)
endif()
endif()

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/coex_examples/coex_zigbee/pin_mux.c
            examples/_boards/${board}/coex_examples/coex_zigbee/pin_mux.h
            examples/_boards/${board}/coex_examples/coex_zigbee/hardware_init.c
            examples/_boards/${board}/coex_examples/coex_zigbee/app.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/coex_examples/coex_zigbee
)

mcux_add_armgcc_configuration(
    TARGETS flash_release
    AS "-g"
    CC "-g"
    CX "-g"
)

mcux_remove_macro(
    TOOLCHAINS armgcc
    TARGETS flash_release
    AS "-DNDEBUG"
    CC "-DNDEBUG"
    CX "-DNDEBUG"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)
