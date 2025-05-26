/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_smartdma.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool g_keyscan_complete_flag = false;
volatile uint32_t g_keyscan_interval = 1000U;
volatile uint32_t g_keyvalue[4] = {0U, 0U, 0U, 0U};
volatile uint8_t g_samrtdma_stack[32];
volatile uint32_t g_keyscan_gpio_register[8] = {
		0x40105000U + 0x60U + 28U,         /* ROW1, P3_28, Pin Data security Register */
		0x40105000U + 0x60U + 27U,         /* ROW2, P3_27, Pin Data security Register */
		0x40103000U + 0x60U + 5U,          /* ROW3, P1_5, Pin Data security Register */
		0x40103000U + 0x60U + 4U,          /* ROW4, P1_4, Pin Data security Register */
		0x40103000U + 0x60U + 1U,          /* COL1, P1_1, Pin Data security Register */
		0x40103000U + 0x60U + 2U,          /* COL2, P1_2, Pin Data security Register */
		0x40103000U + 0x60U + 0U,          /* COL3, P1_0, Pin Data security Register */
		0x40103000U + 0x60U + 6U,          /* COL4, P1_6, Pin Data security Register */
};
typedef struct {
    uint8_t colIndex;
    uint8_t rowIndex;
    const char* buttonName;
} buttonMap;

static const buttonMap g_button_map[] = {
    {0U, 0U, "Button 1"}, {0U, 1U, "Button 4"}, {0U, 2U, "Button 7"}, {0U, 3U, "Button 0"},
    {1U, 0U, "Button 2"}, {1U, 1U, "Button 5"}, {1U, 2U, "Button 8"}, {1U, 3U, "Button F"},
    {2U, 0U, "Button 3"}, {2U, 1U, "Button 6"}, {2U, 2U, "Button 9"}, {2U, 3U, "Button E"},
    {3U, 0U, "Button A"}, {3U, 1U, "Button B"}, {3U, 2U, "Button C"}, {3U, 3U, "Button D"},
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void Demo_SmartDmaKeyScan_CallBack(void *param)
{
    g_keyscan_complete_flag = true;
}

static void DEMO_InitSmartDma(void)
{
    static smartdma_keyscan_4x4_param_t smartdmaParam;
  
    SMARTDMA_InitWithoutFirmware();
    SMARTDMA_InstallFirmware(SMARTDMA_KEYSCAN_MEM_ADDR, s_smartdmaKeyscanFirmware, SMARTDMA_KEYSCAN_FIRMWARE_SIZE);
    SMARTDMA_InstallCallback(Demo_SmartDmaKeyScan_CallBack, NULL);

    NVIC_EnableIRQ(SMARTDMA_IRQn);
    NVIC_SetPriority(SMARTDMA_IRQn, 3);

    smartdmaParam.smartdma_stack        = (uint32_t*)g_samrtdma_stack;
    smartdmaParam.p_gpio_reg            = (uint32_t*)g_keyscan_gpio_register;
    smartdmaParam.p_keyvalue            = (uint32_t*)g_keyvalue;
    smartdmaParam.p_keycan_interval     = (uint32_t*)&g_keyscan_interval;
    SMARTDMA_Boot(kSMARTDMA_Keyscan_4x4, &smartdmaParam, 0x2U);
}

int main(void)
{
    BOARD_InitHardware();
    DEMO_InitSmartDma();

    PRINTF("\r\nSmartDma Keyscan Example.");

    while (1)
    {
        while (!g_keyscan_complete_flag)
        {
        }
        g_keyscan_complete_flag = false;
        
        for (uint8_t index = 0; index < (sizeof(g_button_map)/sizeof(*g_button_map)); ++index)
        {
            const buttonMap* btn = &g_button_map[index];
            if (g_keyvalue[btn->colIndex] & (1U << btn->rowIndex))
            {
                PRINTF("\r\n%s is pressed", btn->buttonName);
            }
        }
    }
}
