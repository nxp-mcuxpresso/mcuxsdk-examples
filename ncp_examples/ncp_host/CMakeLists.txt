cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ncp_host LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ncp_examples/ncp_host)


include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES main.c
)

mcux_add_source(
    SOURCES ncp_host_app.h
            ncp_host_app.c
            ncp_host_command.h
            lwipopts.h
            lwippools.h
            FreeRTOSConfig.h
            system/ncp_host_app_system.c
            system/ncp_host_command_system.c
            wifi/ncp_host_app_wifi.c
            wifi/ncp_host_command_wifi.h
            wifi/ncp_host_command_wifi.c
            wifi/ncp_host_utils_wifi.h
            ble/ncp_host_app_ble.c
            ble/ncp_host_command_ble.h
            ble/ncp_host_command_ble.c
            ble/ble_service/service.h
            ble/ble_service/ht.h
            ble/ble_service/htc.c
            ble/ble_service/hts.c
            ble/ble_service/hr.h
            ble/ble_service/hrc.c
            ble/ble_service/hrs.c
            ble/ble_service/bas.h
            ble/ble_service/bas.c
            ot/ncp_host_app_ot.c
            ot/ncp_host_command_ot.c
            ot/ot_opcode/otopcode.c
            ot/ot_opcode/otopcode.h
            ot/ot_opcode/otopcode_private.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/ncp_examples/
    SOURCES common/ncp_cmd_common.h
            common/ncp_adapter/ncp_intf/usb/usb_host_cdc/ncp_intf_usb_host_cdc.c
            common/ncp_adapter/ncp_intf/usb/usb_host_cdc/ncp_intf_usb_host_cdc.h
            common/ncp_adapter/ncp_intf/usb/usb_host_cdc/ncp_usb_host_cdc.c
            common/ncp_adapter/ncp_intf/usb/usb_host_cdc/ncp_usb_host_cdc.h
            common/ncp_adapter/ncp_intf/usb/usb_host_cdc/usb_host_cdc_app.c
            common/ncp_adapter/ncp_intf/usb/usb_host_cdc/usb_host_cdc_app.h
            common/ncp_adapter/ncp_intf/usb/usb_host_cdc/usb_host_config.h
            common/ncp_adapter/ncp_intf/sdio/sdio_host/ncp_intf_sdio_host.c
            common/ncp_adapter/ncp_intf/sdio/sdio_host/ncp_intf_sdio_host.h
            common/ncp_adapter/ncp_intf/spi/spi_master/ncp_intf_spi_master.c
            common/ncp_adapter/ncp_intf/spi/spi_master/ncp_intf_spi_master.h
            common/crc/crc.c
            common/crc/crc.h
            common/ncp_adapter/ncp_tlv/ncp_tlv_adapter.c
            common/ncp_adapter/ncp_tlv/ncp_tlv_adapter.h
            common/ncp_adapter/ncp_adapter.h
            common/ncp_adapter/ncp_intf/spi/spi_master/ncp_intf_spi_master.h
            common/ncp_debug/ncp_debug.h
            common/ncp_cmd_system.h
            common/ncp_cmd_wifi.h
            common/ncp_cmd_ble.h
            common/ncp_adapter/ncp_intf/ncp_intf_pm.c
            common/ncp_adapter/ncp_intf/ncp_intf_pm.h
            common/ncp_adapter/ncp_intf/uart/ncp_intf_uart.c
            common/ncp_adapter/ncp_intf/uart/ncp_intf_uart.h
)

mcux_add_include(
    INCLUDES .
             system
             wifi
             ble
             ble/ble_service
             ot
             ot/ot_opcode
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/ncp_examples/
    INCLUDES common/ncp_adapter/ncp_intf/usb/usb_host_cdc
             common/ncp_adapter/ncp_intf/sdio/sdio_host
             common/ncp_adapter/ncp_intf/spi/spi_master
             common/crc
             common/ncp_adapter/ncp_tlv
             common/ncp_adapter
             common/ncp_debug
             common
             common/ncp_adapter/ncp_intf
             common/ncp_adapter/ncp_intf/uart
)
include(${SdkRootDirPath}/examples/_boards/${board}/ncp_examples/ncp_host/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)