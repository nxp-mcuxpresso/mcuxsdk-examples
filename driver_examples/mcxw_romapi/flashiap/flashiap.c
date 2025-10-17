/*
 * Copyright 2018, 2023-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_iap.h"
#include "fsl_rom_api.h"
#include "fsl_clock.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* For this example, the flash operations should be performed on the last Flash sector */
#ifndef FLASH_TEST_SECTOR_INDEX_FROM_END
#define FLASH_TEST_SECTOR_INDEX_FROM_END                                                                             \
    1U /* FLASH_TEST_SECTOR_INDEX_FROM_END = 1 : Last Sector , FLASH_TEST_SECTOR_INDEX_FROM_END = 2 : Last Sector -1 \
        */
#endif /* FLASH_TEST_SECTOR_INDEX_FROM_END */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Make sure these prototypes are placed in RAM.
 * Otherwise, symbols will be placed in the flash causing the application to fail*/
#if defined(__CC_ARM) || defined(__ARMCC_VERSION) || defined(__GNUC__)
void SysTick_Handler(void) __attribute__((section(".ramfunc")));
void GpioPinToggle(void) __attribute__((section(".ramfunc")));
void GpioPinClear(void) __attribute__((section(".ramfunc")));
#elif defined(__ICCARM__)
__ramfunc void SysTick_Handler(void);
__ramfunc void GpioPinToggle(void);
__ramfunc void GpioPinClear(void);
#else
#error Unsupported toolchain!
#endif //(__CC_ARM) || (__ARMCC_VERSION)

/*******************************************************************************
 * Variables
 ******************************************************************************/
typedef struct
{
    const char *descr;
    uint8_t flashPropertyTag;
} propertyItem_t;

static const propertyItem_t s_propertyItems[] = {
    {.descr = "PflashTotalSize", .flashPropertyTag = kFLASH_PropertyPflashTotalSize},
    {.descr = "PflashSectorSize", .flashPropertyTag = kFLASH_PropertyPflashSectorSize},
    {.descr = "PflashPageSize", .flashPropertyTag = kFLASH_PropertyPflashPageSize},
    {.descr = "PflashBlockSize", .flashPropertyTag = kFLASH_PropertyPflashBlockSize},
    {.descr = "PflashBlockCount", .flashPropertyTag = kFLASH_PropertyPflashBlockCount},
    {.descr = "PflashBlockBaseAddr", .flashPropertyTag = kFLASH_PropertyPflashBlockBaseAddr},
    {.descr = "PflashSystemFreqMHz", .flashPropertyTag = kFLASH_PropertyPflashSystemFreq}};

static uint8_t s_testBuffer[FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES] __ALIGNED(sizeof(uint32_t));
static uint8_t s_readBuffer[FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES] __ALIGNED(sizeof(uint32_t));

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Handles SysTick interrupts .
 */
void SysTick_Handler(void)
{
    GpioPinToggle();
}

/*!
 * @brief Reverses current output level of BOARD_LED_GPIO.
 */
void GpioPinToggle(void)
{
    BOARD_LED_GPIO->NOT[BOARD_LED_GPIO_PORT] = 1U << BOARD_LED_GPIO_PIN;
}
/*!
 * @brief Sets the output level of BOARD_LED_GPIO to the logic 0.
 */
void GpioPinClear(void)
{
    BOARD_LED_GPIO->CLR[BOARD_LED_GPIO_PORT] = 1U << BOARD_LED_GPIO_PIN;
}
/*!
 * @brief Retrieves and prints the flash properties.
 *
 * @param flashInstance Pointer to the flash configuration.
 *
 * @retval status  Indicates if API was executed successfully or not.
 *
 */

static status_t FlashGetProperties(flash_config_t *flashInstance)
{
    static uint32_t status;
    uint32_t flashPropertyValue = 0;
    int flashPropertiesCount    = sizeof(s_propertyItems) / sizeof(s_propertyItems[0]);

    for (int i = 0; i < flashPropertiesCount; i++)
    {
        /* Get flash properties */
        status = FLASH_GetProperty(flashInstance, (flash_property_tag_t)s_propertyItems[i].flashPropertyTag, &flashPropertyValue);
        if (status != kStatus_Success)
        {
            PRINTF("FLASH_GetProperty %s failed !\r\n", s_propertyItems[i].descr);
        }
        /* Print flash Properties. */
        PRINTF("\t%s = %ld\n", s_propertyItems[i].descr, flashPropertyValue);
    }
    return status;
}
/*!
 * @brief Demonstrates a typical Erase/Write scenario: Erase/Verify Erase/Program/Verify Program.
 *
 * @param flashInstance Pointer to the flash configuration.
 * @param destAddr      The start address of the target flash region.
 *
 * @retval status  Indicates if API was executed successfully or not.
 *
 */
static status_t FlashEraseWriteScenario(flash_config_t *flashInstance, uint32_t destAddr)
{
    static uint32_t status;
    uint32_t failedAddress = 0;
    uint32_t primask;
    uint32_t failedData = 0;

    /* Erase the Flash region */
    /* Disable the global interrupt and return the current primask register */
    primask = DisableGlobalIRQ();
    /* Flash Erase has to be done at sector level */
    status = FLASH_Erase(flashInstance, destAddr, flashInstance->flashDesc.sectorSize, kFLASH_ApiEraseKey);
    /*!
     * Set the primask register with the provided primask value, not just enable the primask.
     * The primask value is supposed to be provided by DisableGlobalIRQ() since it is required to
     * use the EnableGlobalIRQ() and DisableGlobalIRQ() in pair.
     */
    EnableGlobalIRQ(primask);
    if (status != kStatus_Success)
    {
        PRINTF("FLASH_Erase failed !\r\n");
    }
    /* Verify if the given flash region is successfully erased. */
    status = FLASH_VerifyErase(flashInstance, destAddr, flashInstance->flashDesc.sectorSize);
    if (status != kStatus_Success)
    {
        PRINTF("FLASH_VerifyErase failed !\r\n");
    }
    /* Start programming specified flash region */
    /* Disable the global interrupt and return the current primask register */
    primask = DisableGlobalIRQ();
    /* The smallest amount that must be written at once is a phrase. */
    status = FLASH_Program(flashInstance, destAddr, s_testBuffer, sizeof(s_testBuffer));
    /*!
     * Set the primask register with the provided primask value, but not just enable the primask
     * The primask value is supposed to be provided by DisableGlobalIRQ() since it is required to
     * use the EnableGlobalIRQ() and DisableGlobalIRQ() in pair
     */
    EnableGlobalIRQ(primask);
    if (status != kStatus_Success)
    {
        PRINTF("FLASH_Program failed !\r\n");
    }

    /* Verify if the given flash region is successfully programmed with given data */
    status =
        FLASH_VerifyProgram(flashInstance, destAddr, sizeof(s_testBuffer), s_testBuffer, &failedAddress, &failedData);
    if (status != kStatus_Success)
    {
        PRINTF("FLASH_VerifyProgram failed @0x%lX : Read %lX !\r\n", failedAddress, failedData);
    }
    return status;
}
/*!
 * @brief Demonstrates a typical Read/Modify/Write scenario: Read/Modify/Erase/Program.
 *
 * @param flashInstance Pointer to the flash configuration.
 * @param destAddr      The start address of the target flash region.
 *
 * @retval status  Indicates if API was executed successfully or not.
 *
 */
static status_t FlashReadModifyWriteScenario(flash_config_t *flashInstance, uint32_t destAddr)
{
    static uint32_t status;
    int compareResult;

    /* Flash read can be done at byte level  */
    status = FLASH_Read(flashInstance, destAddr, s_testBuffer, sizeof(s_testBuffer));
    if (status != kStatus_Success)
    {
        PRINTF("FLASH_Read @%lX failed !\r\n", destAddr);
    }
    /* Modify the first 4 bytes */
    uint32_t *data = (uint32_t *)s_testBuffer;
    (*data)++;
    /* Flash Erase has to be done at sector level */
    status = FLASH_Erase(flashInstance, destAddr, flashInstance->flashDesc.sectorSize, kFLASH_ApiEraseKey);
    if (status != kStatus_Success)
    {
        PRINTF("FLASH_Erase failed !\r\n");
    }
    /* Program the specified flash region with the modified buffer.*/
    /* The smallest amount that must be written at once is a phrase. */
    status = FLASH_Program(flashInstance, destAddr, s_testBuffer, sizeof(s_testBuffer));
    if (status != kStatus_Success)
    {
        PRINTF("FLASH_Program failed !\r\n");
    }
    /* Flash read can be done at byte level */
    status = FLASH_Read(flashInstance, destAddr, s_readBuffer, sizeof(s_readBuffer));
    if (status != kStatus_Success)
    {
        PRINTF("FLASH_Read @%lX after modification failed !\r\n", destAddr);
    }
    /* Verify reading the modified value */
    compareResult = memcmp(s_testBuffer, ((uint8_t *)&s_readBuffer), sizeof(s_testBuffer));
    if (compareResult != 0)
    {
        PRINTF("FLASH_Program after modification failed @%lX !\r\n", destAddr);
    }
    /* Flash is memory mapped, so we can compare the read buffer with memory mapped flash address. */
    compareResult = memcmp(s_readBuffer, ((uint8_t *)destAddr), sizeof(s_readBuffer));
    if (compareResult != 0)
    {
        PRINTF("Flash Read comparaison failed @%lX !\r\n", destAddr);
    }
    return status;
}
/*!
 * @brief Demonstrates how to execute interrupts from RAM during flash write/erase.
 *
 * @param flashInstance Pointer to the flash configuration.
 * @param destAddr      The start address of the target flash region.
 *
 * @retval status       Indicates if API was executed successfully or not.
 */
static status_t FlashEraseProgramDuringRAMInterrupt(flash_config_t *flashInstance, uint32_t destAddr)
{
    static uint32_t status;
    uint32_t counter = 1;

    /* Disable all the interrupts except for systick */
    for (int i = NonMaskableInt_IRQn; i < WAKE_PAD_IRQn + 1; i++)
    {
        if (i != SysTick_IRQn)
        {
            DisableIRQ((IRQn_Type)i);
        }
    }
    /* Make sure interrupt vector table is copied to RAM. This can be done by doing a call to InstallIRQHandler,
     * while ENABLE_RAM_VECTOR_TABLE symbol is defined.
     */
    InstallIRQHandler(SysTick_IRQn, (uint32_t)SysTick_Handler);
    /* Set systick reload value to generate 1ms interrupt and toggle a LED */
    SysTick_Config(SystemCoreClock / 1000U);
    if (status != kStatus_Success)
    {
        PRINTF("FLASH_Program failed !\r\n");
    }
    /* Make a loop that does 10 flash erase and program, while the systick interrupt is toggling a LED. */
    while (counter < 11)
    {
        /* Call flash erase and program while systick interrupts are triggered, to check if flash IAP driver is
           re-entrant. Note that the interrupts that are fired while executing flash IAP functions MUST be executed from
           RAM. Flash controller cannot execute interrupts from flash while handling a flash operation
        */
        status = FLASH_Erase(flashInstance, destAddr, flashInstance->flashDesc.sectorSize, kFLASH_ApiEraseKey);
        if (status != kStatus_Success)
        {
            PRINTF("FLASH_Erase failed !\r\n");
        }
        /* The smallest amount that must be written at once is a phrase*/
        status = FLASH_Program(flashInstance, destAddr, s_testBuffer, sizeof(s_testBuffer));
        if (status != kStatus_Success)
        {
            PRINTF("FLASH_Program failed !\r\n");
        }
        counter++;
    }
    /* Disable Systicks for tickless mode and stop toggling the LED*/
    SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
    GpioPinClear();
    return status;
}
/*!
 * @brief Demonstrates the relationship between main clock frequency and flash number of wait states.
 *
 * @param flashInstance Pointer to the flash configuration.
 * @param destAddr      The start address of the desired flash region to be erased.
 *
 * @retval status       Indicates if API was executed successfully or not.
 */
static status_t FlashUpdateWaitStatesAccordingToMainClk(flash_config_t *flashInstance, uint32_t destAddr)
{
    static uint32_t status;
    uint32_t curSystemCoreClock = 0;

    /* Get the current main Clk frequency */
    curSystemCoreClock = CLOCK_GetCoreSysClkFreq();

    /* Check if the clock speed has been increased to 32MHz */
    if (curSystemCoreClock < kFreq_32MHz) /* the clock speed is below 32MHz */
    {
        /* Going from current freq (lower freq) to 32MHz (higher freq)  */
        PRINTF("1.Going from current main clk (lower freq) to 32MHz (higher freq)\r\n");
        /* 1. Update the flash wait states  */
        /* Starting from 16MHz or lower frequency, an extra flash wait state is needed*/
        CLOCK_SetFLASHAccessCyclesForFreq(kFreq_32MHz);
        /* 2. Configure the main clk frequency  */
        CLOCK_SetupFROClocking(kFreq_32MHz);
        /* Flash Erase has to be done at sector level */
        status = FLASH_Erase(flashInstance, destAddr, flashInstance->flashDesc.sectorSize, kFLASH_ApiEraseKey);
        if (status != kStatus_Success)
        {
            PRINTF("FLASH_Erase failed !\r\n");
        }
        /* The smallest amount that must be written at once is a phrase*/
        status = FLASH_Program(flashInstance, destAddr, s_testBuffer, sizeof(s_testBuffer));
        if (status != kStatus_Success)
        {
            PRINTF("FLASH_Program failed !\r\n");
        }
        /* Going from 32MHz (high freq) to 12MHz (low freq)   */
        PRINTF("2.Going from 32MHz (high freq) to 12MHz (low freq)\r\n");
        /* 1. Configure the main clk frequency  */
        CLOCK_SetupFROClocking(kFreq_12MHz);
        /* 2. Update the flash wait states  */
        /* When moving back from 32MHz to 16MHz or lower frequency, no extra wait state is needed*/
        CLOCK_SetFLASHAccessCyclesForFreq(kFreq_12MHz);
        /* Flash Erase has to be done at sector level */
        status = FLASH_Erase(flashInstance, destAddr, flashInstance->flashDesc.sectorSize, kFLASH_ApiEraseKey);
        if (status != kStatus_Success)
        {
            PRINTF("FLASH_Erase failed !\r\n");
        }
        /* The smallest amount that must be written at once is a phrase*/
        status = FLASH_Program(flashInstance, destAddr, s_testBuffer, sizeof(s_testBuffer));
        if (status != kStatus_Success)
        {
            PRINTF("FLASH_Program failed !\r\n");
        }
    }
    else /* the main clk frequency is 32MHz, since 32MHz is the maximum value */
    {
        /* Going from 32MHz (high freq) to 12MHz (low freq)   */
        PRINTF("1.Going from 32MHz (high freq) to 12MHz (low freq)\r\n");
        /* 1. Configure the main clk frequency  */
        CLOCK_SetupFROClocking(kFreq_12MHz);
        /* 2. Update the flash wait states  */
        /* When moving back from 32MHz to 16MHz or lower frequency, no extra wait state is needed*/
        CLOCK_SetFLASHAccessCyclesForFreq(kFreq_12MHz);
        /* Flash Erase has to be done at sector level  */
        status = FLASH_Erase(flashInstance, destAddr, flashInstance->flashDesc.sectorSize, kFLASH_ApiEraseKey);
        if (status != kStatus_Success)
        {
            PRINTF("FLASH_Erase failed !\r\n");
        }
        /* The smallest amount that must be written at once is a phrase*/
        status = FLASH_Program(flashInstance, destAddr, s_testBuffer, sizeof(s_testBuffer));
        if (status != kStatus_Success)
        {
            PRINTF("FLASH_Program failed !\r\n");
        }

        /* Going from 12MHz (low freq) to 32MHz (high freq)   */
        PRINTF("2.Going from 12MHz (low freq) to 32MHz (high freq)\r\n");
        /* 1. Update the flash wait states  */
        /* Starting from 16MHz or lower frequency, an extra flash wait state is needed*/
        CLOCK_SetFLASHAccessCyclesForFreq(kFreq_32MHz);
        /* 2. Configure the main clk frequency  */
        CLOCK_SetupFROClocking(kFreq_32MHz);
        /* Flash Erase has to be done at sector level  */
        status = FLASH_Erase(flashInstance, destAddr, flashInstance->flashDesc.sectorSize, kFLASH_ApiEraseKey);
        if (status != kStatus_Success)
        {
            PRINTF("FLASH_Erase failed !\r\n");
        }
        /* The smallest amount that must be written at once is a phrase */
        status = FLASH_Program(flashInstance, destAddr, s_testBuffer, sizeof(s_testBuffer));
        if (status != kStatus_Success)
        {
            PRINTF("FLASH_Program failed !\r\n");
        }

        /* Switch back from 32MHz (high freq) to 12MHz (low freq), to continue with 12MHz main clock speed    */
        PRINTF("3.Going from 32MHz (high freq) to 12MHz (low freq)\r\n");
        /* 1. Configure the main clk frequency  */
        CLOCK_SetupFROClocking(kFreq_12MHz);
        /* 2. Update the flash wait states  */
        /* When moving back from 32MHz to 16MHz or lower frequency, no extra wait state is needed*/
        CLOCK_SetFLASHAccessCyclesForFreq(kFreq_12MHz);
        /* Flash Erase has to be done at sector level  */
        status = FLASH_Erase(flashInstance, destAddr, flashInstance->flashDesc.sectorSize, kFLASH_ApiEraseKey);
        if (status != kStatus_Success)
        {
            PRINTF("FLASH_Erase failed !\r\n");
        }
        /* The smallest amount that must be written at once is a phrase */
        status = FLASH_Program(flashInstance, destAddr, s_testBuffer, sizeof(s_testBuffer));
        if (status != kStatus_Success)
        {
            PRINTF("FLASH_Program failed !\r\n");
        }
    }

    return status;
}
/*!
 * @brief Erases a flash region.
 *
 * @param flashInstance Pointer to the flash configuration.
 * @param destAddr      The start address of the desired flash region to be erased.
 * @attention           Flash Erase has to be done at sector level.
 *
 */
void EraseFlashTestRegion(flash_config_t *flashInstance, uint32_t destAddr)
{
    static uint32_t status;
    /* Flash Erase has to be done at sector level */
    status = FLASH_Erase(flashInstance, destAddr,
                         (FLASH_TEST_SECTOR_INDEX_FROM_END * flashInstance->flashDesc.sectorSize), kFLASH_ApiEraseKey);
    if (status != kStatus_Success)
    {
        PRINTF("EraseFlashTestRegion failed !\r\n");
    }
}
/*!
 * @brief Main function.
 */
int main()
{
    flash_config_t flashInstance;
    uint32_t destAddr; /* Address of the target flash region */

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    /* Print basic information for Flash Driver API Demo.*/
    PRINTF(" \t----- Flash driver IAP Demo Application -----\r\n");
    /* 1.Initialize flash driver */
    PRINTF("Initializing Flash driver...\r\n");
    if (FLASH_Init(&flashInstance) == kStatus_Success)
    {
        PRINTF("Flash Init done with success !\r\n");
    }
    else
    {
        PRINTF("Flash Init failed ! \r\n");
    }

    /* kCLOCK_Sysctl must be enabled for FLASH_CacheClear, FLASH_CacheSpeculationControl and FLASH_CheckECC to have an
     * effect. */
    CLOCK_EnableClock(kCLOCK_Sysctl);

    /* 2.Flash cache cleared to make sure no content is left in from other applications*/
    FLASH_CacheClear();
    /* 3.Get flash properties */
    if (FlashGetProperties(&flashInstance) == kStatus_Success)
    {
        PRINTF("Flash Properties printed with success !\r\n");
    }
    else
    {
        PRINTF("FlashGetProperties failed !\r\n");
    }
    /* Set the address of the target flash region */
    destAddr =
        (flashInstance.flashDesc.blockBase) +
        (flashInstance.flashDesc.totalSize - (FLASH_TEST_SECTOR_INDEX_FROM_END * flashInstance.flashDesc.sectorSize));
    PRINTF("Working on Flash Address : 0x%lX \r\n", destAddr);
    /* 4.Enable speculative loading of the cache, to reduce the amount of traffic. */
    FLASH_CacheSpeculationControl(true);
    /* 5.Flash Typical "Erase/Write" Scenario */
    if (FlashEraseWriteScenario(&flashInstance, destAddr) == kStatus_Success)
    {
        PRINTF("Finished 'Erase/Write' Scenario with success !\r\n");
    }
    else
    {
        PRINTF("'Erase/Write' Scenario failed !\r\n");
    }

    /* 6.Flash Typical "Read/Modify/Write" Scenario */
    if (FlashReadModifyWriteScenario(&flashInstance, destAddr) == kStatus_Success)
    {
        PRINTF("Finished 'Read/Modify/Write' Scenario with success !\r\n");
    }
    else
    {
        PRINTF("'Read/Modify/Write' Scenario failed !\r\n");
    }

    /* Resume flash region status */
    EraseFlashTestRegion(&flashInstance, destAddr);
    /* 7. Execute interrupts from RAM during flash write/erase   */
    PRINTF("Executing interrupts from RAM during flash operations: USER LED should start blinking ..\r\n");
    if (FlashEraseProgramDuringRAMInterrupt(&flashInstance, destAddr) == kStatus_Success)
    {
        PRINTF("Finished FlashEraseProgramDuringRAMInterrupt with success !\r\n");
    }
    else
    {
        PRINTF("FlashEraseProgramDuringRAMInterrupt failed !\r\n");
    }
    /* Resume flash region status */
    EraseFlashTestRegion(&flashInstance, destAddr);
    /* 8. Update flash wait states according to main clock speed variation */
    if (FlashUpdateWaitStatesAccordingToMainClk(&flashInstance, destAddr) == kStatus_Success)
    {
        PRINTF("Finished FlashUpdateWaitStatesAccordingToMainClk with success !\r\n");
    }
    else
    {
        PRINTF("FlashUpdateWaitStatesAccordingToMainClk failed !\r\n");
    }
    /* 9. Check the entire flash for ECC errors.*/
    if (FLASH_CheckECC(&flashInstance, 0, FSL_FEATURE_SYSCON_FLASH_SIZE_BYTES) == kStatus_Success)
    {
        PRINTF("FLASH_CheckECC found no ECC errors !\r\n");
    }
    else
    {
        PRINTF("ECC errors present in flash !\r\n");
    }

    /* 10.Disable speculation buffer .*/
    FLASH_CacheSpeculationControl(false);

    while (1)
    {
        ;
    }
}
