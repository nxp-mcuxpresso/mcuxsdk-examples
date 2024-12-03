include(${SdkRootDirPath}/CMakeLists.txt)

mcux_set_variable(NXP_ZB_BASE ${SdkRootDirPath}/middleware/wireless/zigbee)
mcux_set_variable(PDUMCONFIG  ${NXP_ZB_BASE}/tools/PDUMConfig/Source/PDUMConfig.py)
mcux_set_variable(ZPSCONFIG   ${NXP_ZB_BASE}/tools/ZPSConfig/Source/ZPSConfig.py)
mcux_set_variable(ZPSCFG      ${NXP_ZB_BASE}/examples/zigbee_coordinator/src/coordinator.zpscfg)

mcux_set_variable(CONFIG_ZB_COORD_SINGLE_CHANNEL "12")
mcux_set_variable(CONFIG_ZB_COORD_TRACE_APP 1)
mcux_set_variable(CONFIG_ZB_COORD_TRACE_ZCL 1)


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

mcux_add_source(
    BASE_PATH ${NXP_ZB_BASE}/examples/zigbee_coordinator/src/
    SOURCES
            app_coordinator.c
            app_coordinator.h
            app_main.h
            app_serial_commands.c
            app_serial_commands.h
            app_start.c
            app_zcl_task.c
            app_zcl_task.h
            bdb_options.h
            zcl_options.h
            zigbee_config.h
            coordinator.zpscfg
            readme.txt
)

mcux_add_source(
    # BASE_PATH ${NXP_ZB_BASE}/examples/zigbee_coordinator_freertos/src/
    BASE_PATH ${NXP_ZB_BASE}/examples/zigbee_coordinator/src/
    SOURCES
            # app_main_freertos.c
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
    SINGLE_CHANNEL=${CONFIG_ZB_COORD_SINGLE_CHANNEL}
    TRACE_APP=${CONFIG_ZB_COORD_TRACE_APP}
    TRACE_ZCL=${CONFIG_ZB_COORD_TRACE_ZCL}
    ZPS_GENERATED_FILE=\\\"${APPLICATION_BINARY_DIR}/zps_gen.h\\\"
    PDUM_GENERATED_FILE=\\\"${APPLICATION_BINARY_DIR}/pdum_gen.h\\\"
    # ZIGBEE_EVENT_IMPL
)


mcux_add_include(
    BASE_PATH ${NXP_ZB_BASE}
    INCLUDES
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


mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
