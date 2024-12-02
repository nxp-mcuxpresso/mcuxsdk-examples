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

    return gBleSuccess_c;
}


/*! *********************************************************************************
* @}
********************************************************************************** */
