/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_gpadc.h"
#include "board.h"
#include "app.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Read voltage how many times. */
#ifndef APP_VOLT_READ_TIMES
#define APP_VOLT_READ_TIMES 10
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int APP_WaitString(uint8_t *pBuf, uint16_t wSize)
{
    uint8_t r;
    uint16_t rBytes = 0;

    do
    {
        do
        {
            r = GETCHAR();
        } while (((r == 0x0A) || (r == 0x0D)) && (rBytes == 0));

        /* If Char is Enter break */
        if ((r == 0x0A) || (r == 0x0D))
        {
            break;
        }
        pBuf[rBytes++] = (uint8_t)r;
    } while (rBytes < wSize);

    return rBytes;
}

static void APP_GpadcExtPinVoltage(uint16_t readTimes)
{
    uint16_t i;
    int32_t volt;
    pn_status_t pnStatus;

    PRINTF("\r\nTest the GPADC Driver by external pin voltage (Input must be within 0 V - 1.8 V)\r\n");
	pnStatus = GPADC_Init();
	if (pnStatus != kStatus_PN_Success)
    {
        PRINTF("\r\n ERROR: GPADC Init Failed\r\n");
        return;
    }
	
    pnStatus = GPADC_ExtPin_Init();

    if (pnStatus != kStatus_PN_Success)
    {
        PRINTF("\r\n ERROR: GPADC ExtPin Init Failed\r\n");
        return;
    }

    for (i = 0; i < readTimes; i++)
    {
        PRINTF("Read %d time: ", i);
        /* Reading the voltage value from GPADC. */
        pnStatus = GPADC_ExtPin_Read(&volt);

        if (pnStatus == kStatus_PN_Success)
        {
            PRINTF("%d mV\r\n", volt);
        }
        else
        {
            PRINTF("ERROR: Voltage Read Failed\r\n");
        }
    }

    GPADC_ExtPin_DeInit();
}

static void APP_GpadcReadSensor(uint16_t readTimes)
{
    uint16_t i;
    int32_t volt;
    pn_status_t pnStatus;

    PRINTF("\r\nTest the GPADC Driver by SensorRead (VBAT) Value\r\n");

    pnStatus = GPADC_Init();

    if (pnStatus != kStatus_PN_Success)
    {
        PRINTF("\r\n ERROR: GPADC Init Failed\r\n");
        return;
    }

    for (i = 0; i < readTimes; i++)
    {
        PRINTF("Read %d time: ", i);
        /* Reading the voltage value from GPADC Sensor VBAT. */
        pnStatus = GPADC_ReadSensor(kGPADC_SensorVBAT_ExtPin, kGPADC_TypeVal, &volt);

        if (pnStatus == kStatus_PN_Success)
        {
            PRINTF("%d mV\r\n", volt);
        }
        else
        {
            PRINTF("ERROR: Voltage Read Failed\r\n");
        }
    }

    GPADC_Deinit();
}

static void APP_GpadcReadSensorOneshot(uint16_t readTimes)
{
    uint16_t i;
    int32_t volt;
    pn_status_t pnStatus;

    PRINTF("\r\nTest the GPADC Driver by SensorReadOneShot (VBAT) Value\r\n");

    pnStatus = GPADC_Init();

    if (pnStatus != kStatus_PN_Success)
    {
        PRINTF("\r\n ERROR: GPADC Init Failed\r\n");
        return;
    }

    for (i = 0; i < readTimes; i++)
    {
        PRINTF("Read %d time: ", i);
        /* Reading the voltage value from GPADC Sensor VBAT. */
        pnStatus = GPADC_ReadSensorOneShot(kGPADC_SensorVBAT_ExtPin, kGPADC_TypeVal, &volt);

        if (pnStatus == kStatus_PN_Success)
        {
            PRINTF("%d mV\r\n", volt);
        }
        else
        {
            PRINTF("ERROR: Voltage Read Failed\r\n");
        }
    }

    GPADC_Deinit();
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t bOption;

    /* Init board hardware. */
    BOARD_InitHardware();

    while (1)
    {
        PRINTF(
            "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n"
            "GPADC Driver Example\r\n"
            "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");

        PRINTF(
            "                         Main Menu\r\n"
            "\t- Enter 0 for Testing the GPADC Driver by external pin voltage (Input must be within 0 V - 1.8 V)\r\n"
            "\t- Enter 1 for Testing the GPADC Driver by SensorRead Value\r\n"
            "\t- Enter 2 for Testing the GPADC Driver by SensorOneShot Value\r\n"
            "\r\n Select option :\r\n");

        if (1 != APP_WaitString(&bOption, 1))
        {
            /* Didn't received right valid data. */
            continue;
        }

        PRINTF("%c\r\n", bOption);

        switch (bOption)
        {
            case '0':
                APP_GpadcExtPinVoltage(APP_VOLT_READ_TIMES);
                break;

            case '1':
                APP_GpadcReadSensor(APP_VOLT_READ_TIMES);
                break;

            case '2':
                APP_GpadcReadSensorOneshot(APP_VOLT_READ_TIMES);
                break;

            default:
                PRINTF(
                    "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\r\n");
                PRINTF("Invalid Selection\r\n");
                PRINTF(
                    "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\r\n");
                break;
        }
    }
}
