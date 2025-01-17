/*! *********************************************************************************
 * \defgroup Temperature Sensor
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright 2015 Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2023 NXP
*
*
* \file
*
* This file is the interface file for the Temperature Sensor application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/

/* Profile Parameters */

#define gReducedPowerMinAdvInterval_c   1600 /* 1 s */
#define gReducedPowerMaxAdvInterval_c   4000 /* 2.5 s */

#define gAdvTime_c                      30 /* 30 s*/
#define gGoToSleepAfterDataTime_c       5 /* 5 s*/

#ifndef gAppDeepSleepMode_c
#define gAppDeepSleepMode_c 1
#endif

#ifndef gAppPrintLePhyEvent_c
#define gAppPrintLePhyEvent_c    0
#endif

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
extern gapAdvertisingData_t         gAppAdvertisingData;
extern gapScanResponseData_t        gAppScanRspData;
extern gapAdvertisingParameters_t   gAdvParams;
/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif


#endif /* TEMPERATURE_SENSOR_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
