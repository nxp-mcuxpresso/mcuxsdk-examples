
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/wireless_uart_bridge/app_config.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/app_config.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/FreeRTOSConfig.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/srtm_config.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/rpmsg_config.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/wireless_uart_bridge.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/ApplMain.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/ApplMain.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/ble_conn_manager.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/ble_conn_manager.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/ble_service_discovery.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/ble_service_discovery.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/ble_init.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/Common/EmbeddedTypes.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Interface/FsciInterface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Source/FsciCommands.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Source/FsciCommands.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Source/FsciCommunication.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Source/FsciCommunication.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Source/FsciLogging.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Source/FsciMain.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/FunctionLib/FunctionLib.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/FunctionLib/FunctionLib.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/Lists/GenericList.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/Lists/GenericList.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/MemManager/Interface/MemManager.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/MemManager/Source/MemManager.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/Messaging/Source/Messaging.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/Messaging/Interface/Messaging.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/OSAbstraction/Source/fsl_os_abstraction_free_rtos.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/OSAbstraction/Interface/fsl_os_abstraction.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/OSAbstraction/Interface/fsl_os_abstraction_config.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/OSAbstraction/Interface/fsl_os_abstraction_free_rtos.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/Panic/Source/Panic.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/Panic/Interface/Panic.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/SerialManager/Interface/SerialManager.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/SerialManager/Source/SerialManager.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/SerialManager/Source/UART_Adapter.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/SerialManager/Source/UART_Adapter.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/gatt_db/gatt_database.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/gatt_db/gatt_database_dynamic.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/gatt_db/gatt_database_dynamic.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_types.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_types.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gap.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gap.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gap_types.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gap_types.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt_types.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt_types.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt_db_app.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt_db_app.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt_db_app_types.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt_db_app_types.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_l2cap.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_l2cap.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_l2cap_types.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_l2cap_types.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/host_ble.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/host_ble.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/host_gap.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/host_gatt.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/host_gatt_db.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/host_l2cap.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/config/ble_config.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/config/ble_globals.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/att_callbacks.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/att_errors.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/att_interface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/att_params.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/att_types.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/ble_constants.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/ble_general.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/ble_host_tasks.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/ble_sig_defines.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/ble_utils.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/fsci_ble_interface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gap_interface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gap_types.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_client_interface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_database.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_db_app_interface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_db_att_interface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_db_dynamic.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_interface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_server_interface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_types.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/hci_interface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/hci_types.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/l2ca_cb_interface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/l2ca_interface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/l2ca_types.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/vendor_debug_commands.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/profiles/device_info/device_info_interface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/profiles/device_info/device_info_service.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/profiles/wireless_uart/wireless_uart_interface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/profiles/wireless_uart/wireless_uart_service.c
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/profiles/battery/battery_interface.h
            ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/profiles/battery/battery_service.c
            ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/${board}/app_srtm.c
            ${board_root}/${board}/app_srtm.h
            middleware/issdk/algorithms/pedometer/include/libinclude/KeynetikPedometer.h
            middleware/issdk/algorithms/pedometer/source/pedometer.c
            middleware/issdk/algorithms/pedometer/include/pedometer.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/issdk/algorithms/pedometer/include/libinclude
             middleware/issdk/algorithms/pedometer/source
             middleware/issdk/algorithms/pedometer/include
)

mcux_add_library(
        BASE_PATH ${SdkRootDirPath}
        LIBS middleware/issdk/algorithms/pedometer/lib/libpedometerm4.a
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/Common
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Interface
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Source
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/FunctionLib
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/Lists
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/MemManager/Interface
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/Messaging/Interface
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/OSAbstraction/Interface
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/Panic/Interface
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/SerialManager/Interface
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/framework/SerialManager/Source
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/gatt_db
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/config
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/profiles/device_info
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/profiles/wireless_uart
             ${board_root}/${board}/demo_apps/wireless_uart_bridge/ble/profiles/battery
             middleware/multicore/remoteproc
)

mcux_add_macro(
    CC "-DENABLE_RAM_VECTOR_TABLE"
    AS "-DNO_CRP"
)
