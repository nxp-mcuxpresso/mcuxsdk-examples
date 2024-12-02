include(${SdkRootDirPath}/CMakeLists.txt)

if (CONFIG_ZB_ED_RXON_R23_REVISION)
    mcux_set_variable(ZPSAPL_LIB ${NXP_ZB_BASE}/platform/${CONFIG_ZB_PLATFORM}/libs/libZPSAPL_R23.a)
    mcux_set_variable(ZPSNWK_LIB ${NXP_ZB_BASE}/platform/${CONFIG_ZB_PLATFORM}/libs/libZPSNWK_ZED_R23.a)
endif()

if (CONFIG_ZB_ED_RXON_R22_REVISION)
    mcux_set_variable(ZPSAPL_LIB ${NXP_ZB_BASE}/platform/${CONFIG_ZB_PLATFORM}/libs/libZPSAPL.a)
    mcux_set_variable(ZPSNWK_LIB ${NXP_ZB_BASE}/platform/${CONFIG_ZB_PLATFORM}/libs/libZPSNWK_ZED.a)
endif()

#temporary for workarounds
mcux_set_variable(NXP_WIRELESS_RELATIVE_PATH ../../../../../../middleware/wireless)
# The files from this dir are pulled in only for the purpose of populating the MCUX project
# They are overwritten by the pre-build steps
mcux_set_variable(DUMMY_GEN_FILES_PATH ${SdkRootDirPath}/middleware/wireless/zigbee/examples/zigbee_ed_rx_on/src)

mcux_add_source(
    BASE_PATH ${NXP_ZB_BASE}/examples/zigbee_ed_rx_on/src/
    SOURCES
            app_end_device_node.c
            app_end_device_node.h
            app_main.h
            app_reporting.c
            app_reporting.h
            app_start.c
            app_zcl_task.c
            app_zcl_task.h
            bdb_options.h
            zcl_options.h
            zigbee_config.h
            enddevice_rxon.zpscfg
            readme.txt
)

mcux_add_source(
    BASE_PATH ${NXP_ZB_BASE}/examples/zigbee_ed_rx_on/src/
    SOURCES
            app_main.c
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
    EndDevice
    SINGLE_CHANNEL=${CONFIG_ZB_ED_RXON_SINGLE_CHANNEL}
    TRACE_APP=${CONFIG_ZB_ED_RXON_TRACE_APP}
    TRACE_ZCL=${CONFIG_ZB_ED_RXON_TRACE_ZCL}
    ZPS_GENERATED_FILE=\\\"${APPLICATION_BINARY_DIR}/zps_gen.h\\\"
    PDUM_GENERATED_FILE=\\\"${APPLICATION_BINARY_DIR}/pdum_gen.h\\\"
    # ZIGBEE_EVENT_IMPL
)

mcux_add_include(
    BASE_PATH ${NXP_ZB_BASE}
    INCLUDES
            examples/zigbee_ed_rx_on/src/
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
        -z enddevice_rxon
        -e ENDIAN
        -f ${ZPSCFG}
        -o ${APPLICATION_BINARY_DIR}
)

add_custom_command(
        PRE_BUILD
        OUTPUT
        ${APPLICATION_BINARY_DIR}/zps_gen.c
        ${APPLICATION_BINARY_DIR}/zps_gen.h
        COMMAND ${PYTHON_EXECUTABLE} ${ZPSCONFIG} ARGS -n enddevice_rxon
                             -e LITTLE_ENDIAN
                             -t JN518x
                             -l ${ZPSNWK_LIB}
                             -a ${ZPSAPL_LIB}
                             -f ${ZPSCFG}
                             -o ${APPLICATION_BINARY_DIR}
                             -c $ENV{ARMGCC_DIR}
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
