/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

volatile int exit_code = 0;

#include <stdint.h>
#include <stdbool.h>
#include "fsl_debug_console.h"
#include "ela_csec_utils.h"
#include "fsl_flash.h"
#include "board.h"
#include "app.h"


/*! @brief Flash driver Structure */
static flexnvm_config_t s_flashDriver;

csec_state_t csecState;

#define CONFIG_IFLASH_DEPART_CODE 0x04
#define CONFIG_IFLASH_EEESIZE_CODE 0x02

#define FLEXNVM_PARTITION_CODE     CONFIG_IFLASH_DEPART_CODE
#define EEPROM_DATA_SET_SIZE_CODE  CONFIG_IFLASH_EEESIZE_CODE

/* Set this macro-definition to 1 if you want to partition FLASH */
#define CSEC_DEMO_PARTITION 1

/* Set this macro-definition to 1 if you want to reset all the keys */
#define ERASE_ALL_KEYS	0

/*
 * @brief Gets called when an error occurs.
 *
 * @details Print error message and trap forever.
 */
void error_trap(void)
{
    PRINTF("\r\n\r\n\r\n\t---- HALTED DUE TO FLASH ERROR! ----");
    while (1)
    {
    }
}

void initFlashForCsecOperation(void)
{
  /* To access the CSEcfeature set, the part must be configured for EEE operation, using the PGMPART command.
   * By enabling security features and configuring a number of user keys, 
   * the total size of the 4 Kbyte FlexRAMused for EEEPROM will be reduced by the space required to store the user keys.
   * The user key space will then effectively be unaddressablespace in the FlexRAM. 
   */

    status_t status = kStatus_Fail;
    ftfx_security_state_t securityStatus = kFTFx_SecurityStateNotSecure; /* Return protection status */

    /* Clean up Flash driver Structure*/
    memset(&s_flashDriver, 0, sizeof(flexnvm_config_t));
    
    /* Setup flash driver structure for device and initialize variables. */
    status = FLEXNVM_Init(&s_flashDriver);
    if (kStatus_FTFx_Success != status)
    {
        error_trap();
    }

    /* Check security status. */
    status = FLEXNVM_GetSecurityState(&s_flashDriver, &securityStatus);
    if (kStatus_FTFx_Success != status)
    {
        error_trap();
    }

    /* Print security status. */
    switch (securityStatus)
    {
        case kFTFx_SecurityStateNotSecure:
            PRINTF("Flash is UNSECURE");
            break;
        case kFTFx_SecurityStateBackdoorEnabled:
            PRINTF("Flash is SECURE, BACKDOOR is ENABLED");
            break;
        case kFTFx_SecurityStateBackdoorDisabled:
            PRINTF("Flash is SECURE, BACKDOOR is DISABLED");
            break;
        default:
            break;
    }
    PRINTF("\r\n");

#if CSEC_DEMO_PARTITION
    
    uint32_t eepromDataSizeCode   = EEPROM_DATA_SET_SIZE_CODE;
    uint32_t flexnvmPartitionCode = FLEXNVM_PARTITION_CODE;
    uint32_t eepromTotalSize = 0u;
    
    FLEXNVM_GetProperty(&s_flashDriver, kFLEXNVM_PropertyEepromTotalSize, &eepromTotalSize);
    
    PRINTF("To access the CSEcfeature set, the part must be configured for EEE operation, using the PGMPART command.\r\n");
    PRINTF("For this purpose FLASH must be parittioned.\r\n\r\n");
    PRINTF("Press any key to trigger partitioning...\r\n");
    GETCHAR();

    if (eepromTotalSize == 0u)
    {
        status = FLEXNVM_ProgramPartition_CSE(&s_flashDriver, kFTFx_PartitionFlexramLoadOptLoadedWithValidEepromData,
                                      eepromDataSizeCode, flexnvmPartitionCode, 0x3, 0);
        if(status == kStatus_Success)
        {
            PRINTF("Partitioning - success! ");
            FLEXNVM_GetProperty(&s_flashDriver, kFLEXNVM_PropertyEepromTotalSize, &eepromTotalSize);
            PRINTF("EEPROM size: 0x%x", eepromTotalSize);
        }
    }
    else
    {
        PRINTF("EEPROM already partitioned, make sure CSEc was enabeled!\r\n");
    }
#endif
}

int main(void)
{
  /* Init hardware */
  BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

  uint8_t key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
              0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
  uint8_t rnd[16] = {0u};
  
  PRINTF("CSEc Driver Example\r\n\r\n");
  
  /* Initialize CSEc driver */
  CSEC_DRV_Init(&csecState);
  PRINTF("CSEc Init - done\r\n");
  
  /* Initialize Flash for CSEc operation */
  initFlashForCsecOperation();

#if ERASE_ALL_KEYS
  eraseKeys();
#endif /* ERASE_ALL_KEYS */
  
  /* For Automation pourposes we use only volatile RAM_KEY, 
   * but below functions can be used as reference */
  /* Load the MASTER_ECU key with a known value, which will be used as Authorization
   * key (a secret key known by the application in order to configure other user keys) */
  //setAuthKey();
  /* Load the selected key */
  /* First load => counter == 1 */
  //keyLoaded = loadKey(CSEC_KEY_2, key, 2);

  if(CSEC_DRV_InitRNG() != kStatus_Success)
  {
      error_trap();
  }
  PRINTF("CSEc RND Init - done\r\n");
  
  if(CSEC_DRV_GenerateRND(rnd) != kStatus_Success)
  {
      error_trap();
  }

  PRINTF("CSEc Generate random number - success!\r\n");
  
  if( CSEC_DRV_LoadPlainKey(key) != kStatus_Success)
  {
      error_trap();
  }

  PRINTF("CSEc Load plaintext key in KEY_RAM location - success!\r\n");  
  

  /* Test an encryption using the loaded key.
   *
   * key        = 000102030405060708090a0b0c0d0e0f
   * plaintext  = 00112233445566778899aabbccddeeff
   * ciphertext = 69c4e0d86a7b0430d8cdb78070b4c55a
   *
   * The values are extracted from the SHE Spec 1.1 test vectors.
   */
  uint8_t i;

  status_t stat;
  bool encryptionOk = false;
  uint8_t cipherText[16];
  uint8_t plainText[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
  uint8_t expectedCipherText[16] = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04,
    0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};

  stat = CSEC_DRV_EncryptECB(CSEC_RAM_KEY, plainText, 16U, cipherText, 1U);
  if (stat == kStatus_Success)
  {
      PRINTF("CSEc AES-ECB encryption - success!\r\n");
      /* Check if the cipher text is the one expected */
      for (i = 0; i < 16; i++)
      {
          if (cipherText[i] == expectedCipherText[i])
          {
              encryptionOk = true;
          }
          else
          {
              encryptionOk = false;
              break;
          }
      }
  }

  if (encryptionOk)
  {
      PRINTF("Encrypted cipher match the expected value!\r\n ");
  }

  PRINTF("\r\n End of Example\r\n");
  
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;

} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/

