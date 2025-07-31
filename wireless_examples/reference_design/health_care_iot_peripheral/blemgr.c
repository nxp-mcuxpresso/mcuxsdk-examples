/*
 * Copyright 2022-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include <stdio.h>
#include "hciot_common.h"
#include "blemgr.h"
#include "ble_host_task_config.h"
#include "board.h"
#include "fsl_component_mem_manager.h"
#include "fsl_component_panic.h"
#include "fsl_component_timer_manager.h"
#include "RNG_Interface.h"
#include "SecLib.h"
#include "gap_types.h"
#include "gap_interface.h"
#include "gatt_server_interface.h"
#include "ble_conn_manager.h"
#include "gatt_db_handles.h"
#include "gap_types.h"
#include "battery_interface.h"
#include "current_time_interface.h"
#include "health_thermometer_interface.h"
#include "device_info_interface.h"
#include "msgq.h"
#include "ble_controller.h"
#include "ll_intf.h"
#include "nxp_hoststack_adapter.h"
#if (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U))
#include "gatt_client_interface.h"
#include "eventstore.h"
#endif /* (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U)) */
#if gAppUseNvm_d
#include "NVM_Interface.h"
#endif /* gAppUseNvm_d */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define mBatteryLevelReportInterval_c (10U) /* battery level report interval in seconds  */
#define mIntTempUpdateRate_c          1000
#ifndef NO_FAST_CONNECTION
#define gFastConnMinAdvInterval_c 32 /* 20 ms */
#define gFastConnMaxAdvInterval_c 48 /* 30 ms */
#else
#define gFastConnMinAdvInterval_c 1600     /* 1 s */
#define gFastConnMaxAdvInterval_c 2000     /* 1.250 s */
#endif                                     /* NO_FAST_CONNECTION */
#define gReducedPowerMinAdvInterval_c 2400 /* 1.5 s */
#define gReducedPowerMaxAdvInterval_c 4000 /* 2.5 s */

#define gFastConnAdvTime_c     30  /* s */
#define gReducedPowerAdvTime_c 300 /* s */

#if gAppUseBonding_d
#define gFastConnWhiteListAdvTime_c 10 /* s */
#else
#define gFastConnWhiteListAdvTime_c 0
#endif /* gAppUseBonding_d */

#if (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U))
#define mCharReadBufferLength_c (13U) /* length of the buffer */
#endif                                /* (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U)) */

/* Host to Application GATT Server Message */
typedef struct gattServerMsg_tag
{
    deviceId_t deviceId;
    gattServerEvent_t serverEvent;
} gattServerMsg_t;

#if (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U))
typedef struct gattClientProcMsg_tag
{
    deviceId_t deviceId;
    gattProcedureType_t procedureType;
    gattProcedureResult_t procedureResult;
    bleResult_t error;
} gattClientProcMsg_t;
#endif /* defined(gAppUseTimeService_d */

typedef enum
{
#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
    fastWhiteListAdvState_c,
#endif /* gAppUseBonding_d */
    fastAdvState_c,
    slowAdvState_c
} advType_t;

typedef struct advState_tag
{
    bool_t advOn;
    advType_t advType;
} advState_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void HostStackInitialized(void);
#if (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U))

static void GattClientProcedureCallback(deviceId_t deviceId,
                                        gattProcedureType_t procedureType,
                                        gattProcedureResult_t procedureResult,
                                        bleResult_t error);
static void GattClientProcedureHandler(gattClientProcMsg_t *gattClientProcMsg);
#endif /* defined(gAppUseTimeService_d */
static void GapGenericCallback(gapGenericEvent_t *pGenericEvent);
static void GapGenericHandler(gapGenericEvent_t *pGenericEvent);
static void GapAdvCallback(gapAdvertisingEvent_t *pAdvertisingEvent);
static void GapAdvHandler(gapAdvertisingEvent_t *pAdvertisingEvent);
static void GapConnCallback(deviceId_t peerDeviceId, gapConnectionEvent_t *pConnectionEvent);
static void GapConnHandler(connectionMsg_t *connMsg);
static void GattServerCallback(deviceId_t peerDeviceId, gattServerEvent_t *pServerEvent);
static void GattServerHandler(gattServerMsg_t *gattServerMsg);
static void AdvertisingTimerCallback(void *pParam);
static void ResetAdvertise(void);
static bleResult_t StartAdvertise(void);
static void BatteryMeasurementTimerCallback(void *pParam);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static msgq_handle_t s_messageQueue;

static disConfig_t disServiceConfig                    = {(uint16_t)service_device_info};
static bool_t basValidClientList[gAppMaxConnections_c] = {FALSE};
static basConfig_t basServiceConfig = {(uint16_t)service_battery, 0, basValidClientList, gAppMaxConnections_c};
static htsUserData_t mUserData;
static htsConfig_t htsServiceConfig = {(uint16_t)service_health_therm, mIntTempUpdateRate_c, gHts_UnitInCelsius_c,
                                       &mUserData};
static uint16_t cpHandles[]         = {(uint16_t)value_measure_int};

extern gapAdvertisingData_t gAppAdvertisingData;
extern gapScanResponseData_t gAppScanRspData;
extern gapAdvertisingParameters_t gAdvParams;

static advState_t mAdvState;
static deviceId_t mPeerDeviceId = gInvalidDeviceId_c;
static TIMER_MANAGER_HANDLE_DEFINE(mAdvTimerHandle);
static TIMER_MANAGER_HANDLE_DEFINE(mBatteryMeasurementTimerHandle);

#if (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U))
static uint8_t mOutCharReadBuffer[mCharReadBufferLength_c];
static uint16_t mOutCharReadByteCount;
static bool_t isTimeSynchronized = FALSE;
#endif /* gAppUseTimeService_d */
static blemgr_event_cb_t s_eventCallback;

/*******************************************************************************
 * Code
 ******************************************************************************/

static bleResult_t BleInitialize()
{
    /* BLE Controller Init */
    if (kBLEC_Success != BLEController_Init(NXPHoststackAdapter_hciControllerToHost, 0, NULL))
    {
        return gBleOsError_c;
    }

    if (kBLEC_Success != BLEController_SetConnectionInitialTxPowerDbm(0))
    {
        return gBleOsError_c;
    }

    if (kBLEC_Success != BLEController_SetTxPowerDbm(0))
    {
        return gBleOsError_c;
    }

    /* Configure PHY calibration interval to 60s */
    ll_intf_le_set_phy_clbr_params(60, 0);

    /* Check for available memory storage */
    if (!Ble_CheckMemoryStorage())
    {
        return gBleOutOfMemory_c;
    }

    /* BLE Host Tasks Init */
    if (KOSA_StatusSuccess != Ble_HostTaskInit())
    {
        return gBleOsError_c;
    }

    /* BLE Host Stack Init */
    return Ble_HostInitialize(GapGenericCallback, NXPHoststackAdapter_hciHostToController);
}

static void HostStackInitialized(void)
{
    uint8_t bdAddress[8];
    status_t bleResult;
    (void)bleResult;
    /* Common GAP configuration */
    BleConnManager_GapCommonConfig();

    /* Register for callbacks*/
    bleResult = GattServer_RegisterCallback(GattServerCallback);
    assert_equal(bleResult, gBleSuccess_c);
    bleResult = GattServer_RegisterHandlesForWriteNotifications(NumberOfElements(cpHandles), cpHandles);
    assert_equal(bleResult, gBleSuccess_c);

#if (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U))
    GattClient_RegisterProcedureCallback(GattClientProcedureCallback);
#endif /* gAppUseTimeService_d */

    mAdvState.advOn = FALSE;

    BLEMGR_HostStackReady();

    /* Start services */
    bleResult = Hts_Start(&htsServiceConfig);
    assert_equal(bleResult, gBleSuccess_c);

    basServiceConfig.batteryLevel = 100;
    bleResult                     = Bas_Start(&basServiceConfig);
    assert_equal(bleResult, gBleSuccess_c);

    memset(bdAddress, 0, sizeof(bdAddress));
    if (BLEController_ReadBdAddr(bdAddress) == kBLEC_Success)
    {
        disServiceConfig.pSystemId = (systemId_t *)bdAddress;
        bleResult                  = Dis_Start(&disServiceConfig);
        assert_equal(bleResult, gBleSuccess_c);
    }

    timer_status_t tmStatus = TM_Open(mBatteryMeasurementTimerHandle);
    (void)tmStatus;
    assert_equal(tmStatus, kStatus_TimerSuccess);
    tmStatus =
        TM_InstallCallback((timer_handle_t)mBatteryMeasurementTimerHandle, BatteryMeasurementTimerCallback, NULL);
    assert_equal(tmStatus, kStatus_TimerSuccess);
#ifndef BLE_POWER_PROFILING
    tmStatus = TM_Start((timer_handle_t)mBatteryMeasurementTimerHandle,
                        kTimerModeLowPowerTimer | kTimerModeSetSecondTimer, mBatteryLevelReportInterval_c);
    assert_equal(tmStatus, kStatus_TimerSuccess);
#endif /* BLE_POWER_PROFILING */

    tmStatus = TM_Open(mAdvTimerHandle);
    assert_equal(tmStatus, kStatus_TimerSuccess);

    ResetAdvertise();
}

#if (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U))
static void GattClientProcedureCallback(deviceId_t deviceId,
                                        gattProcedureType_t procedureType,
                                        gattProcedureResult_t procedureResult,
                                        bleResult_t error)
{
    gattClientProcMsg_t *gattClientProcMsg =
        MSGQ_CreateMsg((msgq_handler_t)&GattClientProcedureHandler, sizeof(gattClientProcMsg_t));

    assert(gattClientProcMsg);

    gattClientProcMsg->deviceId        = deviceId;
    gattClientProcMsg->procedureType   = procedureType;
    gattClientProcMsg->error           = error;
    gattClientProcMsg->procedureResult = procedureResult;

    MSGQ_Put(s_messageQueue, gattClientProcMsg);
}

static void GattClientProcedureHandler(gattClientProcMsg_t *gattClientProcMsg)
{
    /* deviceId_t serverDeviceId = gattClientProcMsg->deviceId; */
    gattProcedureType_t procedureType     = gattClientProcMsg->procedureType;
    gattProcedureResult_t procedureResult = gattClientProcMsg->procedureResult;
    bleResult_t error                     = gattClientProcMsg->error;

    if (procedureResult == gGattProcError_c)
    {
        attErrorCode_t attError = (attErrorCode_t)(error & 0xFF);
        if (attError == gAttErrCodeInsufficientEncryption_c || attError == gAttErrCodeInsufficientAuthorization_c ||
            attError == gAttErrCodeInsufficientAuthentication_c)
        {
            /* handle auth. errors */
        }

        else
        {
            /* characteristic not found */
        }
    }
    else if (procedureResult == gGattProcSuccess_c)
    {
        switch (procedureType)
        {
            case gGattProcReadUsingCharacteristicUuid_c:
            {
                if (mOutCharReadByteCount > 2)
                {
                    ctsDayDateTime_t time;
                    /* uint16_t charValueLength = mOutCharReadByteCount - 3; */
                    /* uint16_t handle = Utils_ExtractTwoByteValue(mOutCharReadBuffer+1); */
                    uint8_t *pValue = &mOutCharReadBuffer[3];

                    time.dateTime.year    = Utils_ExtractTwoByteValue(&pValue[0]);
                    time.dateTime.month   = pValue[2];
                    time.dateTime.day     = pValue[3];
                    time.dateTime.hours   = pValue[4];
                    time.dateTime.minutes = pValue[5];
                    time.dateTime.seconds = pValue[6];
                    time.dayOfWeek        = pValue[7];

                    uint32_t localTime = Cts_DayDateTimeToEpochTime(time);
                    /* Is this the first time we record the user current time? */
                    if (0 == EVENTSTORE_GetCurrentTimeOffset())
                    {
                        EVENTSTORE_RecordCurrentTime(localTime);
                    }
                    isTimeSynchronized = TRUE;
                }
            }
            break;

            default:; /* For MISRA compliance */
                break;
        }
    }
}
#endif /* gAppUseTimeService_d */

static void GapGenericCallback(gapGenericEvent_t *pGenericEvent)
{
    switch (pGenericEvent->eventType)
    {
        case gInternalError_c:
            PRINTF("Internal error: %d, source: %d, hci opcode: 0x%x\n",
                   pGenericEvent->eventData.internalError.errorCode, pGenericEvent->eventData.internalError.errorSource,
                   pGenericEvent->eventData.internalError.hciCommandOpcode);
            /* The host stack reports error gBleInvalidParameter_c each time a malformed HCI event is received.
             * Invalid parameters may be the result of malformed air packets (e.g.  Sweyntooth).
             * Handle these errors gracefully.
             * Unexpected connection handle error is expected under certain race conditions. */
            assert((pGenericEvent->eventData.internalError.errorCode == gBleInvalidParameter_c) ||
                   (pGenericEvent->eventData.internalError.errorCode == gHciUnknownConnectionIdentifier_c));
            break;
        default:
            break;
    }
    void *message = MSGQ_CreateMsg((msgq_handler_t)&GapGenericHandler, sizeof(gapGenericEvent_t));

    assert(message);
    FLib_MemCpy(message, pGenericEvent, sizeof(gapGenericEvent_t));
    MSGQ_Put(s_messageQueue, message);
}

static void GapGenericHandler(gapGenericEvent_t *pGenericEvent)
{
    status_t bleResult;
    (void)bleResult;
    /* Inform BLE Conn Manager */
    BleConnManager_GenericEvent(pGenericEvent);

    switch (pGenericEvent->eventType)
    {
        case gInitializationComplete_c:
            HostStackInitialized();
            break;
        case gAdvertisingParametersSetupComplete_c:
            bleResult = Gap_SetAdvertisingData(&gAppAdvertisingData, &gAppScanRspData);
            assert_equal(bleResult, gBleSuccess_c);
            break;
        case gAdvertisingDataSetupComplete_c:
            bleResult = HCIOT_GapStartAdvertising(GapAdvCallback, GapConnCallback);
            assert_equal(bleResult, gBleSuccess_c);
            break;
        case gAdvertisingSetupFailed_c:
            panic(0, 0, 0, 0);
            break;
        default:
        {
            ; /* No action required */
        }
        break;
    }
}

static void GapAdvCallback(gapAdvertisingEvent_t *pAdvertisingEvent)
{
    void *message = MSGQ_CreateMsg((msgq_handler_t)GapAdvHandler, sizeof(gapAdvertisingEvent_t));
    assert(message);
    FLib_MemCpy(message, pAdvertisingEvent, sizeof(gapAdvertisingEvent_t));
    MSGQ_Put(s_messageQueue, message);
}

static void GapAdvHandler(gapAdvertisingEvent_t *pAdvertisingEvent)
{
    switch (pAdvertisingEvent->eventType)
    {
        case gAdvertisingStateChanged_c:
            mAdvState.advOn = !mAdvState.advOn;
            break;
        case gAdvertisingCommandFailed_c:
            panic(0, 0, 0, 0);
            break;
        default:; /* For MISRA compliance */
            break;
    }
}

static void GapConnCallback(deviceId_t peerDeviceId, gapConnectionEvent_t *pConnectionEvent)
{
    uint32_t msgLen   = sizeof(connectionMsg_t);
    uint32_t datasize = 0;

    if (pConnectionEvent->eventType == gConnEvtKeysReceived_c)
    {
        datasize = HCIOT_GapSmpKeysSize(pConnectionEvent->eventData.keysReceivedEvent.pKeys);
    }
    else if (pConnectionEvent->eventType == gConnEvtIqReportReceived_c)
    {
        msgLen += 2U * (uint32_t)pConnectionEvent->eventData.connIqReport.sampleCount;
    }
    else
    {
        /* MISRA compliance */
    }

    connectionMsg_t *connMsg = MSGQ_CreateMsg((msgq_handler_t)GapConnHandler, msgLen + datasize);

    assert(connMsg);
    connMsg->peer     = peerDeviceId;
    connMsg->datasize = datasize;
    FLib_MemCpy(&connMsg->Event, pConnectionEvent, sizeof(gapConnectionEvent_t));
    if (pConnectionEvent->eventType == gConnEvtKeysReceived_c)
    {
        HCIOT_CopyGapSmpKeys(connMsg->gapSmpKeys, pConnectionEvent->eventData.keysReceivedEvent.pKeys);
        connMsg->Event.eventData.keysReceivedEvent.pKeys = connMsg->gapSmpKeys;
    }
    else if (pConnectionEvent->eventType == gConnEvtIqReportReceived_c)
    {
        HCIOT_CopyGapConnIqReport(connMsg, &pConnectionEvent->eventData.connIqReport);
    }
    MSGQ_Put(s_messageQueue, connMsg);
}

static void GapConnHandler(connectionMsg_t *connMsg)
{
    deviceId_t peerDeviceId                = connMsg->peer;
    gapConnectionEvent_t *pConnectionEvent = &connMsg->Event;

    /* Connection Manager to handle Host Stack interactions */
    BleConnManager_GapPeripheralEvent(peerDeviceId, pConnectionEvent);

    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
            bleResult_t bleResult;
            PRINTF("Connected to peer device\n");
            mPeerDeviceId = peerDeviceId;

#if (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U))
            bool_t isBonded = FALSE;

            bleResult = Gap_CheckIfBonded(mPeerDeviceId, &isBonded, NULL);
            if ((bleResult == gBleSuccess_c) && (isBonded == FALSE))
            {
                if (isTimeSynchronized == FALSE)
                {
                    bleUuid_t uuid = {.uuid16 = gBleSig_CurrentTime_d};

                    /* Read CTS Characteristic. If the device doesn't have time services
                     gAttErrCodeAttributeNotFound_c will be received. */
                    GattClient_ReadUsingCharacteristicUuid(peerDeviceId, gBleUuidType16_c, &uuid, NULL,
                                                           mOutCharReadBuffer, 13, &mOutCharReadByteCount);
                }
            }
#endif /* gAppUseTimeService_d */
            /* Advertising stops when connected */
            mAdvState.advOn = FALSE;

            /* Subscribe client*/
            bleResult = Bas_Subscribe(&basServiceConfig, peerDeviceId);
            assert_equal(bleResult, gBleSuccess_c);
            bleResult = Hts_Subscribe(peerDeviceId);
            assert_equal(bleResult, gBleSuccess_c);
            bleResult = Hts_SetMeasurementInterval(&htsServiceConfig, 2000);
            assert_equal(bleResult, gBleSuccess_c);
            Hts_SendMeasurementIntervalIndication((uint16_t)value_measure_int);

            /* Stop Advertising Timer*/
            timer_status_t tmStatus = TM_Stop((timer_handle_t)mAdvTimerHandle);
            (void)tmStatus;
            assert_equal(tmStatus, kStatus_TimerSuccess);
            if (s_eventCallback)
            {
                s_eventCallback(kBLEMGR_Event_Connected);
            }
        }
        break;

        case gConnEvtDisconnected_c:
        {
            PRINTF("Lost connection to central, reason=0x%x\n", pConnectionEvent->eventData.disconnectedEvent.reason);

            /* Unsubscribe client */
            bleResult_t bleResult = Bas_Unsubscribe(&basServiceConfig, peerDeviceId);
            (void)bleResult;
            assert_equal(bleResult, gBleSuccess_c);
            bleResult = Hts_Unsubscribe();
            assert_equal(bleResult, gBleSuccess_c);

            mPeerDeviceId = gInvalidDeviceId_c;

            /* Restart advertising*/
            ResetAdvertise();
        }
        break;
        case gConnEvtPairingRequest_c:
        {
            PRINTF("Pairing request from peer device. Pass key = %d\n", gPasskeyValue_c);
        }
        break;

        case gConnEvtEncryptionChanged_c:
        {
            PRINTF("Encryption of connection changed\n");

#if (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U))
            if (isTimeSynchronized == FALSE)
            {
                bleUuid_t uuid = {.uuid16 = gBleSig_CurrentTime_d};

                /* Read CTS Characteristic. If the device doesn't have time services
                 gAttErrCodeAttributeNotFound_c will be received. */
                GattClient_ReadUsingCharacteristicUuid(peerDeviceId, gBleUuidType16_c, &uuid, NULL, mOutCharReadBuffer,
                                                       13, &mOutCharReadByteCount);
            }
#endif /* gAppUseTimeService_d */
        }
        break;

        case gConnEvtPairingComplete_c:
        {
            if (!pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful)
            {
                PRINTF("Pairing failed (reason=0x%x), restarting advertiser\n",
                       pConnectionEvent->eventData.pairingCompleteEvent.pairingCompleteData.failReason);
                bleResult_t result = Gap_Disconnect(peerDeviceId);
                (void)result;
                assert_equal(result, gBleSuccess_c);
            }
            else
            {
                PRINTF("Pairing complete\n");

#if (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U))
                if (isTimeSynchronized == FALSE)
                {
                    bleUuid_t uuid = {.uuid16 = gBleSig_CurrentTime_d};

                    /* Read CTS Characteristic. If the device doesn't have time services
                     gAttErrCodeAttributeNotFound_c will be received. */
                    GattClient_ReadUsingCharacteristicUuid(peerDeviceId, gBleUuidType16_c, &uuid, NULL,
                                                           mOutCharReadBuffer, 13, &mOutCharReadByteCount);
                }
#endif /* gAppUseTimeService_d */
            }
        }
        break;

        default:; /* For MISRA compliance */
            break;
    }
}

static void GattServerCallback(deviceId_t peerDeviceId, gattServerEvent_t *pServerEvent)
{
    uint32_t msgLen = sizeof(gattServerMsg_t);

    if (pServerEvent->eventType == gEvtAttributeWritten_c ||
        pServerEvent->eventType == gEvtAttributeWrittenWithoutResponse_c)
    {
        msgLen += pServerEvent->eventData.attributeWrittenEvent.cValueLength;
    }

    gattServerMsg_t *gattServerMsg = MSGQ_CreateMsg((msgq_handler_t)GattServerHandler, msgLen);

    assert(gattServerMsg);

    gattServerMsg->deviceId = peerDeviceId;
    FLib_MemCpy(&gattServerMsg->serverEvent, pServerEvent, sizeof(gattServerEvent_t));

    if ((gattServerMsg->serverEvent.eventType == gEvtAttributeWritten_c) ||
        (gattServerMsg->serverEvent.eventType == gEvtAttributeWrittenWithoutResponse_c))
    {
        /* Copy value after the gattServerEvent_t structure and update the aValue pointer*/
        gattServerMsg->serverEvent.eventData.attributeWrittenEvent.aValue =
            (uint8_t *)&gattServerMsg->serverEvent.eventData.attributeWrittenEvent.aValue + sizeof(uint8_t *);
        FLib_MemCpy(gattServerMsg->serverEvent.eventData.attributeWrittenEvent.aValue,
                    pServerEvent->eventData.attributeWrittenEvent.aValue,
                    pServerEvent->eventData.attributeWrittenEvent.cValueLength);
    }

    MSGQ_Put(s_messageQueue, gattServerMsg);
}

static void GattServerHandler(gattServerMsg_t *gattServerMsg)
{
    uint16_t handle;
    uint8_t status;
    deviceId_t deviceId             = gattServerMsg->deviceId;
    gattServerEvent_t *pServerEvent = &gattServerMsg->serverEvent;

    switch (pServerEvent->eventType)
    {
        case gEvtAttributeWritten_c:
        {
            bleResult_t bleResult;
            (void)bleResult;
            handle = pServerEvent->eventData.attributeWrittenEvent.handle;
            status = (uint8_t)gAttErrCodeNoError_c;

            if (handle == (uint16_t)value_measure_int)
            {
                attErrorCode_t result;
                uint16_t newValue = Utils_ExtractTwoByteValue(pServerEvent->eventData.attributeWrittenEvent.aValue);

                result = Hts_MeasurementIntervalWriting(&htsServiceConfig, (uint16_t)value_measure_int, newValue);

                /* Send response OTA */
                bleResult =
                    GattServer_SendAttributeWrittenStatus(deviceId, (uint16_t)value_measure_int, (uint8_t)result);
                assert_equal(bleResult, gBleSuccess_c);
            }
            else
            {
                bleResult = GattServer_SendAttributeWrittenStatus(deviceId, handle, status);
                assert_equal(bleResult, gBleSuccess_c);
            }
            break;
        }
        case gEvtHandleValueConfirmation_c:
        {
            break;
        }
        case gEvtError_c:
        {
            break;
        }
        default:; /* For MISRA compliance */
            break;
    }
}

static void AdvertisingTimerCallback(void *pParam)
{
    bleResult_t bleResult;
    (void)bleResult;
    /* Stop and restart advertising with new parameters */
    bleResult = Gap_StopAdvertising();
    assert_equal(bleResult, gBleSuccess_c);
    switch (mAdvState.advType)
    {
#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
        case fastWhiteListAdvState_c:
        {
            mAdvState.advType = fastAdvState_c;
        }
        break;
#endif /* gAppUseBonding_d */
        case fastAdvState_c:
        {
            mAdvState.advType = slowAdvState_c;
        }
        break;

        default:; /* For MISRA compliance */
            break;
    }
    StartAdvertise();
}

static void ResetAdvertise(void)
{
    if (mPeerDeviceId == gInvalidDeviceId_c)
    {
#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
        if (gcBondedDevices > 0U)
        {
            mAdvState.advType = fastWhiteListAdvState_c;
        }
        else
        {
#endif /* gAppUseBonding_d */
            mAdvState.advType = fastAdvState_c;
#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
        }
#endif /* gAppUseBonding_d */

        StartAdvertise();
    }
}

static bleResult_t StartAdvertise()
{
    bleResult_t result = gBleSuccess_c;

    uint32_t timeout = 0;

    PRINTF("Start ADV\n");
    switch (mAdvState.advType)
    {
#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
        case fastWhiteListAdvState_c:
        {
            gAdvParams.minInterval  = gFastConnMinAdvInterval_c;
            gAdvParams.maxInterval  = gFastConnMaxAdvInterval_c;
            gAdvParams.filterPolicy = gProcessFilterAcceptListOnly_c;
            timeout                 = gFastConnWhiteListAdvTime_c;
        }
        break;
#endif /* gAppUseBonding_d */
        case fastAdvState_c:
        {
            gAdvParams.minInterval  = gFastConnMinAdvInterval_c;
            gAdvParams.maxInterval  = gFastConnMaxAdvInterval_c;
            gAdvParams.filterPolicy = gProcessAll_c;
            timeout                 = gFastConnAdvTime_c - gFastConnWhiteListAdvTime_c;
        }
        break;

        case slowAdvState_c:
        {
            gAdvParams.minInterval  = gReducedPowerMinAdvInterval_c;
            gAdvParams.maxInterval  = gReducedPowerMinAdvInterval_c;
            gAdvParams.filterPolicy = gProcessAll_c;
            timeout                 = gReducedPowerAdvTime_c;
        }
        break;

        default:; /* For MISRA compliance */
            break;
    }

    /* Start advertising */
    result = Gap_SetAdvertisingParameters(&gAdvParams);

    /* Start advertising timer */
    if (result == gBleSuccess_c)
    {
        timer_status_t tmStatus = TM_InstallCallback((timer_handle_t)mAdvTimerHandle, AdvertisingTimerCallback, NULL);
        (void)tmStatus;
        assert_equal(tmStatus, kStatus_TimerSuccess);
        tmStatus =
            TM_Start((timer_handle_t)mAdvTimerHandle, kTimerModeLowPowerTimer | kTimerModeSetSecondTimer, timeout);
        assert_equal(tmStatus, kStatus_TimerSuccess);
    }
    if (s_eventCallback)
    {
        s_eventCallback(kBLEMGR_Event_AdvertisingStarted);
    }
    return result;
}

static void BatteryMeasurementTimerCallback(void *pParam)
{
    /* basServiceConfig.batteryLevel = 100; */
    static uint8_t tmp_bat = 100;
    if (tmp_bat == 0)
    {
        tmp_bat = 100;
    }
    else
    {
        tmp_bat -= 10;
    }
    basServiceConfig.batteryLevel = tmp_bat;
    bleResult_t bleResult         = Bas_RecordBatteryMeasurement(&basServiceConfig);
    (void)bleResult;
    assert_equal(bleResult, gBleSuccess_c);
}

status_t BLEMGR_Init(osa_msgq_handle_t messageQueue, blemgr_event_cb_t eventCallback)
{
    status_t status = kStatus_Success;
    s_eventCallback = eventCallback;
    uint8_t bdAddr[6];

    BLEController_ReadBdAddr(bdAddr);

    /* Append '_' and 2 least significant bytes of BLE address to the advertised name, if size permits. */
    uint32_t offset = strlen((const char *)gAppAdvertisingData.aAdStructures[2].aData);
    if (offset < gAppAdvertisingData.aAdStructures[2].length)
    {
        snprintf((char *)gAppAdvertisingData.aAdStructures[2].aData + offset,
                 gAppAdvertisingData.aAdStructures[2].length - offset, "_%02X%02X", bdAddr[1], bdAddr[0]);
    }

    /* Initialize memory manager */
    if (MEM_Init() != kStatus_MemSuccess)
    {
        status = kStatus_Fail;
    }

    if (status == kStatus_Success)
    {
        /* Copy the reference to the message queue */
        s_messageQueue = messageQueue;

        /* Cryptographic hardware initialization */
        SecLib_Init();

        /* RNG software initialization and PRNG initial seeding (from hardware) */
        if (RNG_Init() != gRngSuccess_d)
        {
            status = kStatus_Fail;
        }
    }

    if (status == kStatus_Success)
    {
        RNG_SetSeed();
#if defined(gAppUseNvm_d) && (gAppUseNvm_d > 0)
        /* Initialize NV module */
        if (NvModuleInit() != gNVM_OK_c)
        {
            status = kStatus_Fail;
        }
#endif /* gAppUseNvm_d */
    }

    if (status == kStatus_Success)
    {
        /* BLE Host Stack Init */
        if (BleInitialize() != gBleSuccess_c)
        {
            status = kStatus_Fail;
        }
    }

    return status;
}

void BLEMGR_SetTemperature(int16_t temperature)
{
    htsMeasurement_t tempMeas;

    tempMeas.tempTypePresent = TRUE;
    tempMeas.tempType        = gHts_Armpit_c;

    tempMeas.timeStampPresent = FALSE;

    tempMeas.unit = gHts_UnitInCelsius_c;
    /* MSB is exponent (in this case 10^-2), lowest 3 LSBs are mantissa (temperature in centigrades) */
    tempMeas.temperature = (ieee11073_32BitFloat_t)(0xFE000000 | (temperature & 0x00FFFFFF));

    bleResult_t bleResult = Hts_RecordTemperatureMeasurement((uint16_t)service_health_therm, &tempMeas);
    (void)bleResult;
    assert_equal(bleResult, gBleSuccess_c);
}

void BLEMGR_SetBatteryLevel(uint8_t level)
{
    basServiceConfig.batteryLevel = level;
    bleResult_t bleResult         = Bas_RecordBatteryMeasurement(&basServiceConfig);
    (void)bleResult;
    assert_equal(bleResult, gBleSuccess_c);
}

WEAK void BLEMGR_HostStackReady()
{
}
