mcux_set_variable(CONFIG_ZB_PLATFORM "K32W1")

mcux_set_variable(OTA_VERSION_STRING "K148EVK-----K32W148--ENCRYPTED00")

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wireless/framework/boards/kw45_k32w1/K32W1480
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/devices/${soc_portfolio}/${soc_series}/${device}
    SOURCES gcc/startup_${device}.S
)

mcux_add_source(BASE_PATH ${SdkRootDirPath}/devices/${soc_portfolio}/${soc_series}/${device} SOURCES ${device}.h)
mcux_add_source(BASE_PATH ${SdkRootDirPath}/devices/${soc_portfolio}/${soc_series}/${device} SOURCES system_${device}.h)
mcux_add_source(BASE_PATH ${SdkRootDirPath}/devices/${soc_portfolio}/${soc_series}/${device} SOURCES system_${device}.c)

mcux_add_source(BASE_PATH ${SdkRootDirPath}/middleware/wireless/zigbee/platform/${CONFIG_ZB_PLATFORM}
                SOURCES FreeRTOSConfig.h)

#zigbee applications defines
mcux_add_configuration(
    CC "-DGENERIC_LIST_LIGHT=0\
        -DUSE_SMU2_AS_SYSTEM_MEMORY=1\
        -DTM_ENABLE_TIME_STAMP=1\
        -DUSE_NBU=1\
        -DgMemManagerLightGuardsCheckEnable=1\
        -DgMemManagerLightExtendHeapAreaUsage=1\
        -DgNvStorageIncluded_d=1\
        -DgAppButtonCnt_c=1\
        -DUSE_PB_RAM_AS_SYSTEM_MEMORY=1\
        -DgHwParamsProdDataPlacement_c=gHwParamsProdDataPlacementLegacyMode_c\
        -DgDebugConsoleEnable_d=1\
        -DDebugConsole_c=1\
        -DFSL_OSA_TASK_ENABLE=1\
        -DcPWR_FullPowerDownMode=1\
        -DHEAP_SIZE=4000\
        -DLITTLE_ENDIAN_PROCESSOR\
        -Dvector_table_size=512\
        -D__ram_vector_table__=1\
        -DosNumberOfSemaphores=0\
        -DosNumberOfMutexes=0\
        -DosNumberOfMessageQs=0\
        -DosNumberOfMessages=0\
        -DosNumberOfEvents=0\
        -DgMainThreadPriority_c=7\
        -DgTaskMultipleInstancesManagement_c=0\
        -DFSL_OSA_BM_TIMER_CONFIG=FSL_OSA_BM_TIMER_SYSTICK\
        -DENABLE_RAM_VECTOR_TABLE\
        -DgFlashEraseDuringWrite=0\
        -DgInvalidateHeaderLength=336\
        -DgEepromType_d=gEepromDevice_InternalFlash_c\
        -DgOtaVerifyWrite_d=0\
        -DgEePromParams_BufferSize_c=512\
        -DgEepromParams_bufferedWrite_c=1\
        -DgEepromParams_CurrentOffset=0\
        -DOM15081\
        -DZIGBEE_USE_FRAMEWORK=1\
        -DgSupportBle=0\
        -DJN518x=5180\
        -DPDM_NO_RTOS\
        -D__NEWLIB__\
        -DMCUXPRESSO_SDK\
        -DTIMER_PORT_TYPE_LPTMR=1\
        -DSERIAL_PORT_TYPE_RPMSG=1\
        -DHAL_UART_ADAPTER_LOWPOWER=1\
        -DMULTICORE_APP=1\
        -DDEBUG\
        -DCR_INTEGER_PRINTF\
        -D__MCUXPRESSO\
        -D__USE_CMSIS\
        -DFSL_RTOS_FREE_RTOS\
        -DSDK_OS_FREE_RTOS\
        -DBOARD_DEBUG_UART_INSTANCE=1\
        -DPRINTF_ADVANCED_ENABLE=1\
        "
)

#Router specific defines
mcux_add_configuration(
    CC "-DZBPRO_DEVICE_TYPE=ZCR\
        -DCLD_OTA_KEY=\\\"${ENCRYPTION_KEY}\\\"\
        -DCLD_OTA_MANF_ID_VALUE=${MANUFACTURER_CODE}\
        -DOTA_INTERNAL_STORAGE\
        -DINTERNAL_ENCRYPTED\
        -DOTA_ENCRYPTED\
        -DBUILD_OTA\
        "
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --sort-section=alignment\
        -Xlinker --defsym=gUseNVMLink_d=1\
        -Xlinker --defsym=gUseInternalStorageLink_d=1\
        -Xlinker --defsym=__stack_size__=0x2000\
        -Xlinker --defsym=__heap_size__=0x8000\
        -Xlinker --defsym=gUseNVMLink_d=1\
        -Xlinker --defsym=m_lowpower_flag_start=0x489C007C"
)

add_custom_command(TARGET ${MCUX_SDK_PROJECT_NAME}
        POST_BUILD
        COMMAND ${PYTHON_EXECUTABLE} ${OTA_TOOL} -log INFO --embed -i ${MCUX_SDK_PROJECT_NAME}.elf -o ${MCUX_SDK_PROJECT_NAME}_v1.axf -OtaMan \"${MANUFACTURER_CODE}\" -OtaImgTyp \"${OTA_IMAGE_TYPE}\" -OtaVersionStr \"${OTA_VERSION_STRING}\" -OtaFileVersion 1 -c $ENV{ARMGCC_DIR}
        COMMAND ${PYTHON_EXECUTABLE} ${OTA_TOOL} -log INFO -i ${MCUX_SDK_PROJECT_NAME}.elf -o ${MCUX_SDK_PROJECT_NAME}_v2.ota -e -k ${ENCRYPTION_KEY} -OtaMan \"${MANUFACTURER_CODE}\" -OtaImgTyp \"${OTA_IMAGE_TYPE}\" -OtaVersionStr \"${OTA_VERSION_STRING}\" -OtaFileVersion 2 -c $ENV{ARMGCC_DIR}
        COMMAND ${PYTHON_EXECUTABLE} ${OTA_TOOL} -log INFO -i ${MCUX_SDK_PROJECT_NAME}.elf -o ${MCUX_SDK_PROJECT_NAME}_v3.ota -e -k ${ENCRYPTION_KEY} -OtaMan \"${MANUFACTURER_CODE}\" -OtaImgTyp \"${OTA_IMAGE_TYPE}\" -OtaVersionStr \"${OTA_VERSION_STRING}\" -OtaFileVersion 3 -c $ENV{ARMGCC_DIR}
)
