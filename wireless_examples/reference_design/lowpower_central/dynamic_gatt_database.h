/*! *********************************************************************************
* \defgroup GATT_DB
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the creation of a Dynamic Gatt Database
* in application.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef _DYNAMIC_GATT_DATABASE_H_
#define _DYNAMIC_GATT_DATABASE_H_


#define service_gatt                       1
#define char_service_changed               2
#define value_service_changed              3
#define cccd_service_changed               4

#define service_gap                        6
#define char_device_name                   7
#define value_device_name                  8


#ifdef __cplusplus
extern "C" {
#endif

bleResult_t GattDbDynamic_CreateDatabase(void);

#ifdef __cplusplus
}
#endif 


#endif /* _DYNAMIC_GATT_DATABASE_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
