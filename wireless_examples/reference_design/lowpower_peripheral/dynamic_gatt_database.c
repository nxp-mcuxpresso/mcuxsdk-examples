/*! *********************************************************************************
* \addtogroup GATT_DB
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2018,2024 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "board.h"
#include "gap_types.h"
#include "gatt_db_dynamic.h"
#include "dynamic_gatt_database.h"
#include "gatt_uuid_decl_x.h"

/*! *********************************************************************************
* \brief    GattDbDynamic_AddPrimaryServiceGatt
********************************************************************************** */
bleResult_t GattDbDynamic_AddPrimaryServiceGatt(void)
{
    bleUuid_t   uuid;
    uint16_t    handle;

    uuid.uuid16 = gBleSig_GenericAttributeProfile_d;

    return GattDbDynamic_AddPrimaryServiceDeclaration(service_gatt, 
                                                      gBleUuidType16_c, 
                                                      &uuid, 
                                                      &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddCharServiceChanged
********************************************************************************** */
bleResult_t GattDbDynamic_AddCharServiceChanged(void)
{
    bleUuid_t   uuid;
    uint8_t     aInitialValue[] = {0x00, 0x00, 0x00, 0x00};
    uint16_t    handle;

    uuid.uuid16 = gBleSig_GattServiceChanged_d;

    return GattDbDynamic_AddCharacteristicDeclarationAndValue(gBleUuidType16_c, 
                                                              &uuid, 
                                                              (gattCharacteristicPropertiesBitFields_t)(gGattCharPropRead_c | gGattCharPropNotify_c), 
                                                              0, 
                                                              4, 
                                                              aInitialValue, 
                                                              (gattAttributePermissionsBitFields_t)(gPermissionNone_c), 
                                                              &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddCccdServiceChanged
********************************************************************************** */
bleResult_t GattDbDynamic_AddCccdServiceChanged(void)
{
    uint16_t handle;

    return GattDbDynamic_AddCccd(&handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddPrimaryServiceGap
********************************************************************************** */
bleResult_t GattDbDynamic_AddPrimaryServiceGap(void)
{
    bleUuid_t   uuid;
    uint16_t    handle;

    uuid.uuid16 = gBleSig_GenericAccessProfile_d;

    return GattDbDynamic_AddPrimaryServiceDeclaration(service_gap, 
                                                      gBleUuidType16_c, 
                                                      &uuid, 
                                                      &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddCharDeviceName
********************************************************************************** */
bleResult_t GattDbDynamic_AddCharDeviceName(void)
{
    bleUuid_t   uuid;
    uint8_t     aInitialValue[] = "NXP_BLE_TEMP";
    uint16_t    handle;

    uuid.uuid16 = gBleSig_GapDeviceName_d;

    return GattDbDynamic_AddCharacteristicDeclarationAndValue(gBleUuidType16_c, 
                                                              &uuid, 
                                                              (gattCharacteristicPropertiesBitFields_t)(gGattCharPropRead_c), 
                                                              0, 
                                                              12, 
                                                              aInitialValue, 
                                                              (gattAttributePermissionsBitFields_t)(gPermissionFlagReadable_c), 
                                                              &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddPrimaryServiceTemperature
********************************************************************************** */
bleResult_t GattDbDynamic_AddPrimaryServiceTemperature(void)
{
    bleUuid_t   uuid;
    uint16_t    handle;

    FLib_MemCpy(uuid.uuid128, uuid_service_temperature, 16);

    return GattDbDynamic_AddPrimaryServiceDeclaration(service_temperature, 
                                                      gBleUuidType128_c, 
                                                      &uuid, 
                                                      &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddCharTemperature
********************************************************************************** */
bleResult_t GattDbDynamic_AddCharTemperature(void)
{
    bleUuid_t   uuid;
    uint8_t     aInitialValue[] = {0x00, 0xB4};
    uint16_t    handle;

    uuid.uuid16 = gBleSig_Temperature_d;

    return GattDbDynamic_AddCharacteristicDeclarationAndValue(gBleUuidType16_c, 
                                                              &uuid, 
                                                              (gattCharacteristicPropertiesBitFields_t)(gGattCharPropNotify_c), 
                                                              0, 
                                                              2, 
                                                              aInitialValue, 
                                                              (gattAttributePermissionsBitFields_t)(gPermissionNone_c), 
                                                              &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddDescTemperature
********************************************************************************** */
bleResult_t GattDbDynamic_AddDescTemperature(void)
{
    bleUuid_t   uuid;
    uint8_t     aInitialValue[] = {0x0E, 0xFE, 0x2F, 0x27, 0x00, 0x00, 0x00};
    uint16_t    handle;

    uuid.uuid16 = gBleSig_CharPresFormatDescriptor_d;

    return GattDbDynamic_AddCharDescriptor(gBleUuidType16_c, 
                                           &uuid, 
                                           7, 
                                           aInitialValue, 
                                           (gattAttributePermissionsBitFields_t)(gPermissionFlagReadable_c), 
                                           &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddCccdTemperature
********************************************************************************** */
bleResult_t GattDbDynamic_AddCccdTemperature(void)
{
    uint16_t handle;

    return GattDbDynamic_AddCccd(&handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddPrimaryServiceBattery
********************************************************************************** */
bleResult_t GattDbDynamic_AddPrimaryServiceBattery(void)
{
    bleUuid_t   uuid;
    uint16_t    handle;

    uuid.uuid16 = gBleSig_BatteryService_d;

    return GattDbDynamic_AddPrimaryServiceDeclaration(service_battery, 
                                                      gBleUuidType16_c, 
                                                      &uuid, 
                                                      &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddCharBatteryLevel
********************************************************************************** */
bleResult_t GattDbDynamic_AddCharBatteryLevel(void)
{
    bleUuid_t   uuid;
    uint8_t     aInitialValue[] = {0x5A};
    uint16_t    handle;

    uuid.uuid16 = gBleSig_BatteryLevel_d;

    return GattDbDynamic_AddCharacteristicDeclarationAndValue(gBleUuidType16_c, 
                                                              &uuid, 
                                                              (gattCharacteristicPropertiesBitFields_t)(gGattCharPropNotify_c | gGattCharPropRead_c), 
                                                              0, 
                                                              1, 
                                                              aInitialValue, 
                                                              (gattAttributePermissionsBitFields_t)(gPermissionFlagReadable_c), 
                                                              &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddDescBatLevel
********************************************************************************** */
bleResult_t GattDbDynamic_AddDescBatLevel(void)
{
    bleUuid_t   uuid;
    uint8_t     aInitialValue[] = {0x04, 0x00, 0xAD, 0x27, 0x01, 0x01, 0x00};
    uint16_t    handle;

    uuid.uuid16 = gBleSig_CharPresFormatDescriptor_d;

    return GattDbDynamic_AddCharDescriptor(gBleUuidType16_c, 
                                           &uuid, 
                                           7, 
                                           aInitialValue, 
                                           (gattAttributePermissionsBitFields_t)(gPermissionFlagReadable_c), 
                                           &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddCccdBatteryLevel
********************************************************************************** */
bleResult_t GattDbDynamic_AddCccdBatteryLevel(void)
{
    uint16_t handle;

    return GattDbDynamic_AddCccd(&handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddPrimaryServiceDeviceInfo
********************************************************************************** */
bleResult_t GattDbDynamic_AddPrimaryServiceDeviceInfo(void)
{
    bleUuid_t   uuid;
    uint16_t    handle;

    uuid.uuid16 = gBleSig_DeviceInformationService_d;

    return GattDbDynamic_AddPrimaryServiceDeclaration(service_device_info, 
                                                      gBleUuidType16_c, 
                                                      &uuid, 
                                                      &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddCharManufName
********************************************************************************** */
bleResult_t GattDbDynamic_AddCharManufName(void)
{
    bleUuid_t   uuid;
    uint8_t     aInitialValue[] = MANUFACTURER_NAME;
    uint16_t    handle;

    uuid.uuid16 = gBleSig_ManufacturerNameString_d;

    return GattDbDynamic_AddCharacteristicDeclarationAndValue(gBleUuidType16_c, 
                                                              &uuid, 
                                                              (gattCharacteristicPropertiesBitFields_t)(gGattCharPropRead_c), 
                                                              0, 
                                                              sizeof(MANUFACTURER_NAME), 
                                                              aInitialValue, 
                                                              (gattAttributePermissionsBitFields_t)(gPermissionFlagReadable_c), 
                                                              &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddCharModelNo
********************************************************************************** */
bleResult_t GattDbDynamic_AddCharModelNo(void)
{
    bleUuid_t   uuid;
    uint8_t     aInitialValue[] = "Temp Sensor Demo";
    uint16_t    handle;

    uuid.uuid16 = gBleSig_ModelNumberString_d;

    return GattDbDynamic_AddCharacteristicDeclarationAndValue(gBleUuidType16_c, 
                                                              &uuid, 
                                                              (gattCharacteristicPropertiesBitFields_t)(gGattCharPropRead_c), 
                                                              0, 
                                                              16, 
                                                              aInitialValue, 
                                                              (gattAttributePermissionsBitFields_t)(gPermissionFlagReadable_c), 
                                                              &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddCharSerialNo
********************************************************************************** */
bleResult_t GattDbDynamic_AddCharSerialNo(void)
{
    bleUuid_t   uuid;
    uint8_t     aInitialValue[] = "BLESN01";
    uint16_t    handle;

    uuid.uuid16 = gBleSig_SerialNumberString_d;

    return GattDbDynamic_AddCharacteristicDeclarationAndValue(gBleUuidType16_c, 
                                                              &uuid, 
                                                              (gattCharacteristicPropertiesBitFields_t)(gGattCharPropRead_c), 
                                                              0, 
                                                              7, 
                                                              aInitialValue, 
                                                              (gattAttributePermissionsBitFields_t)(gPermissionFlagReadable_c), 
                                                              &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddCharHwRev
********************************************************************************** */
bleResult_t GattDbDynamic_AddCharHwRev(void)
{
    bleUuid_t   uuid;
    uint8_t     aInitialValue[] = BOARD_NAME;
    uint16_t    handle;

    uuid.uuid16 = gBleSig_HardwareRevisionString_d;

    return GattDbDynamic_AddCharacteristicDeclarationAndValue(gBleUuidType16_c, 
                                                              &uuid, 
                                                              (gattCharacteristicPropertiesBitFields_t)(gGattCharPropRead_c), 
                                                              0, 
                                                              sizeof(BOARD_NAME), 
                                                              aInitialValue, 
                                                              (gattAttributePermissionsBitFields_t)(gPermissionFlagReadable_c), 
                                                              &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddCharFwRev
********************************************************************************** */
bleResult_t GattDbDynamic_AddCharFwRev(void)
{
    bleUuid_t   uuid;
    uint8_t     aInitialValue[] = "1.1.1";
    uint16_t    handle;

    uuid.uuid16 = gBleSig_FirmwareRevisionString_d;

    return GattDbDynamic_AddCharacteristicDeclarationAndValue(gBleUuidType16_c, 
                                                              &uuid, 
                                                              (gattCharacteristicPropertiesBitFields_t)(gGattCharPropRead_c), 
                                                              0, 
                                                              5, 
                                                              aInitialValue, 
                                                              (gattAttributePermissionsBitFields_t)(gPermissionFlagReadable_c), 
                                                              &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_AddCharSwRev
********************************************************************************** */
bleResult_t GattDbDynamic_AddCharSwRev(void)
{
    bleUuid_t   uuid;
    uint8_t     aInitialValue[] = "1.1.4";
    uint16_t    handle;

    uuid.uuid16 = gBleSig_SoftwareRevisionString_d;

    return GattDbDynamic_AddCharacteristicDeclarationAndValue(gBleUuidType16_c, 
                                                              &uuid, 
                                                              (gattCharacteristicPropertiesBitFields_t)(gGattCharPropRead_c), 
                                                              0, 
                                                              5, 
                                                              aInitialValue, 
                                                              (gattAttributePermissionsBitFields_t)(gPermissionFlagReadable_c), 
                                                              &handle);
}


/*! *********************************************************************************
* \brief    GattDbDynamic_CreateDatabase
********************************************************************************** */
bleResult_t GattDbDynamic_CreateDatabase(void)
{
    bleResult_t result;

    result = GattDbDynamic_AddPrimaryServiceGatt();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddCharServiceChanged();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddCccdServiceChanged();
    if(gBleSuccess_c != result) {return result;}

    result = GattDbDynamic_AddPrimaryServiceGap();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddCharDeviceName();
    if(gBleSuccess_c != result) {return result;}

    result = GattDbDynamic_AddPrimaryServiceTemperature();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddCharTemperature();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddDescTemperature();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddCccdTemperature();
    if(gBleSuccess_c != result) {return result;}

    result = GattDbDynamic_AddPrimaryServiceBattery();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddCharBatteryLevel();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddDescBatLevel();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddCccdBatteryLevel();
    if(gBleSuccess_c != result) {return result;}

    result = GattDbDynamic_AddPrimaryServiceDeviceInfo();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddCharManufName();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddCharModelNo();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddCharSerialNo();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddCharHwRev();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddCharFwRev();
    if(gBleSuccess_c != result) {return result;}
    result = GattDbDynamic_AddCharSwRev();
    if(gBleSuccess_c != result) {return result;}

    return gBleSuccess_c;
}


/*! *********************************************************************************
* @}
********************************************************************************** */
