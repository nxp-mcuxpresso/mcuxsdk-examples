/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_lpi2c.h"
#include "fsl_pf3000.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static bool I2C_SendFunc(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *txBuff, uint8_t txBuffSize);
static bool I2C_ReceiveFunc(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static pf3000_handle_t pf3000Handle;
static lpi2c_master_handle_t lpi2cHandle;
static volatile bool pf3000IntFlag;
static volatile bool lpi2cIntFlag;
static uint8_t buffer[256U];

/*******************************************************************************
 * Code
 ******************************************************************************/

/*! @brief PF3000 INTB Pad ISR function */
void DEMO_PF3000_INTB_HANDLER(void)
{
    /* Disable level-triggered PF3000 INTB Pad Interrupt here.
       The interrupt will be enabled once current events have
       been solved in the main loop. */
    PORT_SetPinInterruptConfig(DEMO_PF3000_INTB_PORT, DEMO_PF3000_INTB_PIN, kPORT_InterruptOrDMADisabled);
    /* Inform main loop PF3000 INTB Pad Interrupt is asserted. */
    pf3000IntFlag = true;
    SDK_ISR_EXIT_BARRIER;
}

static void lpi2c_master_callback(LPI2C_Type *base, lpi2c_master_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        lpi2cIntFlag = true;
    }
}

static uint32_t DEMO_GetDemoToRun(void)
{
    uint8_t demoSel;

    PRINTF("\r\n-------------- PF3000 on board PMIC driver example --------------\r\n");
    PRINTF("Please select the PMIC example you want to run:\r\n");
    PRINTF("[1]. Setting Regulator Output Voltage\r\n");
    PRINTF("[2]. Dumping Regulator Output Voltage\r\n");
    PRINTF("[3]. Dumping Selected Register Content\r\n");

    while (1)
    {
        demoSel = GETCHAR();
        if ((demoSel >= '1') && (demoSel <= '4'))
        {
            break;
        }
    }

    return demoSel;
}

static uint32_t DEMO_GetInputNumWithEcho(uint32_t length, bool allowZero)
{
    uint8_t ch;
    uint8_t digBuffer[10U];
    uint8_t i, j;
    uint8_t digCount = 0U;
    uint32_t temp1, temp2;
    uint32_t result  = 0U;
    bool getFirstDig = false;

    assert(length <= (sizeof(digBuffer) / sizeof(digBuffer[0U])));

    /* Get user input and echo it back to terminal. */
    for (;;)
    {
        ch = GETCHAR();
        if ((('0' <= ch) && ('9' >= ch)) && (digCount < length))
        {
            if (false == getFirstDig)
            {
                if (allowZero || (('0' < ch) && ('9' >= ch)))
                {
                    getFirstDig = true;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                if ('0' == digBuffer[0U])
                {
                    continue;
                }
            }
            PUTCHAR(ch);
            digBuffer[digCount] = ch;
            digCount++;
        }

        if ('\r' == ch)
        {
            break;
        }
    }

    /* Reconstruct user input number. */
    temp1 = digCount - 1;
    for (i = 0; i < digCount; i++)
    {
        temp2 = digBuffer[i] - '0';
        for (j = 0U; j < temp1; j++)
        {
            temp2 *= 10U;
        }
        temp1--;
        result += temp2;
    }

    return result;
}

static void DEMO_SetRegulatorOutput(void)
{
    uint8_t regulatorSel;
    uint32_t outputVoltage = 0;
    pf3000_module_t module;

    for (;;)
    {
        PRINTF("\f");
        PRINTF("\r\n-------------- Setting Regulator Output Voltage  --------------\r\n");
        PRINTF("Please select which regulator to set:\r\n");
        PRINTF("[a]. Buck Switch 1A      (  700mV ~ 1425mV, 1800mV, 3300mV )\r\n");
        PRINTF("[b]. Buck Switch 1B      (  700mV ~ 1425mV )\r\n");
        PRINTF("[c]. Buck Switch 2       ( %s )\r\n",
               (pf3000Handle.switch2Range) ? ("2500mV ~ 3300mV") : ("1500mV ~ 1850mV"));
        PRINTF("[d]. Buck Switch 3       (  900mV ~ 1650mV )\r\n");
        PRINTF("[e]. Buck Switch Boost   ( 5000mV ~ 5150mV )\r\n");
        PRINTF("[f]. LDO 1               ( 1800mV ~ 3300mV )\r\n");
        PRINTF("[g]. LDO 2               (  800mV ~ 1550mV )\r\n");
        PRINTF("[h]. LDO 3               ( 1800mV ~ 3300mV )\r\n");
        PRINTF("[i]. LDO 4               ( 1800mV ~ 3300mV )\r\n");
        PRINTF("[j]. VCC_SD              ( %s )\r\n",
               (pf3000Handle.vccsdVsel) ? ("1800mV ~ 1850mV") : ("2850mV ~ 3300mV"));
        PRINTF("[k]. V33                 ( 2850mV ~ 3300mV )\r\n");
        PRINTF("[l]. Back to Home Page\r\n");

        for (;;)
        {
            regulatorSel = GETCHAR();
            if ((regulatorSel >= 'a') && (regulatorSel <= 'l'))
            {
                break;
            }
        }

        if ('l' != regulatorSel)
        {
            PRINTF("\f");
            PRINTF("Please input the regulator output(mV):");
            outputVoltage = DEMO_GetInputNumWithEcho(4U, false);
            PRINTF("\r\n");

            PRINTF("User desired regulator output:%dmV\r\n", outputVoltage);
            PRINTF("Please confirm the desired output will not damage to the hardware!!!\r\n");
            PRINTF("Press any key to update regulator output...\r\n");
            GETCHAR();

            /* Convert user input unit to uV. */
            outputVoltage *= 1000U;

            module = (pf3000_module_t)(kPF3000_ModuleSwitch1A + regulatorSel - 'a');
            PF3000_SetRegulatorOutputVoltage(&pf3000Handle, module, kPF3000_OperatingStatusSystemOn, outputVoltage);
            outputVoltage = PF3000_GetRegulatorOutputVoltage(&pf3000Handle, module, kPF3000_OperatingStatusSystemOn);

            /* Convert actual output unit to mV. */
            outputVoltage /= 1000U;
            PRINTF("Actual Regulator Output:%dmV\r\n", outputVoltage);
            PRINTF("Press Any Key to Continue...\r\n");
            GETCHAR();
            PRINTF("\f");
        }
        else
        {
            break;
        }
    }
}

static void DEMO_PrintRegulatorInfo(const char *regulatorName, pf3000_module_t module)
{
    PRINTF("[Name:%s]", regulatorName);

    PRINTF("[Enable:%s][Voltage:%dmV]\r\n", PF3000_IsRegulatorEnabled(&pf3000Handle, module) ? "Yes " : "No  ",
           PF3000_GetRegulatorOutputVoltage(&pf3000Handle, module, kPF3000_OperatingStatusSystemOn) / 1000U);
}

static void DEMO_DumpRegulatorOutput(void)
{
    /* Scroll to a new page. */
    PRINTF("\f");
    PRINTF("\r\n-------------- Dumping PF3000 Regulator Output --------------\r\n");
    PRINTF("All the Regulator Output Parameters are listed here:\r\r\n\n");

    /* Print All Regulator Output Voltages. */
    DEMO_PrintRegulatorInfo("   Switch1A   ", kPF3000_ModuleSwitch1A);
    DEMO_PrintRegulatorInfo("   Switch1B   ", kPF3000_ModuleSwitch1B);
    DEMO_PrintRegulatorInfo("   Switch2    ", kPF3000_ModuleSwitch2);
    DEMO_PrintRegulatorInfo("   Switch3    ", kPF3000_ModuleSwitch3);
    DEMO_PrintRegulatorInfo(" Switch Boost ", kPF3000_ModuleSwitchBoost);
    DEMO_PrintRegulatorInfo("    LDO1      ", kPF3000_ModuleLdo1);
    DEMO_PrintRegulatorInfo("    LDO2      ", kPF3000_ModuleLdo2);
    DEMO_PrintRegulatorInfo("    LDO3      ", kPF3000_ModuleLdo3);
    DEMO_PrintRegulatorInfo("    LDO4      ", kPF3000_ModuleLdo4);
    DEMO_PrintRegulatorInfo("   VCC_SD     ", kPF3000_ModuleVcc_sd);
    DEMO_PrintRegulatorInfo("    V33       ", kPF3000_ModuleV33);
    DEMO_PrintRegulatorInfo("   Vsnvs      ", kPF3000_ModuleVsnvs);
    PRINTF("\r\n");
}

static void DEMO_DumpRegisterContent(void)
{
    uint8_t page;
    uint8_t addressStart;
    uint8_t addressEnd;

    uint32_t address;
    uint8_t number;
    uint8_t i;

    for (;;)
    {
        /* Scroll to a new page. */
        PRINTF("\f");
        PRINTF("\r\n-------------- Dumping PF3000 Register Content --------------\r\n");
        PRINTF("Please select the page to dump (0, 1, 2):");
        page = DEMO_GetInputNumWithEcho(1U, true);
        if (2U >= page)
        {
            break;
        }
    }
    PRINTF("\r\n");

    switch (page)
    {
        /* Page 0 */
        case 0U:
            addressStart = 0x00U;
            addressEnd   = 0x7FU;
            break;

        /* Page 1 */
        case 1U:
            addressStart = 0x80U;
            addressEnd   = 0xFFU;
            break;

        /* Page 2 */
        case 2U:
            addressStart = 0x81U;
            addressEnd   = 0xF5U;
            break;
        default:
            addressStart = 0x00U;
            addressEnd   = 0x00U;
            break;
    }

    for (;;)
    {
        /* Scroll to a new page. */
        PRINTF("\f");
        PRINTF("Please select the first address to dump (%d ~ %d):", addressStart, addressEnd);
        address = DEMO_GetInputNumWithEcho(3U, true);
        if ((addressEnd >= address) && (addressStart <= address))
        {
            break;
        }
    }
    PRINTF("\r\n");

    for (;;)
    {
        /* Scroll to a new page. */
        PRINTF("\f");
        PRINTF("Please select the number of registers to dump(1~%d):", (addressEnd - address + 1U));
        number = DEMO_GetInputNumWithEcho(3U, false);
        if (((addressEnd - address + 1U) >= number) && (0U < number))
        {
            break;
        }
    }
    PRINTF("\r\n");

    /* Read register from PF3000 */
    PF3000_DumpReg(&pf3000Handle, page, address, buffer, number);

    PRINTF("\r\nDump Format: [Register Address] = Register Content:\r\n");
    for (i = 0; i < number; i++)
    {
        PRINTF("[0x%x] = 0x%x\r\n", address + i, buffer[i]);
    }
    PRINTF("\r\n");
}

/*! @brief Main function */
int main(void)
{
    gpio_pin_config_t gpioPinConfig;
    lpi2c_master_config_t masterConfig;
    pf3000_config_t pf3000Config;
    uint32_t demoSelection;

    /* Init board hardware. */
    BOARD_InitHardware();

    /* Init GPIO Interrupt used by PMIC. */
    gpioPinConfig.pinDirection = kGPIO_DigitalInput;
    gpioPinConfig.outputLogic  = 0x0U;
    GPIO_PinInit(DEMO_PF3000_INTB_GPIO, DEMO_PF3000_INTB_PIN, &gpioPinConfig);

    /* Init LPI2C used by PF3000. */
    /*
     * masterConfig.debugEnable = false;
     * masterConfig.ignoreAck = false;
     * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * masterConfig.baudRate_Hz = 100000U;
     * masterConfig.busIdleTimeout_ns = 0;
     * masterConfig.pinLowTimeout_ns = 0;
     * masterConfig.sdaGlitchFilterWidth_ns = 0;
     * masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = DEMO_PF3000_LPI2C_BAUDRATE;
    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(DEMO_PF3000_LPI2C, &masterConfig, DEMO_PF3000_LPI2C_CLKSRC_FREQ);
    /* Create the LPI2C handle for the non-blocking transfer */
    LPI2C_MasterTransferCreateHandle(DEMO_PF3000_LPI2C, &lpi2cHandle, lpi2c_master_callback, NULL);

    /* Init PF3000 Component. */
    PF3000_GetDefaultConfig(&pf3000Config);
    pf3000Config.I2C_SendFunc    = I2C_SendFunc;
    pf3000Config.I2C_ReceiveFunc = I2C_ReceiveFunc;
    PF3000_Init(&pf3000Handle, &pf3000Config);

    /* Print the initial banner. */
    for (;;)
    {
        demoSelection = DEMO_GetDemoToRun();

        switch (demoSelection)
        {
            /* Setting Regulator Output Voltage: */
            case '1':
                DEMO_SetRegulatorOutput();
                break;

            /* Dumping all Regulator Output Voltage: */
            case '2':
                DEMO_DumpRegulatorOutput();
                PRINTF("Press Any Key to Home Page...\r\n");
                GETCHAR();
                break;

            /* Dumping Selected Register Content: */
            case '3':
                DEMO_DumpRegisterContent();
                PRINTF("Press Any Key to Home Page...\r\n");
                GETCHAR();
                break;

            default:
                break;
        }

        /* Scroll to a new page. */
        PRINTF("\f");
    }
}

static bool I2C_SendFunc(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *txBuff, uint8_t txBuffSize)
{
    status_t reVal = kStatus_Fail;
    lpi2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kLPI2C_Write;
    masterXfer.subaddress     = subAddress;
    masterXfer.subaddressSize = subAddressSize;
    masterXfer.data           = txBuff;
    masterXfer.dataSize       = txBuffSize;
    masterXfer.flags          = kLPI2C_TransferDefaultFlag;

    lpi2cIntFlag = false;
    reVal        = LPI2C_MasterTransferNonBlocking(DEMO_PF3000_LPI2C, &lpi2cHandle, &masterXfer);
    if (kStatus_Success != reVal)
    {
        return false;
    }

    while (false == lpi2cIntFlag)
    {
    }

    return true;
}

static bool I2C_ReceiveFunc(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    status_t reVal = kStatus_Fail;
    lpi2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kLPI2C_Read;
    masterXfer.subaddress     = subAddress;
    masterXfer.subaddressSize = subAddressSize;
    masterXfer.data           = rxBuff;
    masterXfer.dataSize       = rxBuffSize;
    masterXfer.flags          = kLPI2C_TransferDefaultFlag;

    lpi2cIntFlag = false;
    reVal        = LPI2C_MasterTransferNonBlocking(DEMO_PF3000_LPI2C, &lpi2cHandle, &masterXfer);
    if (kStatus_Success != reVal)
    {
        return false;
    }

    while (false == lpi2cIntFlag)
    {
    }

    return true;
}
