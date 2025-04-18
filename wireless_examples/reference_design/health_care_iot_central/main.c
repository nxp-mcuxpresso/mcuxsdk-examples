/*
 * Copyright 2023-2025 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * This BLE central application demonstrates how to connect to a specific
 * device.  In this example, it will try to connect to a device running the
 * MCXW23 reference application.
 *
 * It searches all nearby devices with name hciot_peripheral and with the Health
 * Thermometer service.
 *
 * Once found, a connection will be set up before the host stack is instructed
 * to discover all supported services. The list of services is checked for the
 * Health Thermometer service.
 *
 * The next step is to retrieve all the characteristics and descriptors. The
 * temperature service is selected and indications are enabled.
 *
 * After the indications are enabled, the host stack will inform the host stack
 * each time a temperature update is received.
 */

#define DELAY_INITIAL_CONNECTION

#include "board.h"
#include "app.h"
#include "fsl_component_timer_manager.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "hciot_common.h"
#include "fsl_os_abstraction.h"
#include "ble_general.h"
#include "ble_conn_manager.h"
#include "fsl_component_panic.h"
#include "gap_interface.h"
#include "msgq.h"
#include "RNG_Interface.h"
#include "ble_host_task_config.h"
#include "fsl_component_mem_manager.h"
#include "gatt_client_interface.h"
#include "math.h"
#include "ble_controller.h"
#include "nxp_hoststack_adapter.h"
#include "fsl_ctimer.h"
#include "fwk_platform.h"
#include "fwk_platform_ble.h"
#ifdef gAppUseNvm_d
#include "NVM_Interface.h"
#endif /* gAppUseNvm_d */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/** The priority of the main task */
#define MAIN_TASK_PRIO 1
/** The stack size of the connectivity task */
#define MAIN_TASK_STACK_SIZE 4096
/** The number of elements that the message queue can hold */
#define MESSAGE_QUEUE_DEPTH 10
/** The scan window */
#define SCAN_WINDOW 0x0010U
/** The scan interval */
#define SCAN_INTERVAL 0x0010U
/** Maximum number of services that can be discovered */
#define MAX_SERVICES 5
/** Maximum number of characteristics that can be discovered */
#define MAX_CHARACTERISTICS 20
/** Maximum number of character descriptors */
#define MAX_CHAR_DESCRIPTORS 10

#define CONVERT_CTIMER_COUNT_TO_SEC(n) ((float)n / (float)CLOCK_GetCTimerClkFreq(0))

/** Gatt procedure info */
typedef struct
{
    deviceId_t serverDeviceId;
    gattProcedureType_t procedureType;
    gattProcedureResult_t procedureResult;
    bleResult_t error;
} gattProcedureInfo_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void MainTask(void *argument);

static void GapGenericHandler(gapGenericEvent_t *pGenericEvent);
static bleResult_t EstablishConnection();
static void ConnectionCallback(deviceId_t peerDeviceId, gapConnectionEvent_t *pConnectionEvent);
static void ConnectionHandler(connectionMsg_t *connMsg);
static void GattClientProcedureCallback(deviceId_t serverDeviceId,
                                        gattProcedureType_t procedureType,
                                        gattProcedureResult_t procedureResult,
                                        bleResult_t error);
static void GattClientProcedureHandler(gattProcedureInfo_t *procedureInfo);
static void ScanningHandler(gapScanningEvent_t *pScanningEvent);

/*******************************************************************************
 * Variables
 ******************************************************************************/
#ifdef DELAY_INITIAL_CONNECTION
/* Delayed timer handle. */
static TIMER_MANAGER_HANDLE_DEFINE(s_delayTimerHandle);
#endif /* DELAY_INITIAL_CONNECTION */
/* Holds the current state of the scanner */
static bool_t mScanningOn = FALSE;
/** Handle of the main task */
static OSA_TASK_HANDLE_DEFINE(s_mainTaskHandle);
/** The main task */
static OSA_TASK_DEFINE(MainTask, MAIN_TASK_PRIO, 1, MAIN_TASK_STACK_SIZE, 0);
/** Handle of the queue that contains the messages for the main task */
static OSA_MSGQ_HANDLE_DEFINE(s_messageQueue, MESSAGE_QUEUE_DEPTH, sizeof(void *));
/** True when the device was found while scanning */
static bool_t s_deviceFound = FALSE;
/** Default Connection Request Parameters */
static gapConnectionRequestParameters_t s_connReqParams = {
    .scanInterval       = 36,
    .scanWindow         = 36,
    .filterPolicy       = (uint8_t)gUseDeviceAddress_c,
    .ownAddressType     = gBleAddrTypePublic_c,
    .connIntervalMin    = gcConnectionIntervalMinDefault_c,
    .connIntervalMax    = gcConnectionIntervalMinDefault_c,
    .connLatency        = 0,
    .supervisionTimeout = 500, /* in 10ms units. */
    .connEventLengthMin = 0,
    .connEventLengthMax = 0xFFFF,
    .initiatingPHYs     = (uint8_t)gLePhy1MFlag_c,
};
/** deviceId of the connected peer */
static deviceId_t s_peerDeviceId = gInvalidDeviceId_c;
/* Buffer used for Service Discovery */
static gattService_t s_serviceDiscoveryBuffer[MAX_SERVICES];
/** Holds the number of primary services in s_serviceDiscoveryBuffer */
static uint8_t mcPrimaryServices = 0;
/** Buffer used for Characteristic Descriptor Discovery */
static gattAttribute_t s_charDescriptorBuffer[MAX_CHAR_DESCRIPTORS];
/** Buffer used for Characteristic Discovery */
static gattCharacteristic_t s_sharDiscoveryBuffer[MAX_CHARACTERISTICS];
/** Health thermometer service */
static gattService_t *s_healthThermometerService = NULL;
/** Temperature characteristic */
static gattCharacteristic_t *s_temperatureCharacteristic = NULL;
/** Temperature cccd */
static gattAttribute_t *s_temperatureCccd = NULL;
/** Indicates whether setting the temperature indications is pending */
static bool_t s_setupTemperatureIndicationPending = FALSE;

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * Function called each time a BLE indication is received
 *
 * @param serverDeviceId The device ID of the sender of the notification
 * @param characteristicValueHandle The corresponding charachteristic
 * @param aValue Pointer to the new value
 * @param valueLength The length of the value
 */
static void GattClientIndicationCallback(deviceId_t serverDeviceId,
                                         uint16_t characteristicValueHandle,
                                         uint8_t *aValue,
                                         uint16_t valueLength)
{
    /** Is the indication coming from the temperature characteristic? */
    if (characteristicValueHandle == s_temperatureCharacteristic->value.handle)
    {
        /* Yes, is the length as expected? 6 = 1 option byte + 4 bytes value + 1 type byte */
        if (valueLength != 6)
        {
            PRINTF("Unexpected indication length received %d\n", valueLength);
        }
        else
        {
            /* Decode IEEE-11073 32 bit float */
            int8_t exponent   = *((int8_t *)(aValue + 4));
            uint32_t mantissa = (*((uint32_t *)(aValue + 1))) & 0x00FFFFFF;
            uint32_t ctime_s  = CONVERT_CTIMER_COUNT_TO_SEC(CTIMER_GetTimerCountValue(CTIMER0));
            PRINTF("Temperature indication received @%ds: %ld\n", ctime_s, (uint32_t)(mantissa * pow(10, exponent)));
        }
    }
    else
    {
        /* No, only indications of the temperature are enabled. This callback is unexpected */
        PRINTF("Unexpected indication callback\n");
    }
}

/**
 * Function called each time a GATT procedure finishes.
 *
 * This function creates and posts a message with all information that
 * is handled by the main task.
 *
 * @param serverDeviceId The device ID of the peripheral
 * @param procedureType The type of the procedure
 * @param procedureResult The result of the procedure
 * @param error Possible error code
 */
static void GattClientProcedureCallback(deviceId_t serverDeviceId,
                                        gattProcedureType_t procedureType,
                                        gattProcedureResult_t procedureResult,
                                        bleResult_t error)
{
    gattProcedureInfo_t *procedureInfo =
        MSGQ_CreateMsg((msgq_handler_t)&GattClientProcedureHandler, sizeof(gattProcedureInfo_t));

    assert(procedureInfo);
    procedureInfo->serverDeviceId  = serverDeviceId;
    procedureInfo->procedureType   = procedureType;
    procedureInfo->procedureResult = procedureResult;
    procedureInfo->error           = error;

    MSGQ_Put(s_messageQueue, (void *)procedureInfo);
}

static void EnableTemperatureIndication(void)
{
    uint16_t value = gCccdIndication_c;
    /* Cccd found, enabling indications */
    bleResult_t result = GattClient_WriteCharacteristicDescriptor(s_peerDeviceId, s_temperatureCccd, sizeof(uint16_t),
                                                                  (uint8_t *)&value);
    (void)result;
    assert_equal(result, gBleSuccess_c);
}

#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 0U))
extern gapPairingParameters_t gPairingParameters;
#endif /* gAppUsePairing_d */

/**
 * This function handles the GATT procedure finish
 *
 * @param procedureInfo Info of the procedure that finished
 */
static void GattClientProcedureHandler(gattProcedureInfo_t *procedureInfo)
{
    /* Has an error occurred? */
    if (procedureInfo->procedureResult == gGattProcError_c)
    {
        /* Yes, extract the error code */
        attErrorCode_t attError = (attErrorCode_t)(procedureInfo->error & 0xFF);
        /* Is it a privilege error? */
        if (attError == gAttErrCodeInsufficientEncryption_c || attError == gAttErrCodeInsufficientAuthorization_c ||
            attError == gAttErrCodeInsufficientAuthentication_c)
        {
            s_setupTemperatureIndicationPending = TRUE;

            /* Yes, increase privileges by starting pairing procedure */
#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
            bool_t isBonded  = FALSE;
            uint8_t nvmIndex = gInvalidNvmIndex_c;

            if ((gBleSuccess_c == Gap_CheckIfBonded(s_peerDeviceId, &isBonded, &nvmIndex)) && (isBonded))
            {
                PRINTF("Privilege error, encrypting link with device id %d...\n", s_peerDeviceId);
                (void)Gap_EncryptLink(s_peerDeviceId);
            }
            else
#endif /* gAppUseBonding_d */
            {
                PRINTF("Privilege error, pairing with device id %d...\n", s_peerDeviceId);
                Gap_Pair(s_peerDeviceId, &gPairingParameters);
            }
        }
    }
    else if (procedureInfo->procedureResult == gGattProcSuccess_c)
    {
        /* No error. Which procedure has finished? */
        switch (procedureInfo->procedureType)
        {
            case gGattProcExchangeMtu_c:
            {
                /* With the new MTU in place, the service discovery can start */
                /* Gap_Pair(serverDeviceId, &gPairingParameters); */
                PRINTF("MTU changed\n");

                /* Start Service Discovery*/
                GattClient_DiscoverAllPrimaryServices(s_peerDeviceId, s_serviceDiscoveryBuffer, MAX_SERVICES,
                                                      &mcPrimaryServices);
                break;
            }
            case gGattProcDiscoverAllPrimaryServices_c:
            {
                /* All primary services have been discovered. Check whether the health thermometer service is present */
                PRINTF("%d services discovered\n", mcPrimaryServices);
                for (uint32_t i = 0; i < mcPrimaryServices; i++)
                {
                    if (s_serviceDiscoveryBuffer[i].uuidType == 0x01 &&
                        s_serviceDiscoveryBuffer[i].uuid.uuid16 == gBleSig_HealthThermometerService_d)
                    {
                        PRINTF("Health thermometer service found\n");

                        s_healthThermometerService                   = s_serviceDiscoveryBuffer + i;
                        s_healthThermometerService->aCharacteristics = s_sharDiscoveryBuffer;

                        /* Start Characteristic Discovery for health thermometer service */
                        GattClient_DiscoverAllCharacteristicsOfService(s_peerDeviceId, s_healthThermometerService,
                                                                       MAX_CHARACTERISTICS);

                        break;
                    }
                }
                break;
            }
            case gGattProcDiscoverAllCharacteristics_c:
            {
                /* All characteristics are discovered. Check for temperature characteristic */
                PRINTF("%d health thermometer characteristics discovered\n",
                       s_healthThermometerService->cNumCharacteristics);
                for (uint32_t i = 0; i < s_healthThermometerService->cNumCharacteristics; i++)
                {
                    if (s_healthThermometerService->aCharacteristics[i].value.uuidType == 0x01 &&
                        s_healthThermometerService->aCharacteristics[i].value.uuid.uuid16 ==
                            gBleSig_TemperatureMeasurement_d)
                    {
                        PRINTF("Health thermometer temperature characteristic found\n");
                        s_temperatureCharacteristic               = s_healthThermometerService->aCharacteristics + i;
                        s_temperatureCharacteristic->aDescriptors = s_charDescriptorBuffer;

                        /* Check whether there are additional characteristic descriptors */
                        uint16_t nextHandle = i < s_healthThermometerService->cNumCharacteristics - 1 ?
                                                  s_healthThermometerService->aCharacteristics[i + 1].value.handle :
                                                  s_healthThermometerService->endHandle + 1;
                        if (s_temperatureCharacteristic->value.handle + 2 < nextHandle)
                        {
                            /* Yes, start discovering these descriptors */
                            GattClient_DiscoverAllCharacteristicDescriptors(s_peerDeviceId, s_temperatureCharacteristic,
                                                                            nextHandle, MAX_CHAR_DESCRIPTORS);
                        }
                        else
                        {
                            PRINTF("No descriptors found for temperature characteristic!\n");
                        }
                        break;
                    }
                }
                break;
            }
            case gGattProcDiscoverAllCharacteristicDescriptors_c:
            {
                /* All descriptors have been discovered. Check for cccd */
                PRINTF("%d descriptors found\n", s_temperatureCharacteristic->cNumDescriptors);
                for (uint32_t i = 0; i < s_temperatureCharacteristic->cNumDescriptors; i++)
                {
                    if (s_temperatureCharacteristic->aDescriptors[i].uuidType == 0x01 &&
                        s_temperatureCharacteristic->aDescriptors[i].uuid.uuid16 == gBleSig_CCCD_d)
                    {
                        PRINTF("Temperature client characteristic configuration descriptor found\n");
                        s_temperatureCccd = s_temperatureCharacteristic->aDescriptors + i;
                        EnableTemperatureIndication();
                        break;
                    }
                }
                break;
            }
            case gGattProcWriteCharacteristicDescriptor_c:
            {
                /* When this event is received, indications are enabled (only 1 descriptor is written by this app) */
                PRINTF("Temperature indications enabled\n");
#ifdef SLOW_CONN_INTERVAL
                PRINTF("Switching to slow connection interval\n");
                Gap_UpdateConnectionParameters(s_peerDeviceId, 1000, 1000, 0, 300, 0, 0xffff);
#endif /* SLOW_CONN_INTERVAL */
                break;
            }
            default:
                break;
        }
    }
}

/**
 * Callback function when the host stack has been initialized
 */
static void HostStackInitialized(void)
{
    /* Common GAP configuration */
    BleConnManager_GapCommonConfig();

    mScanningOn = FALSE;

    GattClient_RegisterProcedureCallback(GattClientProcedureCallback);
    GattClient_RegisterIndicationCallback(GattClientIndicationCallback);

    /* Establishing a connection starts when LE Phy command has finished.
     * See gLePhyEvent_c in GapGenericCallback */
}

/**
 * Checks whether the discovered device is the hciot peripheral.
 *
 * @param pData Data of the scanned device
 * @return True when the device matches the hciot_peripheral
 */
static bool_t CheckScannedDevice(gapScannedDevice_t *pData)
{
    uint8_t offset = 0;
    char name[32];
    bool_t deviceFound  = FALSE;
    bool_t serviceFound = FALSE;

    while (offset < pData->dataLength)
    {
        gapAdStructure_t adElement;

        adElement.length = pData->data[offset];
        adElement.adType = (gapAdType_t)pData->data[offset + 1];
        adElement.aData  = &pData->data[offset + 2];

        /* Search for Health Temperature Service */
        if ((adElement.adType == gAdIncomplete16bitServiceList_c) ||
            (adElement.adType == gAdComplete16bitServiceList_c))
        {
            serviceFound   = FALSE;
            uint16_t *uuid = (uint16_t *)adElement.aData;
            /* adElement.length holds the length of the element in bytes, including adType field. The number
             * of 16bit services is thus equal to (length - 1) / 2 */
            uint16_t serviceCount = (adElement.length - 1) / 2;

            /* adElement.length is equal to two times the number of services plus 1 */
            for (uint32_t i = 0; i < serviceCount; i++, uuid++)
            {
                PRINTF("  Service: 0x%x\n", *uuid);
                serviceFound = *uuid == gBleSig_HealthThermometerService_d;
            }
        }

        if ((adElement.adType == gAdShortenedLocalName_c) || (adElement.adType == gAdCompleteLocalName_c))
        {
            /* Check if the name matches HCIOT_PERIPHERAL */
            uint8_t nameLength = MIN(adElement.length, sizeof(name) - 1);
            memset(name, 0, sizeof(name));
            memcpy(name, adElement.aData, nameLength);
#if defined(PEER_NAME)
            deviceFound = 0 == strncmp(name, PEER_NAME, strlen(PEER_NAME));
            PRINTF("NAME: '%s' rssi: -%d.\n", name, -pData->rssi);
#else
            deviceFound = (0 == strncmp(name, "HCIOT", 5)) && (pData->rssi > PROXIMITY_RSSI_THRESHOLD);
            PRINTF("NAME: '%s' rssi: -%d, rssi threshold: -%d\n", name, -pData->rssi, -PROXIMITY_RSSI_THRESHOLD);
#endif /* PEER_NAME */
        }

        /* Move on to the next AD element type */
        offset += adElement.length + sizeof(uint8_t);
    }

    return serviceFound && deviceFound;
}

/**
 * Called when a new device has been discovered during the scanning phase.
 *
 * This function creates and posts a message with all information that
 * is handled by the main task.
 *
 * @param pScanningEvent Info about the newly discovered device
 */
static void ScanningCallback(gapScanningEvent_t *pScanningEvent)
{
    uint32_t msgLen = sizeof(uint32_t) + sizeof(gapScanningEvent_t);
    gapScanningEvent_t *evt;

    if (pScanningEvent->eventType == gDeviceScanned_c)
    {
        msgLen += pScanningEvent->eventData.scannedDevice.dataLength;
    }
    else if (pScanningEvent->eventType == gExtDeviceScanned_c)
    {
        msgLen += pScanningEvent->eventData.extScannedDevice.dataLength;
    }
    else if (pScanningEvent->eventType == gPeriodicDeviceScanned_c)
    {
        msgLen += pScanningEvent->eventData.periodicScannedDevice.dataLength;
    }
    else if (pScanningEvent->eventType == gConnectionlessIqReportReceived_c)
    {
        msgLen += 2U * pScanningEvent->eventData.iqReport.sampleCount;
    }
    else
    {
        /* msgLen does not modify for all other event types */
    }

    evt = MSGQ_CreateMsg((msgq_handler_t)&ScanningHandler, msgLen);

    assert(evt);

    evt->eventType = pScanningEvent->eventType;

    if (pScanningEvent->eventType == gScanCommandFailed_c)
    {
        evt->eventData.failReason = pScanningEvent->eventData.failReason;
    }
    else if (pScanningEvent->eventType == gDeviceScanned_c)
    {
        memcpy(&evt->eventData.scannedDevice, &pScanningEvent->eventData.scannedDevice,
               sizeof(pScanningEvent->eventData.scannedDevice));

        /*
         * Copy data after the gapScanningEvent_t structure and update
         * the data pointer
         */
        evt->eventData.scannedDevice.data = ((uint8_t *)evt) + sizeof(gapScanningEvent_t);
        memcpy(evt->eventData.scannedDevice.data, pScanningEvent->eventData.scannedDevice.data,
               pScanningEvent->eventData.scannedDevice.dataLength);
    }
    else if (pScanningEvent->eventType == gExtDeviceScanned_c)
    {
        memcpy(&evt->eventData.extScannedDevice, &pScanningEvent->eventData.extScannedDevice,
               sizeof(pScanningEvent->eventData.extScannedDevice));

        /*
         * Copy data after the gapScanningEvent_t structure and update
         * the data pointer
         */
        evt->eventData.extScannedDevice.pData = ((uint8_t *)evt) + sizeof(gapScanningEvent_t);
        memcpy(evt->eventData.extScannedDevice.pData, pScanningEvent->eventData.extScannedDevice.pData,
               pScanningEvent->eventData.extScannedDevice.dataLength);
    }
    else if (pScanningEvent->eventType == gPeriodicDeviceScanned_c)
    {
        memcpy(&evt->eventData.periodicScannedDevice, &pScanningEvent->eventData.periodicScannedDevice,
               sizeof(pScanningEvent->eventData.periodicScannedDevice));

        evt->eventData.periodicScannedDevice.pData = ((uint8_t *)evt) + sizeof(gapScanningEvent_t);
        memcpy(evt->eventData.periodicScannedDevice.pData, pScanningEvent->eventData.periodicScannedDevice.pData,
               pScanningEvent->eventData.periodicScannedDevice.dataLength);
    }
    else if (pScanningEvent->eventType == gPeriodicAdvSyncEstablished_c)
    {
        memcpy(&evt->eventData.syncEstb, &pScanningEvent->eventData.syncEstb,
               sizeof(pScanningEvent->eventData.syncEstb));
    }
    else if (pScanningEvent->eventType == gPeriodicAdvSyncLost_c)
    {
        memcpy(&evt->eventData.syncLost, &pScanningEvent->eventData.syncLost,
               sizeof(pScanningEvent->eventData.syncLost));
    }
    else if (pScanningEvent->eventType == gConnectionlessIqReportReceived_c)
    {
        memcpy(&evt->eventData.iqReport, &pScanningEvent->eventData.iqReport,
               sizeof(pScanningEvent->eventData.iqReport));
        evt->eventData.iqReport.aI_samples = ((int8_t *)evt) + sizeof(gapScanningEvent_t);
        memcpy(evt->eventData.iqReport.aI_samples, pScanningEvent->eventData.iqReport.aI_samples,
               pScanningEvent->eventData.iqReport.sampleCount);
        evt->eventData.iqReport.aQ_samples =
            ((int8_t *)evt) + sizeof(gapScanningEvent_t) + pScanningEvent->eventData.iqReport.sampleCount;
        memcpy(evt->eventData.iqReport.aQ_samples, pScanningEvent->eventData.iqReport.aQ_samples,
               pScanningEvent->eventData.iqReport.sampleCount);
    }
    else
    {
        /* no action for all other event types */
    }

    MSGQ_Put(s_messageQueue, evt);
}

/**
 * Handles the scan event.
 *
 * @param pScanningEvent Info about the scan event
 */
static void ScanningHandler(gapScanningEvent_t *pScanningEvent)
{
    switch (pScanningEvent->eventType)
    {
        case gDeviceScanned_c:
        {
            uint8_t *a = pScanningEvent->eventData.scannedDevice.aAddress;
            PRINTF("New device: %x:%x:%x:%x:%x:%x\n", a[0], a[1], a[2], a[3], a[4], a[5]);
            bool match = CheckScannedDevice(&pScanningEvent->eventData.scannedDevice);
            if (match)
            {
                PRINTF("Device found!\n");
                s_connReqParams.peerAddressType = pScanningEvent->eventData.scannedDevice.addressType;
                memcpy(s_connReqParams.peerAddress, pScanningEvent->eventData.scannedDevice.aAddress,
                       sizeof(bleDeviceAddress_t));
#if gAppUsePrivacy_d
                s_connReqParams.usePeerIdentityAddress =
                    pScanningEvent->eventData.scannedDevice.advertisingAddressResolved;
#endif /* gAppUsePrivacy_d */
                s_deviceFound = TRUE;
                Gap_StopScanning();
            }
            break;
        }
        break;

        case gScanStateChanged_c:
        {
            mScanningOn = !mScanningOn;
            PRINTF("Scanning %s\n", mScanningOn ? "on" : "off");

            if (!mScanningOn)
            {
                /* Scanning stopped. Did we find the peripheral? */
                if (s_deviceFound)
                {
                    PRINTF("Connecting...\r\n");
                    bleResult_t result = Gap_Connect(&s_connReqParams, ConnectionCallback);
                    assert_equal(result, gBleSuccess_c);
                    (void)result;
                }
            }
        }
        break;

        case gScanCommandFailed_c:
        {
            panic(0, 0, 0, 0);
            break;
        }
        default:
            break;
    }
}

/**
 * Called each time a connection related event occurs
 *
 * This function creates and posts a message with all information that
 * is handled by the main task.
 *
 * @param pConnectionEvent Info about the connection event
 */
static void ConnectionCallback(deviceId_t peerDeviceId, gapConnectionEvent_t *pConnectionEvent)
{
    uint32_t datasize = 0;

    if (gConnEvtAuthenticationRejected_c == pConnectionEvent->eventType)
    {
        PRINTF("Authentication rejected. Reason = 0x%x\n",
               pConnectionEvent->eventData.authenticationRejectedEvent.rejectReason);
    }
    else if (pConnectionEvent->eventType == gConnEvtKeysReceived_c)
    {
        datasize = HCIOT_GapSmpKeysSize(pConnectionEvent->eventData.keysReceivedEvent.pKeys);
    }
    else if (pConnectionEvent->eventType == gConnEvtIqReportReceived_c)
    {
        datasize = 2U * (uint32_t)pConnectionEvent->eventData.connIqReport.sampleCount;
    }
    else
    {
        /* MISRA compliance */
    }

    connectionMsg_t *connMsg = MSGQ_CreateMsg((msgq_handler_t)ConnectionHandler, sizeof(connectionMsg_t) + datasize);

    assert(connMsg);

    connMsg->peer     = peerDeviceId;
    connMsg->datasize = datasize;
    memcpy(&connMsg->Event, pConnectionEvent, sizeof(gapConnectionEvent_t));
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

/**
 * Handles the connection event
 *
 * @param connMsg Info about the connection event
 */
static void ConnectionHandler(connectionMsg_t *connMsg)
{
    deviceId_t peerDeviceId                = connMsg->peer;
    gapConnectionEvent_t *pConnectionEvent = &connMsg->Event;

    if (gConnEvtConnected_c != pConnectionEvent->eventType)
    {
        /* Connection Manager to handle Host Stack interactions */
        BleConnManager_GapCentralEvent(peerDeviceId, pConnectionEvent);
    }

    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
            PRINTF("Connected to peer device\n");
            s_peerDeviceId = peerDeviceId;
            GattClient_ExchangeMtu(peerDeviceId, gAttMaxMtu_c);
            break;
        }

        case gConnEvtDisconnected_c:
        {
            PRINTF("Lost connection to peripheral, reason=%d\n", pConnectionEvent->eventData.disconnectedEvent.reason);
            s_peerDeviceId     = gInvalidDeviceId_c;
            bleResult_t result = EstablishConnection();
            assert_equal(result, gBleSuccess_c);
            (void)result;
            break;
        }

        case gConnEvtPairingComplete_c:
        {
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful)
            {
                PRINTF("Successfully paired\n");
            }
            else
            {
                PRINTF("Pairing failed, reason = 0x%x\n",
                       pConnectionEvent->eventData.pairingCompleteEvent.pairingCompleteData.failReason);
                Gap_Disconnect(peerDeviceId);
            }
        }
        case gConnEvtEncryptionChanged_c:
        {
            if (pConnectionEvent->eventData.encryptionChangedEvent.newEncryptionState == TRUE)
            {
                if (s_setupTemperatureIndicationPending)
                {
                    PRINTF("Link is encrypted. Retrying temperature indication setup\n");
                    s_setupTemperatureIndicationPending = FALSE;
                    EnableTemperatureIndication();
                }
            }
        }

        default:
            break;
    }
}

/**
 * Starts the scanner
 *
 * @return gBleSuccess_c in case of success, an error code otherwise
 */
static bleResult_t EstablishConnection()
{
    s_deviceFound = FALSE;
    bleResult_t result;
#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
    gapIdentityInformation_t info;
    uint8_t count;
    result = Gap_GetBondedDevicesIdentityInformation(&info, 1, &count);
    assert_equal(result, gBleSuccess_c);
    /* Already bonded? */
    if (count > 0)
    {
        /* Yes, connect immediately to the bonded device */
        s_connReqParams.peerAddressType = info.identityAddress.idAddressType;
        memcpy(s_connReqParams.peerAddress, info.identityAddress.idAddress, sizeof(bleDeviceAddress_t));
        s_connReqParams.usePeerIdentityAddress = true;
        PRINTF("Connecting to %x:%x:%x:%x:%x:%x...\r\n", s_connReqParams.peerAddress[5], s_connReqParams.peerAddress[4],
               s_connReqParams.peerAddress[3], s_connReqParams.peerAddress[2], s_connReqParams.peerAddress[1],
               s_connReqParams.peerAddress[0]);
        result = Gap_Connect(&s_connReqParams, ConnectionCallback);
        assert_equal(result, gBleSuccess_c);
        (void)result;
    }
    else
    {
#endif /* gAppUseBonding_d */
        /* No bond exists yet. First scan for a device that meets the criteria */
        PRINTF("Start scanning for peripheral\n");
        result = Gap_StartScanning(&(gapScanningParameters_t){/* type */ gScanTypeActive_c,
                                                              /* interval */ SCAN_INTERVAL,
                                                              /* window */ SCAN_WINDOW,
                                                              /* ownAddressType */ gBleAddrTypePublic_c,
                                                              /* filterPolicy */ (uint8_t)gScanAll_c,
                                                              /* scanning PHY */ (uint8_t)gLePhy1MFlag_c},
                                   ScanningCallback, gGapDuplicateFilteringDisable_c, gGapScanContinuously_d,
                                   gGapScanPeriodicDisabled_d);
#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
    }
#endif /* gAppUsePrivacy_d */
    return result;
}

#ifdef DELAY_INITIAL_CONNECTION
static void DelayedStart(void *param)
{
    EstablishConnection();
}
#endif /* DELAY_INITIAL_CONNECTION */

/**
 * Called each time a generic GAP related event occurs
 *
 * This function creates and posts a message with all information that
 * is handled by the main task.
 *
 * @param pGenericEvent Info about the generic GAP event
 */
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
        case gLePhyEvent_c:
        {
#ifdef DELAY_INITIAL_CONNECTION
            /* After LE SetPhy completed, the host stack still sends a SetAddressResolutionEnable command which
             * is not reported by the host stack. Therefore, we use a timer to allow this command to finish before
             * create connection is sent. */
            timer_status_t tmStatus = TM_Open(s_delayTimerHandle);
            (void)tmStatus;
            assert_equal(tmStatus, kStatus_TimerSuccess);
            tmStatus = TM_InstallCallback((timer_handle_t)s_delayTimerHandle, DelayedStart, NULL);
            assert_equal(tmStatus, kStatus_TimerSuccess);
            tmStatus = TM_Start((timer_handle_t)s_delayTimerHandle, kTimerModeSingleShot | kTimerModeSetSecondTimer, 1);
            assert_equal(tmStatus, kStatus_TimerSuccess);
#else
            bleResult_t result = EstablishConnection();
            assert_equal(result, gBleSuccess_c);
            (void)result;
#endif /* DELAY_INITIAL_CONNECTION */
            break;
        }
        default:
            break;
    }
    void *message = MSGQ_CreateMsg((msgq_handler_t)&GapGenericHandler, sizeof(gapGenericEvent_t));
    assert(message);
    memcpy(message, pGenericEvent, sizeof(gapGenericEvent_t));
    MSGQ_Put(s_messageQueue, message);
}

/**
 * Handles the generic GAP event
 *
 * @param pGenericEvent Info about the GAP generic event
 */
static void GapGenericHandler(gapGenericEvent_t *pGenericEvent)
{
    /* Inform BLE Conn Manager */
    BleConnManager_GenericEvent(pGenericEvent);

    switch (pGenericEvent->eventType)
    {
        case gInitializationComplete_c:
            HostStackInitialized();
            break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/**
 * Initializes all BLE related components
 *
 * @return gBleSuccess_c in case of success, an error code otherwise
 */
static bleResult_t BleInitialize()
{
    /* BLE Controller Init */
    if (kBLEC_Success != BLEController_SetTaskStackSize((uint32_t)gControllerTaskStackSize_c,
                                                        (uint32_t)gControllerHighPriorityTaskStackSize_c))
    {
        return gBleOsError_c;
    }

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

/**
 * Initializes the system
 *
 * @return kStatus_Success in case of success, an error code otherwhise
 */
static status_t InitSystem(void)
{
    status_t status = kStatus_Success;

    MSGQ_CreateQueue(s_messageQueue, MESSAGE_QUEUE_DEPTH, sizeof(void *));

    /* Initialize memory manager */
    if (MEM_Init() != kStatus_MemSuccess)
    {
        status = kStatus_Fail;
    }

    if (status == kStatus_Success)
    {
        /* Initialize timer manager */
        PLATFORM_InitTimerManager();

        /* Cryptographic hardware initialization */
        SecLib_Init();

        /* RNG software initialization and PRNG initial seeding (from hardware) */
        if (RNG_Init() != gRngSuccess_d)
        {
            PRINTF("RNG_Init fail\n");
            status = kStatus_Fail;
        }
    }

    if (status == kStatus_Success)
    {
#if defined(gAppUseNvm_d) && (gAppUseNvm_d > 0)
        /* Initialize NV module */
        if (NvModuleInit() != gNVM_OK_c)
        {
            PRINTF("NvModuleInit fail\n");
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

/**
 * The main task. This task will process all queued messages
 *
 * @param argument Not used
 */
static void MainTask(void *argument)
{
    while (true)
    {
        void *message = MSGQ_Get(s_messageQueue, osaWaitForever_c);
        if (message == NULL)
        {
            break;
        }
        MSGQ_GetHandler(message)(message);
        MSGQ_FreeMsg(message);
    }
}

#if (gAppUseNvm_d)
void vApplicationIdleHook(void)
{
    NvIdle();
}
#endif /* gAppUseNvm_d */

/*!
 * @brief Main function
 */
int main(void)
{
    osa_status_t osaStatus;
    status_t status;
    (void)osaStatus;
    (void)status;

    /* Init OSA: should be called before any other OSA API */
    OSA_Init();

    BOARD_InitHardware();

    PLATFORM_InitBle();

    HCIOT_InitSupplyMode();

    osaStatus = OSA_TaskCreate((osa_task_handle_t)s_mainTaskHandle, OSA_TASK(MainTask), NULL);
    assert_equal(osaStatus, KOSA_StatusSuccess);

    status = InitSystem();
    assert_equal(status, kStatus_Success);

    while (!gUseRtos_c)
    {
        /* Handle all LL events */
        BLEController_EmngrHandleAllEvents();
        /* Handle OS tasks */
        OSA_ProcessTasks();

        if (!gUseRtos_c)
        {
            //NvIdle();
        }

        OSA_DisableIRQGlobal();

        /* Check if some connectivity tasks have turned to ready state from interrupts or
              if messages are to be processed in Application process */
        if ((OSA_TaskShouldYield() == FALSE) && BLEController_EmngrIsIdle())
        {
            __WFI();
        }

        OSA_EnableIRQGlobal();
    }
    OSA_Start();
    /*won't run here*/
    assert(0);

    return 0;
}
