
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/wireless_uart_bridge/app_config.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/app_config.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
            examples/_boards/${board}/srtm_config.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/rpmsg_config.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/wireless_uart_bridge.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/ApplMain.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/ApplMain.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/ble_conn_manager.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/ble_conn_manager.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/ble_service_discovery.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/ble_service_discovery.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/ble_init.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/Common/EmbeddedTypes.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Interface/FsciInterface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Source/FsciCommands.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Source/FsciCommands.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Source/FsciCommunication.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Source/FsciCommunication.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Source/FsciLogging.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Source/FsciMain.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/FunctionLib/FunctionLib.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/FunctionLib/FunctionLib.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/Lists/GenericList.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/Lists/GenericList.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/MemManager/Interface/MemManager.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/MemManager/Source/MemManager.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/Messaging/Source/Messaging.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/Messaging/Interface/Messaging.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/OSAbstraction/Source/fsl_os_abstraction_free_rtos.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/OSAbstraction/Interface/fsl_os_abstraction.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/OSAbstraction/Interface/fsl_os_abstraction_config.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/OSAbstraction/Interface/fsl_os_abstraction_free_rtos.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/Panic/Source/Panic.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/Panic/Interface/Panic.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/SerialManager/Interface/SerialManager.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/SerialManager/Source/SerialManager.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/SerialManager/Source/UART_Adapter.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/SerialManager/Source/UART_Adapter.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/gatt_db/gatt_database.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/gatt_db/gatt_database_dynamic.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/gatt_db/gatt_database_dynamic.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_types.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_types.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gap.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gap.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gap_types.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gap_types.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt_types.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt_types.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt_db_app.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt_db_app.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt_db_app_types.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_gatt_db_app_types.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_l2cap.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_l2cap.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_l2cap_types.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci/fsci_ble_l2cap_types.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/host_ble.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/host_ble.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/host_gap.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/host_gatt.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/host_gatt_db.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/host_l2cap.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/config/ble_config.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/config/ble_globals.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/att_callbacks.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/att_errors.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/att_interface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/att_params.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/att_types.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/ble_constants.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/ble_general.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/ble_host_tasks.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/ble_sig_defines.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/ble_utils.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/fsci_ble_interface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gap_interface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gap_types.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_client_interface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_database.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_db_app_interface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_db_att_interface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_db_dynamic.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_interface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_server_interface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/gatt_types.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/hci_interface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/hci_types.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/l2ca_cb_interface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/l2ca_interface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/l2ca_types.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface/vendor_debug_commands.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/profiles/device_info/device_info_interface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/profiles/device_info/device_info_service.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/profiles/wireless_uart/wireless_uart_interface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/profiles/wireless_uart/wireless_uart_service.c
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/profiles/battery/battery_interface.h
            examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/profiles/battery/battery_service.c
            examples/_boards/${board}/rsc_table.c
            examples/_boards/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            examples/_boards/${board}/app_srtm.c
            examples/_boards/${board}/app_srtm.h
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
    INCLUDES examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/Common
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Interface
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/FSCI/Source
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/FunctionLib
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/Lists
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/MemManager/Interface
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/Messaging/Interface
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/OSAbstraction/Interface
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/Panic/Interface
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/SerialManager/Interface
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/framework/SerialManager/Source
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/gatt_db
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/fsci
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/config
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/fsci_host/host/interface
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/profiles/device_info
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/profiles/wireless_uart
             examples/_boards/${board}/demo_apps/wireless_uart_bridge/ble/profiles/battery
             middleware/multicore/remoteproc
)

mcux_add_macro(
    CC "-DENABLE_RAM_VECTOR_TABLE"
    AS "-DNO_CRP"
)
