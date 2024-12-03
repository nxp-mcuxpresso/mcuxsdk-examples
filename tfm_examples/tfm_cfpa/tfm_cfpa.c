/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "board.h"
#include "app.h"
#include "fsl_iap.h"
#include "fsl_iap_ffr.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define REGION(a, b, c) a##b##c
#define REGION_NAME(a, b, c) REGION(a, b, c)
#define REGION_DECLARE(a, b, c) extern uint32_t REGION_NAME(a, b, c)
#define BOOTLOADER_API_TREE_POINTER (0x1301fe00U)

REGION_DECLARE(Image$$, cfpa_data, $$RO$$Base);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

status_t get_cfpa_higher_version(flash_config_t *config, uint32_t *cfpaVersion, uint8_t *cfpaData);

typedef struct BootloaderTreeCut
{
    void (*runBootloader)(void *arg);      /*!< Function to start the bootloader executing. */
} bootloader_tree_cut_t;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */

int main(void)
{
    status_t status = 0;
    uint32_t i      = 0;
    uint32_t arg    = 0xEB120000; //0xEB:represents Enter Boot;
                                  //0x12: represents enter ISP mode by UART only
    bootloader_tree_cut_t *romApiTree = (bootloader_tree_cut_t *)BOOTLOADER_API_TREE_POINTER;

    static flash_config_t flashConfig;

    uint8_t currentCFPA[512]     = {0};
    uint32_t *currentVersionCFPA = (uint32_t *)&currentCFPA[4];

    uint8_t newCFPA[512]     = {0};
    uint32_t *newVersionCFPA = (uint32_t *)&newCFPA[4];

    uint8_t *storedCFPA = (uint8_t *)&REGION_NAME(Image$$, cfpa_data, $$RO$$Base);

    uint8_t contChar = ' ';

    /* Init board hardware. */
    BOARD_InitHardware();

		/* The CFPA table is written only after confirmation */
		PRINTF("**** The writting to the protected flash region is irreversible operation!!! ****\n"
		"**** Do you want to continue? (y/n) and press enter:\n");
		SCANF("%c", &contChar);

		if (contChar == 'y' || contChar == 'Y') {

			PRINTF("\n**** UPDATE CFPA TABLE STARTED ****\n\n");

			/* Data from flash must be stored to ram */
			for (i = 0; i < sizeof(newCFPA); i++)
					newCFPA[i] = *(storedCFPA + i);

			/* Init of the flash controller - it must be called before calling other flash APIs */
			status = FLASH_Init(&flashConfig);
			if (status != kStatus_Success)
			{
					PRINTF("[ERROR] FLASH_Init status = %x\n", status);
					return status;
			}

			/* Init of the FFR controller - it must be called before calling other FFR_APIs */
			status = FFR_Init(&flashConfig);
			if (status != kStatus_Success)
			{
					PRINTF("[ERROR] FFR_Init status = %x\n", status);
					return status;
			}

			/* Workaround: Resolving issue with CFPA ping/pong pages */
			status = get_cfpa_higher_version(&flashConfig, currentVersionCFPA, currentCFPA);
			if (status != kStatus_Success)
			{
					PRINTF("[ERROR] get_cfpa_higher_version status = %x\n", status);
					return status;
			}

			PRINTF("Current version: %x\n", *currentVersionCFPA);

			/* Comparison, if new version of CFPA is higher than current version of CFPA */
			if (*currentVersionCFPA >= *newVersionCFPA)
					*newVersionCFPA = *currentVersionCFPA + 1;

			PRINTF("Updated version: %x\n\n", *newVersionCFPA);

			/* Write the new CFPA with new version of CFPA to the FFR */
			status = FFR_InfieldPageWrite(&flashConfig, newCFPA, sizeof(newCFPA));
			if (status != kStatus_Success)
			{
					PRINTF("[ERROR] FFR_InfieldPageWrite status = %x\n", status);
					return status;
			}

			PRINTF("**** UPDATE CFPA TABLE FINISHED ****\n\n");
		}
		else if(contChar == 'n' || contChar == 'N') {
			PRINTF("\n**** UPDATE CFPA TABLE WAS CANCELED ****\n\n");
		}
		else {
			PRINTF("\n[ERROR] Chosen unsupported character (%c)\n\n", contChar);
		}

    /* Switch to ISP mode */
    romApiTree->runBootloader(&arg);

    while (1)
    {
    }
}

/********************************************************************************
 * Workaround: Resolving issue with CFPA ping/pong pages
 *******************************************************************************/
status_t get_cfpa_higher_version(flash_config_t *config, uint32_t *cfpaVersion, uint8_t *cfpaData)
{
    uint8_t pingData[512];
    uint8_t pongData[512];
    uint32_t readSize = 512;

    /*Get the CFPA ping page data and the corresponding cfpa version1*/
    config->ffrConfig.cfpaPageOffset = 1;
    status_t status                  = FFR_GetCustomerInfieldData(config, pingData, 0, readSize);
    if (status != kStatus_FLASH_Success)
    {
        return status;
    }

    uint32_t *temp1      = (uint32_t *)pingData;
    uint32_t versionPing = temp1[1];

    /*Get the CFPA pong page data and the corresponding cfpa version2*/
    config->ffrConfig.cfpaPageOffset = 2;
    status                           = FFR_GetCustomerInfieldData(config, (uint8_t *)pongData, 0, readSize);
    if (status != kStatus_FLASH_Success)
    {
        return status;
    }

    uint32_t *temp2      = (uint32_t *)pongData;
    uint32_t versionPong = temp2[1];

    /* Compare the cfpa ping version and pong version to return the higher version and the cfpa data*/
    if (versionPing > versionPong)
    {
        *cfpaVersion = versionPing;
        memcpy(cfpaData, pingData, sizeof(pingData));
        config->ffrConfig.cfpaPageVersion = versionPing;
        config->ffrConfig.cfpaPageOffset  = 1;
    }
    else
    {
        *cfpaVersion = versionPong;
        memcpy(cfpaData, pongData, sizeof(pingData));
        config->ffrConfig.cfpaPageVersion = versionPong;
        config->ffrConfig.cfpaPageOffset  = 2;
    }
    return kStatus_FLASH_Success;
}
