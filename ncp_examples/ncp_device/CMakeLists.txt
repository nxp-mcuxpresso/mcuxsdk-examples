cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ncp_device LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/ncp_examples/ncp_device)

include(${SdkRootDirPath}/examples/ncp_examples/ncp_device/app_config.cmake)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES main.c
            lwipopts.h
            lwippools.h
            lwiphooks.h
            app_config.cmake
            controller_coex_nxp.c
            system/ncp_system.h
            system/ncp_system.c
            system/ncp_glue_system.h
            system/ncp_glue_system.c
            wifi/ncp_wifi.c
            wifi/ncp_wifi.h
            wifi/ncp_glue_wifi.c
            wifi/ncp_glue_wifi.h
            wifi/wifi_config.h
            ble/gap.c
            ble/gatt.c
            ble/l2cap.c
            ble/ncp_ble.c
            ble/ncp_ble.h
            ble/ncp_glue_ble.c
            ble/ncp_glue_ble.h
            ble/ble_config.h
            ble/service/gatt_server/peripheral_hrs.c
            ble/service/gatt_server/peripheral_hrs.h
            ble/service/gatt_server/peripheral_hts.c
            ble/service/gatt_server/peripheral_hts.h
            ble/service/gatt_server/peripheral_ncs.c
            ble/service/gatt_server/peripheral_ncs.h
            ble/service/gatt_server/peripheral_matter.c
            ble/service/gatt_server/peripheral_matter.h
            ble/service/gatt_client/central_hrc.c
            ble/service/gatt_client/central_hrc.h
            ble/service/gatt_client/central_htc.c
            ble/service/gatt_client/central_htc.h
            ble/service/bas.c
            ble/service/bas.h
            ble/service/service.c
            ble/service/service.h
            mbedtls/mbedtls_device.c
            mbedtls/mbedtls_device.h
            mbedtls/key_cert.h
            ot/third_party/ot_config.cmake
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/ncp_examples/ncp_device/
    SOURCES ot/peripherals.c
            ot/peripherals.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/ncp_examples/ncp_device/
    INCLUDES ot
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/ncp_examples/
    SOURCES common/ncp_cmd_common.h
            common/ncp_cmd_system.h
            common/ncp_cmd_wifi.h
            common/ncp_cmd_ble.h
            common/ncp_glue_common.c
            common/ncp_utils.c
            common/ncp_utils.h
            common/app_notify.h
            common/app_notify.c
            common/ncp_adapter/ncp_adapter.h
            common/ncp_adapter/ncp_tlv/ncp_tlv_adapter.c
            common/ncp_adapter/ncp_tlv/ncp_tlv_adapter.h
            common/ncp_adapter/ncp_intf/ncp_intf_pm.c
            common/ncp_adapter/ncp_intf/ncp_intf_pm.h
            common/ncp_adapter/ncp_intf/uart/ncp_intf_uart.c
            common/ncp_adapter/ncp_intf/uart/ncp_intf_uart.h
            common/ncp_adapter/ncp_intf/usb/usb_device_cdc/usb_device_cdc_app.c
            common/ncp_adapter/ncp_intf/usb/usb_device_cdc/usb_device_cdc_app.h
            common/ncp_adapter/ncp_intf/usb/usb_device_cdc/ncp_intf_usb_device_cdc.c
            common/ncp_adapter/ncp_intf/usb/usb_device_cdc/ncp_intf_usb_device_cdc.h
            common/ncp_adapter/ncp_intf/usb/usb_device_cdc/usb_device_descriptor.c
            common/ncp_adapter/ncp_intf/usb/usb_device_cdc/usb_device_descriptor.h
            #common/ncp_adapter/ncp_intf/usb/usb_device_cdc/usb_device_config.h
            common/ncp_adapter/ncp_intf/spi/spi_slave/ncp_intf_spi_slave.c
            common/ncp_adapter/ncp_intf/spi/spi_slave/ncp_intf_spi_slave.h
            common/ncp_adapter/ncp_intf/sdio/sdio_device/ncp_intf_sdio.c
            common/ncp_adapter/ncp_intf/sdio/sdio_device/ncp_intf_sdio.h
            common/ncp_config/littlefs_config.c
            common/ncp_config/littlefs_config.h
            common/ncp_config/littlefs_adapter.c
            common/ncp_config/littlefs_adapter.h
            common/ncp_config/ncp_config.c
            common/ncp_config/ncp_config.h
            common/mdns/mdns_service.c
            common/mdns/mdns_service.h
            common/lpm/ncp_lpm.c
            common/lpm/ncp_lpm.h
            common/lpm/host_sleep.c
            common/lpm/host_sleep.h
            common/lpm/host_sleep_wifi.c
            common/lpm/host_sleep_wifi.h
            common/crc/ncp_crc.c
            common/crc/ncp_crc.h
            common/mbedtls/mbedtls_common.c
            common/mbedtls/mbedtls_common.h
            common/mbedtls/mbedtls_config_client.h
            common/ncp_debug/ncp_debug.h
            common/httpc/inc/json_parser/jsmn.h
            common/httpc/inc/json_parser/json_generator.h
            common/httpc/inc/json_parser/json_parser.h
            common/httpc/inc/httpc.h
            common/httpc/inc/http-strings.h
            common/httpc/inc/serial_errno.h
            common/httpc/inc/serial_httpc.h
            common/httpc/inc/serial_network.h
            common/httpc/inc/serial_socket.h
            common/httpc/inc/websockets.h
            common/httpc/inc/wmstats.h
            common/httpc/src/httpc/httpc.c
            common/httpc/src/httpc/httpc_tests.c
            common/httpc/src/json_parser/json_generator.c
            common/httpc/src/serial_httpc/serial_httpc.c
            common/httpc/src/serial_httpc/serial_network.c
            common/httpc/src/serial_httpc/serial_socket.c
            common/httpc/src/websocket/websockets.c
            common/httpc/src/websocket/wslay.h
            common/httpc/src/websocket/wslay_frame.c
            common/httpc/src/websocket/wslay_frame.h
            common/httpc/src/websocket/wslayver.h
            common/uap_prov/httpsrv_fs_data.c
            common/uap_prov/http_server.c
            common/uap_prov/http_server.h
            common/uap_prov/uap_prov.c
            common/uap_prov/uap_prov.h
)

mcux_add_source(
    SOURCES wifi/inet/ncp_inet.h
            wifi/inet/ncp_inet.c
)

mcux_add_include(
    INCLUDES wifi/inet/
)

mcux_add_include(
    INCLUDES .
             system
             wifi
             ble
             ble/service
             ble/service/gatt_client
             ble/service/gatt_server
             mbedtls
             ot
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/ncp_examples/
    INCLUDES
        common
        common/ncp_adapter
        common/ncp_adapter/ncp_intf
        common/ncp_adapter/ncp_tlv
        common/ncp_adapter/ncp_intf/uart
        common/ncp_adapter/ncp_intf/usb/usb_device_cdc
        common/ncp_adapter/ncp_intf/spi/spi_slave
        common/ncp_adapter/ncp_intf/sdio/sdio_device
        common/ncp_config
        common/mdns
        common/lpm
        common/crc
        common/mbedtls
        common/ncp_debug
        common/httpc/inc
        common/httpc/inc/json_parser
        common/httpc/src/websocket
        common/uap_prov
)

include(${SdkRootDirPath}/examples/ncp_examples/ncp_device/ot/third_party/ot_config.cmake OPTIONAL)

include(${SdkRootDirPath}/${board_root}/${board}/ncp_examples/ncp_device/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
