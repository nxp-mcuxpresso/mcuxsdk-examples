/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_clock.h"
#include "fsl_power.h"
#include "power_mode_switch_primary.h"
#include "fsl_mu.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_CopyCore1Image(void);
static app_power_mode_select_t DEMO_GetTargetPowerMode(void);
static void DEMO_EnterDesiredPowerMode(app_power_mode_select_t powerMode);
static void DEMO_EnableWakeupSource(app_power_mode_select_t powerMode);

/*******************************************************************************
 * Variables
 ******************************************************************************/
char *const g_modeNameArray[] = DEMO_POWER_MODE_NAME;
char *const g_modeDescArray[] = DEMO_POWER_MODE_DESC;

#if defined(__ICCARM__) /* IAR Workbench */
#pragma location = "rpmsg_sh_mem_section"
power_handle_t powerHandle;
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION) /* Keil MDK */
power_handle_t powerHandle __attribute__((section("rpmsg_sh_mem_section")));
#elif defined(__GNUC__)
power_handle_t powerHandle __attribute__((section(".noinit.$rpmsg_sh_mem")));
#else
#error "RPMsg: Please provide your definition of rpmsg_lite_base[]!"
#endif


/*******************************************************************************
 * Code
 ******************************************************************************/
void MU_A_RX_IRQHandler(void)
{
    Power_MuMessageCallback(MU_ReceiveMsgNonBlocking(APP_MU, APP_MU_CHANNEL), APP_MU_CHANNEL);
}


int main(void)
{
    app_power_mode_select_t selectedPowerMode;
    BOARD_InitHardware();
    PRINTF("\r\n###########################  Power Mode Switch Demo Primary Core Boot  ###########################\r\n");
    PRINTF("Normal Boot......\r\n");
    PRINTF("Core Clock Frequency: %d\r\n", CLOCK_GetCoreSysClkFreq());
    APP_CopyCore1Image();
    APP_BootCore1();
    if (Power_CreateHandle(&powerHandle, APP_MU_CHANNEL) !=  kStatus_Success)
    {
        PRINTF("Fail to sync with secondary core!\r\n");
        return 0;
    }
    while(1)
    {
        selectedPowerMode = DEMO_GetTargetPowerMode();
        if (selectedPowerMode != kAPP_PowerModeActive)
        {
            DEMO_EnableWakeupSource(selectedPowerMode);
            DEMO_EnterDesiredPowerMode(selectedPowerMode);
        }
        
        PRINTF("\r\n--------- Next Loop ---------\r\n");
    }
}

/*!
 * @brief Function to copy core1 image to execution address.
 */
 static void APP_CopyCore1Image(void)
 {
 #ifdef CORE1_IMAGE_COPY_TO_RAM
     /* Calculate size of the image  - not required on MCUXpresso IDE. MCUXpresso copies the secondary core
        image to the target memory during startup automatically */
     uint32_t core1_image_size = get_core1_image_size();
 
     PRINTF("Copy Secondary core image to address: 0x%x, size: %d\r\n", CORE1_BOOT_ADDRESS, core1_image_size);
 
     /* Copy Secondary core application from FLASH to the target memory. */
 #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
     SCB_CleanInvalidateDCache_by_Addr((void *)CORE1_BOOT_ADDRESS, core1_image_size);
 #endif
 #ifdef CORE1_IMAGE_FLUSH_CACHE
     CORE1_IMAGE_FLUSH_CACHE(CORE1_BOOT_ADDRESS, core1_image_size);
 #endif
     memcpy((void *)CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);
 #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
     SCB_CleanInvalidateDCache_by_Addr((void *)CORE1_BOOT_ADDRESS, core1_image_size);
 #endif
 #ifdef CORE1_IMAGE_FLUSH_CACHE
     CORE1_IMAGE_FLUSH_CACHE(CORE1_BOOT_ADDRESS, core1_image_size);
 #endif
 #endif
 }

static void DEMO_EnableWakeupSource(app_power_mode_select_t powerMode)
{
      PRINTF("Please Select Wakeup Source...\r\n");
      PRINTF("\t[1]: EXT_INT button.\r\n");
      
      char ch = GETCHAR();
      switch(ch)
      {
          case '1':
          {
            Power_EnableWakeupSource(kPower_WS_Both_ExternalINTFallEdge);
            break;
           }
          default:
          {
              PRINTF("Do Not Support!\r\n");
              break;
          }
      }
      
}


static app_power_mode_select_t DEMO_GetTargetPowerMode(void)
{
    uint8_t ch;

    app_power_mode_select_t inputPowerMode;

    do
    {
        PRINTF("\r\nSelect the desired operation...\r\n");
        for (app_power_mode_select_t modeIndex = kAPP_PowerModeActive; modeIndex < kAPP_PowerModeMax; modeIndex++)
        {
            PRINTF("\tPress %c to enter: %s mode\r\n", modeIndex,
                   g_modeNameArray[(uint8_t)(modeIndex - kAPP_PowerModeActive)]);
        }

        PRINTF("\r\nWaiting for power mode select...\r\n\r\n");

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }
        inputPowerMode = (app_power_mode_select_t)ch;

        if ((inputPowerMode >= kAPP_PowerModeMax) || (inputPowerMode <= kAPP_PowerModeMin))
        {
            PRINTF("Wrong Input!");
        }
    } while (inputPowerMode >= kAPP_PowerModeMax);

    PRINTF("\t%s\r\n", g_modeDescArray[(uint8_t)(inputPowerMode - kAPP_PowerModeActive)]);

    return inputPowerMode;
}

static void DEMO_EnterDesiredPowerMode(app_power_mode_select_t powerMode)
{
    PRINTF("Please Press EXT_INT Button to wakeup!\r\n");
    
    switch (powerMode)
    {
        case kAPP_PowerModeSleep:
        {
            Power_EnterSleep();
            break;
        }
        case kAPP_PowerModeDeepSleep:
        {
            Power_EnterDeepSleep(NULL);
            break;
        }
        case kAPP_PowerModePowerDown1:
        {
            power_pd_config_t pd1Config = 
            {   
                .disableBandgap = true,
                .mainRamArraysToRetain = (uint32_t)kPower_MainDomainAllRams,
            };
            if (Power_EnterPowerDown1(&pd1Config) != kStatus_Success)
            {
                PRINTF("Fail to enter Power Down1 Mode!\r\n");
            }
            break;
        }
        case kAPP_PowerModePowerDown2:
        {
            power_pd_config_t pd2Config = 
            {
                .disableBandgap = true,
                .mainRamArraysToRetain = (uint32_t)kPower_MainDomainAllRams,
            };
            if (Power_EnterPowerDown1(&pd2Config) != kStatus_Success)
            {
                PRINTF("Fail to enter Power Down2 Mode!\r\n");
            }
            break;
        }
        case kAPP_PowerModeDeepPowerDown1:
        {
            power_dpd1_config_t dpd1Config = {
              .disableBandgap = true,
              .mainRamArraysToRetain = (uint32_t)kPower_MainDomainAllRams,
            };
            
            if (Power_EnterDeepPowerDown1(&dpd1Config) != kStatus_Success)
            {
                PRINTF("Fail to enter Deep Power Down1 Mode!\r\n");
            }
            break;
        }
        case kAPP_PowerModeDeepPowerDown2:
        {
            power_dpd2_config_t dpd2Config = {
                .aonRamArraysToRetain = kPower_AonDomainAllRams,
                .disableBandgap = true,
                .mainRamArraysToRetain = kPower_MainDomainAllRams,
                .switchToX32K = false,
            };
            
            if (Power_EnterDeepPowerDown2(&dpd2Config) != kStatus_Success)
            {
                PRINTF("Fail to enter Deep Power Down2 Mode!\r\n");
            }
            break;
        }
        case kAPP_PowerModeDeepPowerDown3:
        {
            power_dpd3_config_t dpd3Config;
            
            if (Power_EnterDeepPowerDown3(&dpd3Config) != kStatus_Success)
            {
                PRINTF("Fail to enter Deep Power Down3 Mode!\r\n");
            }
            break;
        }
        case kAPP_PowerModeShutDown:
        {
            power_sd_config_t sdConfig;
            
            if (Power_EnterShutDown(&sdConfig) != kStatus_Success)
            {
                PRINTF("Fail to enter Shut Down Mode!\r\n");
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

