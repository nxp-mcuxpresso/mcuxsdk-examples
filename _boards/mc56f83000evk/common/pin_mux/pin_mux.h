/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#define GPIO_PUR_PU_15_MASK 0x8000u /*!<@brief Pull Resistor Enable Bits Mask for item 15. */
#define GPIO_PUR_PU_1_MASK 0x02u    /*!<@brief Pull Resistor Enable Bits Mask for item 1. */
#define GPIO_PUR_PU_4_MASK 0x10u    /*!<@brief Pull Resistor Enable Bits Mask for item 4. */
#define GPIO_PUR_PU_6_MASK 0x40u    /*!<@brief Pull Resistor Enable Bits Mask for item 6. */
#define PUR_PU_15_DISABLED 0x00u    /*!<@brief Pull Resistor Enable Bits: Pull resistor is disabled */
#define PUR_PU_1_DISABLED 0x00u     /*!<@brief Pull Resistor Enable Bits: Pull resistor is disabled */
#define PUR_PU_4_DISABLED 0x00u     /*!<@brief Pull Resistor Enable Bits: Pull resistor is disabled */
#define PUR_PU_6_DISABLED 0x00u     /*!<@brief Pull Resistor Enable Bits: Pull resistor is disabled */

/*! @name GPIOF6 (number 94), SW4
  @{ */
#define BOARD_SW4_GPIO GPIOF          /*!<@brief GPIO device name: GPIOF */
#define BOARD_SW4_PIN 6U              /*!<@brief GPIOF pin index: 6 */
#define BOARD_SW4_PIN_MASK kGPIO_Pin6 /*!<@brief PORT pin mask */
                                      /* @} */

/*! @name GPIOF1 (number 77), SW3
  @{ */
#define BOARD_SW3_GPIO GPIOF          /*!<@brief GPIO device name: GPIOF */
#define BOARD_SW3_PIN 1U              /*!<@brief GPIOF pin index: 1 */
#define BOARD_SW3_PIN_MASK kGPIO_Pin1 /*!<@brief PORT pin mask */
                                      /* @} */

/*! @name GPIOF15 (number 46), SW2
  @{ */
#define BOARD_SW2_GPIO GPIOF           /*!<@brief GPIO device name: GPIOF */
#define BOARD_SW2_PIN 15U              /*!<@brief GPIOF pin index: 15 */
#define BOARD_SW2_PIN_MASK kGPIO_Pin15 /*!<@brief PORT pin mask */
                                       /* @} */

/*! @name GPIOC4 (number 12), SW5
  @{ */
#define BOARD_SW5_GPIO GPIOC          /*!<@brief GPIO device name: GPIOC */
#define BOARD_SW5_PIN 4U              /*!<@brief GPIOC pin index: 4 */
#define BOARD_SW5_PIN_MASK kGPIO_Pin4 /*!<@brief PORT pin mask */
                                      /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitBUTTONsPins(void);

/*! @name GPIOF8 (number 6), USER_LED1
  @{ */
#define BOARD_LED_RED_GPIO GPIOF          /*!<@brief GPIO device name: GPIOF */
#define BOARD_LED_RED_PIN 8U              /*!<@brief GPIOF pin index: 8 */
#define BOARD_LED_RED_PIN_MASK kGPIO_Pin8 /*!<@brief PORT pin mask */
                                          /* @} */

/*! @name GPIOF11 (number 45), USER_LED4
  @{ */
#define BOARD_LED_YELLOW_GPIO GPIOF           /*!<@brief GPIO device name: GPIOF */
#define BOARD_LED_YELLOW_PIN 11U              /*!<@brief GPIOF pin index: 11 */
#define BOARD_LED_YELLOW_PIN_MASK kGPIO_Pin11 /*!<@brief PORT pin mask */
                                              /* @} */

/*! @name GPIOF10 (number 56), USER_LED3
  @{ */
#define BOARD_LED_BLUE_GPIO GPIOF           /*!<@brief GPIO device name: GPIOF */
#define BOARD_LED_BLUE_PIN 10U              /*!<@brief GPIOF pin index: 10 */
#define BOARD_LED_BLUE_PIN_MASK kGPIO_Pin10 /*!<@brief PORT pin mask */
                                            /* @} */

/*! @name GPIOF9 (number 57), USER_LED2
  @{ */
#define BOARD_LED_GREEN_GPIO GPIOF          /*!<@brief GPIO device name: GPIOF */
#define BOARD_LED_GREEN_PIN 9U              /*!<@brief GPIOF pin index: 9 */
#define BOARD_LED_GREEN_PIN_MASK kGPIO_Pin9 /*!<@brief PORT pin mask */
                                            /* @} */

/*! @name GPIOE0 (number 68), J3[13]/U3A[1]/GE0/PWMA_0B/LED0
  @{ */
#define BOARD_LED0_GPIO GPIOE          /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED0_PIN 0U              /*!<@brief GPIOE pin index: 0 */
#define BOARD_LED0_PIN_MASK kGPIO_Pin0 /*!<@brief PORT pin mask */
                                       /* @} */

/*! @name GPIOE1 (number 69), J3[15]/U3B[3]/GE1/PWMA_0A/LED1
  @{ */
#define BOARD_LED1_GPIO GPIOE          /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED1_PIN 1U              /*!<@brief GPIOE pin index: 1 */
#define BOARD_LED1_PIN_MASK kGPIO_Pin1 /*!<@brief PORT pin mask */
                                       /* @} */

/*! @name GPIOE2 (number 74), J3[9]/GE2/PWMA_1B
  @{ */
#define BOARD_LED2_GPIO GPIOE          /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED2_PIN 2U              /*!<@brief GPIOE pin index: 2 */
#define BOARD_LED2_PIN_MASK kGPIO_Pin2 /*!<@brief PORT pin mask */
                                       /* @} */

/*! @name GPIOE3 (number 75), J3[11]/GE3/PWMA_1A
  @{ */
#define BOARD_LED3_GPIO GPIOE          /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED3_PIN 3U              /*!<@brief GPIOE pin index: 3 */
#define BOARD_LED3_PIN_MASK kGPIO_Pin3 /*!<@brief PORT pin mask */
                                       /* @} */

/*! @name GPIOE4 (number 82), J3[5]/GE4/PWMA_2B
  @{ */
#define BOARD_LED4_GPIO GPIOE          /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED4_PIN 4U              /*!<@brief GPIOE pin index: 4 */
#define BOARD_LED4_PIN_MASK kGPIO_Pin4 /*!<@brief PORT pin mask */
                                       /* @} */

/*! @name GPIOE5 (number 83), J3[7]/GE5/PWMA_2A
  @{ */
#define BOARD_LED5_GPIO GPIOE          /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED5_PIN 5U              /*!<@brief GPIOE pin index: 5 */
#define BOARD_LED5_PIN_MASK kGPIO_Pin5 /*!<@brief PORT pin mask */
                                       /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDsPins(void);

/*! @name GPIOC0 (number 3), Y1[1]/EXTAL
  @{ */
#define BOARD_EXTAL_GPIO GPIOC          /*!<@brief GPIO device name: GPIOC */
#define BOARD_EXTAL_PIN 0U              /*!<@brief GPIOC pin index: 0 */
#define BOARD_EXTAL_PIN_MASK kGPIO_Pin0 /*!<@brief PORT pin mask */
                                        /* @} */

/*! @name GPIOC1 (number 4), Y1[3]/XTAL
  @{ */
#define BOARD_XTAL_GPIO GPIOC          /*!<@brief GPIO device name: GPIOC */
#define BOARD_XTAL_PIN 1U              /*!<@brief GPIOC pin index: 1 */
#define BOARD_XTAL_PIN_MASK kGPIO_Pin1 /*!<@brief PORT pin mask */
                                       /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitOSCPins(void);

/*! @name GPIOA7 (number 17), THER_B
  @{ */
#define BOARD_THER_B_GPIO GPIOA          /*!<@brief GPIO device name: GPIOA */
#define BOARD_THER_B_PIN 7U              /*!<@brief GPIOA pin index: 7 */
#define BOARD_THER_B_PIN_MASK kGPIO_Pin7 /*!<@brief PORT pin mask */
                                         /* @} */

/*! @name GPIOA6 (number 19), THER_A
  @{ */
#define BOARD_THER_A_GPIO GPIOA          /*!<@brief GPIO device name: GPIOA */
#define BOARD_THER_A_PIN 6U              /*!<@brief GPIOA pin index: 6 */
#define BOARD_THER_A_PIN_MASK kGPIO_Pin6 /*!<@brief PORT pin mask */
                                         /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitTHERMISTORPins(void);

#define GPIO_PUS_PUS_13_MASK 0x2000u /*!<@brief Pull Resistor Type Select Bits Mask for item 13. */
#define PUS_PUS_13_PULLUP 0x2000u    /*!<@brief Pull Resistor Type Select Bits: Pullup resistor */

/*! @name GPIOF13 (number 90), U7[A1]/USB_PWR
  @{ */
#define BOARD_USB_PWR_GPIO GPIOF           /*!<@brief GPIO device name: GPIOF */
#define BOARD_USB_PWR_PIN 13U              /*!<@brief GPIOF pin index: 13 */
#define BOARD_USB_PWR_PIN_MASK kGPIO_Pin13 /*!<@brief PORT pin mask */
                                           /* @} */

/*! @name GPIOF14 (number 91), U7[A2]/USB_OC
  @{ */
#define BOARD_USB_OC_GPIO GPIOF           /*!<@brief GPIO device name: GPIOF */
#define BOARD_USB_OC_PIN 14U              /*!<@brief GPIOF pin index: 14 */
#define BOARD_USB_OC_PIN_MASK kGPIO_Pin14 /*!<@brief PORT pin mask */
                                          /* @} */

/*! @name USB_DP (number 39), USB_DP
  @{ */
/* @} */

/*! @name USB_DM (number 40), USB_DM
  @{ */
/* @} */

/*! @name GPIOF12 (number 89), J16[2]/Q2[2]/USB_OTG_ID
  @{ */
#define BOARD_USB_OTG_ID_GPIO GPIOF           /*!<@brief GPIO device name: GPIOF */
#define BOARD_USB_OTG_ID_PIN 12U              /*!<@brief GPIOF pin index: 12 */
#define BOARD_USB_OTG_ID_PIN_MASK kGPIO_Pin12 /*!<@brief PORT pin mask */
                                              /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitUSBPins(void);

/*! @name GPIOG11 (number 48), U8[5]/MOSI1
  @{ */
#define BOARD_SPI_FLASH_MOSI_GPIO GPIOG           /*!<@brief GPIO device name: GPIOG */
#define BOARD_SPI_FLASH_MOSI_PIN 11U              /*!<@brief GPIOG pin index: 11 */
#define BOARD_SPI_FLASH_MOSI_PIN_MASK kGPIO_Pin11 /*!<@brief PORT pin mask */
                                                  /* @} */

/*! @name GPIOD7 (number 47), U8[2]/MISO1
  @{ */
#define BOARD_SPI_FLASH_MISO_GPIO GPIOD          /*!<@brief GPIO device name: GPIOD */
#define BOARD_SPI_FLASH_MISO_PIN 7U              /*!<@brief GPIOD pin index: 7 */
#define BOARD_SPI_FLASH_MISO_PIN_MASK kGPIO_Pin7 /*!<@brief PORT pin mask */
                                                 /* @} */

/*! @name GPIOF0 (number 55), U8[6]/SCLK1
  @{ */
#define BOARD_SPI_FLASH_SCLK_GPIO GPIOF          /*!<@brief GPIO device name: GPIOF */
#define BOARD_SPI_FLASH_SCLK_PIN 0U              /*!<@brief GPIOF pin index: 0 */
#define BOARD_SPI_FLASH_SCLK_PIN_MASK kGPIO_Pin0 /*!<@brief PORT pin mask */
                                                 /* @} */

/*! @name GPIOF7 (number 95), U8[1]/SS1
  @{ */
#define BOARD_SPI_FLASH_SS_GPIO GPIOF          /*!<@brief GPIO device name: GPIOF */
#define BOARD_SPI_FLASH_SS_PIN 7U              /*!<@brief GPIOF pin index: 7 */
#define BOARD_SPI_FLASH_SS_PIN_MASK kGPIO_Pin7 /*!<@brief PORT pin mask */
                                               /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSPI_FLASHPins(void);

#define GPIO_PUR_PU_2_MASK 0x04u /*!<@brief Pull Resistor Enable Bits Mask for item 2. */
#define GPIO_PUR_PU_3_MASK 0x08u /*!<@brief Pull Resistor Enable Bits Mask for item 3. */
#define PUR_PU_2_ENABLED 0x04u   /*!<@brief Pull Resistor Enable Bits: Pull resistor is enabled */
#define PUR_PU_3_ENABLED 0x08u   /*!<@brief Pull Resistor Enable Bits: Pull resistor is enabled */

/*! @name GPIOF2 (number 60), U11[4]/I2C_SCL1
  @{ */
#define BOARD_ACCEL_MAGNET_SCL_GPIO GPIOF          /*!<@brief GPIO device name: GPIOF */
#define BOARD_ACCEL_MAGNET_SCL_PIN 2U              /*!<@brief GPIOF pin index: 2 */
#define BOARD_ACCEL_MAGNET_SCL_PIN_MASK kGPIO_Pin2 /*!<@brief PORT pin mask */
                                                   /* @} */

/*! @name GPIOF3 (number 61), U11[6]/I2C_SDA1
  @{ */
#define BOARD_ACCEL_MAGNET_SDA_GPIO GPIOF          /*!<@brief GPIO device name: GPIOF */
#define BOARD_ACCEL_MAGNET_SDA_PIN 3U              /*!<@brief GPIOF pin index: 3 */
#define BOARD_ACCEL_MAGNET_SDA_PIN_MASK kGPIO_Pin3 /*!<@brief PORT pin mask */
                                                   /* @} */

/*! @name GPIOF8 (number 6), USER_LED1
  @{ */
#define BOARD_LEDRED_GPIO GPIOF          /*!<@brief GPIO device name: GPIOF */
#define BOARD_LEDRED_PIN 8U              /*!<@brief GPIOF pin index: 8 */
#define BOARD_LEDRED_PIN_MASK kGPIO_Pin8 /*!<@brief PORT pin mask */
                                         /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitACCELPins(void);

/*! @name GPIOC11 (number 58), J18[1]/CANTX
  @{ */
#define BOARD_CAN_TX_GPIO GPIOC           /*!<@brief GPIO device name: GPIOC */
#define BOARD_CAN_TX_PIN 11U              /*!<@brief GPIOC pin index: 11 */
#define BOARD_CAN_TX_PIN_MASK kGPIO_Pin11 /*!<@brief PORT pin mask */
                                          /* @} */

/*! @name GPIOC12 (number 59), J18[3]/CANRX
  @{ */
#define BOARD_CAN_RX_GPIO GPIOC           /*!<@brief GPIO device name: GPIOC */
#define BOARD_CAN_RX_PIN 12U              /*!<@brief GPIOC pin index: 12 */
#define BOARD_CAN_RX_PIN_MASK kGPIO_Pin12 /*!<@brief PORT pin mask */
                                          /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitCANPins(void);

/*! @name GPIOD6 (number 9), U14C[9]/TXD_BDM
  @{ */
#define BOARD_TXD_BDM_GPIO GPIOD          /*!<@brief GPIO device name: GPIOD */
#define BOARD_TXD_BDM_PIN 6U              /*!<@brief GPIOD pin index: 6 */
#define BOARD_TXD_BDM_PIN_MASK kGPIO_Pin6 /*!<@brief PORT pin mask */
                                          /* @} */

/*! @name GPIOD5 (number 10), U13[3]/RXD_BDM
  @{ */
#define BOARD_RXD_BDM_GPIO GPIOD          /*!<@brief GPIO device name: GPIOD */
#define BOARD_RXD_BDM_PIN 5U              /*!<@brief GPIOD pin index: 5 */
#define BOARD_RXD_BDM_PIN_MASK kGPIO_Pin5 /*!<@brief PORT pin mask */
                                          /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);

/*! @name GPIOC14 (number 87), J2[18]/I2C_SDA0
  @{ */
#define BOARD_I2C_SDA0_GPIO GPIOC           /*!<@brief GPIO device name: GPIOC */
#define BOARD_I2C_SDA0_PIN 14U              /*!<@brief GPIOC pin index: 14 */
#define BOARD_I2C_SDA0_PIN_MASK kGPIO_Pin14 /*!<@brief PORT pin mask */
                                            /* @} */

/*! @name GPIOC15 (number 88), J2[20]/I2C_SCL0
  @{ */
#define BOARD_I2C_SCL0_GPIO GPIOC           /*!<@brief GPIO device name: GPIOC */
#define BOARD_I2C_SCL0_PIN 15U              /*!<@brief GPIOC pin index: 15 */
#define BOARD_I2C_SCL0_PIN_MASK kGPIO_Pin15 /*!<@brief PORT pin mask */
                                            /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitI2CPins(void);

/*! @name GPIOC7 (number 50), J2[6]/SS0
  @{ */
#define BOARD_SS0_GPIO GPIOC          /*!<@brief GPIO device name: GPIOC */
#define BOARD_SS0_PIN 7U              /*!<@brief GPIOC pin index: 7 */
#define BOARD_SS0_PIN_MASK kGPIO_Pin7 /*!<@brief PORT pin mask */
                                      /* @} */

/*! @name GPIOC8 (number 52), J2[10]/MISO0
  @{ */
#define BOARD_MISO0_GPIO GPIOC          /*!<@brief GPIO device name: GPIOC */
#define BOARD_MISO0_PIN 8U              /*!<@brief GPIOC pin index: 8 */
#define BOARD_MISO0_PIN_MASK kGPIO_Pin8 /*!<@brief PORT pin mask */
                                        /* @} */

/*! @name GPIOC9 (number 53), J2[12]/SCK0
  @{ */
#define BOARD_SCK0_GPIO GPIOC          /*!<@brief GPIO device name: GPIOC */
#define BOARD_SCK0_PIN 9U              /*!<@brief GPIOC pin index: 9 */
#define BOARD_SCK0_PIN_MASK kGPIO_Pin9 /*!<@brief PORT pin mask */
                                       /* @} */

/*! @name GPIOC10 (number 54), J2[8]/MOSI0
  @{ */
#define BOARD_MOSI0_GPIO GPIOC           /*!<@brief GPIO device name: GPIOC */
#define BOARD_MOSI0_PIN 10U              /*!<@brief GPIOC pin index: 10 */
#define BOARD_MOSI0_PIN_MASK kGPIO_Pin10 /*!<@brief PORT pin mask */
                                         /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSPIPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
