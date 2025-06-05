cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/bluetooth/CMakeLists.txt)


if (CONFIG_ZB_COORD_R23_REVISION)
    mcux_set_variable(ZPSAPL_LIB ${NXP_ZB_BASE}/platform/${CONFIG_ZB_PLATFORM}/libs/libZPSAPL_R23.a)
    mcux_set_variable(ZPSNWK_LIB ${NXP_ZB_BASE}/platform/${CONFIG_ZB_PLATFORM}/libs/libZPSNWK_R23.a)
    message("R23 selected")
endif()

if (CONFIG_ZB_COORD_R22_REVISION)
    mcux_set_variable(ZPSAPL_LIB ${NXP_ZB_BASE}/platform/${CONFIG_ZB_PLATFORM}/libs/libZPSAPL.a)
    mcux_set_variable(ZPSNWK_LIB ${NXP_ZB_BASE}/platform/${CONFIG_ZB_PLATFORM}/libs/libZPSNWK.a)
    message("R22 selected")
endif()


# The files from this dir are pulled in only for the purpose of populating the MCUX project
# They are overwritten by the pre-build steps
mcux_set_variable(DUMMY_GEN_FILES_PATH ${SdkRootDirPath}/middleware/wireless/zigbee/examples/zigbee_coordinator/src)

# Zigbee sources
mcux_add_source(
    BASE_PATH ${NXP_ZB_BASE}/examples/zigbee_coordinator/src/
    SOURCES
            app_coordinator.c
            app_coordinator.h
            app_serial_commands.c
            app_serial_commands.h
            app_zcl_task.c
            app_zcl_task.h
            bdb_options.h
            zcl_options.h
            zigbee_config.h
            coordinator.zpscfg
)

mcux_add_source(
    BASE_PATH ${NXP_ZB_BASE}/examples/zb_common
    SOURCES
            app_shell_utils.c
            app_shell_utils.h
)

mcux_add_source(
    BASE_PATH ${NXP_ZB_BASE}/examples/zigbee_ble/bluetooth
    SOURCES
            wireless_uart.c
            wireless_uart.h
            app_config.c
            gatt_uuid128.h
)

mcux_add_source(
    BASE_PATH ${NXP_ZB_BASE}/examples/zigbee_ble/zigbee_coordinator_ble_wuart
    SOURCES
            app_zigbee.c
            app_zigbee.h
            app_shell.c
            app_shell.h
            gatt_db.h
)

mcux_add_source(
    BASE_PATH ${NXP_ZB_BASE}/examples/zigbee_ble/zigbee_coordinator_ble_wuart
    SOURCES
            main_freertos.c
)

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${NXP_ZB_BASE}
    SOURCES examples/zigbee_ble/bluetooth/app_preinclude.h
)

# Bluetooth sources
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    SOURCES
            bluetooth/application/common/app_conn.c
            bluetooth/application/common/app_conn.h
            bluetooth/application/common/ble_init.c
            bluetooth/application/common/ble_init.h
            bluetooth/application/common/app_advertiser.c
            bluetooth/application/common/app_advertiser.h
            bluetooth/application/common/app_connection.c
            bluetooth/application/common/app_scanner.c
            bluetooth/application/common/app_scanner.h
            bluetooth/application/common/app_nvm.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/application/common/
)


# Dummy files, to be pulled for MCUX package
mcux_add_source(
    BASE_PATH ${DUMMY_GEN_FILES_PATH}
    SOURCES
            pdum_apdu.S
            pdum_gen.c
            pdum_gen.h
            zps_gen.c
            zps_gen.h
)

#Files generated per build
mcux_add_source(
    BASE_PATH ${APPLICATION_BINARY_DIR}
    SOURCES
            pdum_apdu.S
            pdum_gen.c
            pdum_gen.h
            zps_gen.c
            zps_gen.h
)

mcux_add_macro(
    SINGLE_CHANNEL=CONFIG_ZB_COORD_SINGLE_CHANNEL
    TRACE_APP=CONFIG_ZB_COORD_TRACE_APP
    TRACE_ZCL=CONFIG_ZB_COORD_TRACE_ZCL
    ZPS_GENERATED_FILE=\\\"${APPLICATION_BINARY_DIR}/zps_gen.h\\\"
    PDUM_GENERATED_FILE=\\\"${APPLICATION_BINARY_DIR}/pdum_gen.h\\\"
    gWuart_CentralRole_c=0
    gWuart_PeripheralRole_c=1
    ZIGBEE_EVENT_IMPL
)

mcux_add_include(
    BASE_PATH ${NXP_ZB_BASE}
    INCLUDES
            examples/zigbee_ble/bluetooth/
            examples/zigbee_ble/zigbee_coordinator_ble_wuart/
            examples/zigbee_coordinator/src/
            platform/${CONFIG_ZB_PLATFORM}/
)

add_custom_command(
        PRE_BUILD
        OUTPUT
        ${APPLICATION_BINARY_DIR}/pdum_gen.c
        ${APPLICATION_BINARY_DIR}/pdum_gen.h
        ${APPLICATION_BINARY_DIR}/pdum_apdu.S
        COMMAND ${PYTHON_EXECUTABLE} ${PDUMCONFIG}
        ARGS 
        -z coordinator
        -e ENDIAN
        -f ${ZPSCFG}
        -o ${APPLICATION_BINARY_DIR}
)

add_custom_command(
        PRE_BUILD
        OUTPUT
        ${APPLICATION_BINARY_DIR}/zps_gen.c
        ${APPLICATION_BINARY_DIR}/zps_gen.h
        COMMAND ${PYTHON_EXECUTABLE} ${ZPSCONFIG} ARGS -n coordinator
                             -e LITTLE_ENDIAN
                             -t JN518x
                             -l ${ZPSNWK_LIB}
                             -a ${ZPSAPL_LIB}
                             -f ${ZPSCFG}
                             -o ${APPLICATION_BINARY_DIR}
                             -c $ENV{ARMGCC_DIR}
)

add_custom_command(
        TARGET ${MCUX_SDK_PROJECT_NAME}
        POST_BUILD
        COMMAND ${PYTHON_EXECUTABLE} ${MEMSIZE} ARGS
        ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.elf
        ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.map
        ${NXP_ZB_BASE}/examples/zigbee_coordinator/src/coordinator.json 2>&1 > NUL
)


mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
mcux_add_armgcc_configuration(
    LD "-Xlinker -Map=${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.map -Wl,--cref -Wl,-fno-lto"
)