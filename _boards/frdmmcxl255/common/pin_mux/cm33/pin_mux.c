/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "fsl_reset.h"

void BOARD_InitDEBUG_UARTPins(void)
{
    /* Write to PORT1: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GatePORT2);
    /* LPUART0 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kLPUART0_RST_SHIFT_RSTn);
    /* PORT1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kPORT2_RST_SHIFT_RSTn);

    /* TX */
    const port_pin_config_t port2_11_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullDisable,
                                                      /* Low internal pull resistor value is selected. */
                                                      kPORT_LowPullResistor,
                                                      /* Fast slew rate is configured */
                                                      kPORT_FastSlewRate,
                                                      /* Passive input filter is disabled */
                                                      kPORT_PassiveFilterDisable,
                                                      /* Open drain output is disabled */
                                                      kPORT_OpenDrainDisable,
                                                      /* Low drive strength is configured */
                                                      kPORT_LowDriveStrength,
                                                      /* Normal drive strength is configured */
                                                      kPORT_NormalDriveStrength,
                                                      /* Pin is configured as LPUART0_TXD */
                                                      kPORT_MuxAlt2,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};
    /* PORT2_11 is configured as LPUART0_TXD */
    PORT_SetPinConfig(PORT2, 11U, &port2_11_config);

    /* RX */
    const port_pin_config_t port2_10_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullDisable,
                                                      /* Low internal pull resistor value is selected. */
                                                      kPORT_LowPullResistor,
                                                      /* Fast slew rate is configured */
                                                      kPORT_FastSlewRate,
                                                      /* Passive input filter is disabled */
                                                      kPORT_PassiveFilterDisable,
                                                      /* Open drain output is disabled */
                                                      kPORT_OpenDrainDisable,
                                                      /* Low drive strength is configured */
                                                      kPORT_LowDriveStrength,
                                                      /* Normal drive strength is configured */
                                                      kPORT_NormalDriveStrength,
                                                      /* Pin is configured as LPUART0_RXD */
                                                      kPORT_MuxAlt2,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};
    /* PORT2_10 is configured as LPUART0_RXD */
    PORT_SetPinConfig(PORT2, 10U, &port2_10_config);
}

void BOARD_InitSWD_DEBUGPins(void)
{
}

void BOARD_InitI2CPins(void)
{
    /* SDA */
    const port_pin_config_t port3_11_config = {/* Internal pull-up resistor is enabled */
                                                     kPORT_PullUp,
                                                     /* Low internal pull resistor value is selected. */
                                                     kPORT_LowPullResistor,
                                                     /* Fast slew rate is configured */
                                                     kPORT_FastSlewRate,
                                                     /* Passive input filter is disabled */
                                                     kPORT_PassiveFilterDisable,
                                                     /* Open drain output is disabled */
                                                     kPORT_OpenDrainDisable,
                                                     /* Low drive strength is configured */
                                                     kPORT_LowDriveStrength,
                                                     /* Normal drive strength is configured */
                                                     kPORT_NormalDriveStrength,
                                                     /* Pin is configured as LPI2C0_SDA */
                                                     kPORT_MuxAlt4,
                                                     /* Digital input enabled */
                                                     kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     kPORT_UnlockRegister};

    PORT_SetPinConfig(PORT3, 11U, &port3_11_config);

    /* SCL */
    const port_pin_config_t port3_10_config = {/* Internal pull-up resistor is enabled */
                                                     kPORT_PullUp,
                                                     /* Low internal pull resistor value is selected. */
                                                     kPORT_LowPullResistor,
                                                     /* Fast slew rate is configured */
                                                     kPORT_FastSlewRate,
                                                     /* Passive input filter is disabled */
                                                     kPORT_PassiveFilterDisable,
                                                     /* Open drain output is disabled */
                                                     kPORT_OpenDrainDisable,
                                                     /* Low drive strength is configured */
                                                     kPORT_LowDriveStrength,
                                                     /* Normal drive strength is configured */
                                                     kPORT_NormalDriveStrength,
                                                     /* Pin is configured as LPI2C0_SCL */
                                                     kPORT_MuxAlt4,
                                                     /* Digital input enabled */
                                                     kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     kPORT_UnlockRegister};

    PORT_SetPinConfig(PORT3, 10U, &port3_10_config);
}

void BOARD_InitSPIPins(void)
{
    /*LPSPI1_SDO*/
    const port_pin_config_t port1_17_config = {/* Internal pull-up/down resistor is disabled */
                                                     .pullSelect = kPORT_PullDisable,
                                                     /* Low internal pull resistor value is selected. */
                                                     .pullValueSelect = kPORT_LowPullResistor,
                                                     /* Fast slew rate is configured */
                                                     .slewRate = kPORT_FastSlewRate,
                                                     /* Passive input filter is disabled */
                                                     .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                                     /* Open drain output is disabled */
                                                     .openDrainEnable = kPORT_OpenDrainDisable,
                                                     /* Low drive strength is configured */
                                                     .driveStrength = kPORT_LowDriveStrength,
                                                     /* Normal drive strength is configured */
                                                     .driveStrength1 = kPORT_NormalDriveStrength,
                                                     /* Pin is configured as LPSPI1_SDO */
                                                     .mux = kPORT_MuxAlt3,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    PORT_SetPinConfig(PORT1, 17U, &port1_17_config);

    /* LPSPI1_SCK */
    const port_pin_config_t port1_18_config = {/* Internal pull-up/down resistor is disabled */
                                                     .pullSelect = kPORT_PullDisable,
                                                     /* Low internal pull resistor value is selected. */
                                                     .pullValueSelect = kPORT_LowPullResistor,
                                                     /* Fast slew rate is configured */
                                                     .slewRate = kPORT_FastSlewRate,
                                                     /* Passive input filter is disabled */
                                                     .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                                     /* Open drain output is disabled */
                                                     .openDrainEnable = kPORT_OpenDrainDisable,
                                                     /* Low drive strength is configured */
                                                     .driveStrength = kPORT_LowDriveStrength,
                                                     /* Normal drive strength is configured */
                                                     .driveStrength1 = kPORT_NormalDriveStrength,
                                                     /* Pin is configured as LPSPI1_SCK */
                                                     .mux = kPORT_MuxAlt3,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    PORT_SetPinConfig(PORT1, 18U, &port1_18_config);

    /* LPSPI1_SDI */
    const port_pin_config_t port1_19_config = {/* Internal pull-up/down resistor is disabled */
                                                     .pullSelect = kPORT_PullDisable,
                                                     /* Low internal pull resistor value is selected. */
                                                     .pullValueSelect = kPORT_LowPullResistor,
                                                     /* Fast slew rate is configured */
                                                     .slewRate = kPORT_FastSlewRate,
                                                     /* Passive input filter is disabled */
                                                     .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                                     /* Open drain output is disabled */
                                                     .openDrainEnable = kPORT_OpenDrainDisable,
                                                     /* Low drive strength is configured */
                                                     .driveStrength = kPORT_LowDriveStrength,
                                                     /* Normal drive strength is configured */
                                                     .driveStrength1 = kPORT_NormalDriveStrength,
                                                     /* Pin is configured as LPSPI1_SDI */
                                                     .mux = kPORT_MuxAlt3,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    PORT_SetPinConfig(PORT1, 19U, &port1_19_config);

    /* LPSPI1_PCS0 */
    const port_pin_config_t port1_20_config = {/* Internal pull-up/down resistor is disabled */
                                                     .pullSelect = kPORT_PullDisable,
                                                     /* Low internal pull resistor value is selected. */
                                                     .pullValueSelect = kPORT_LowPullResistor,
                                                     /* Fast slew rate is configured */
                                                     .slewRate = kPORT_FastSlewRate,
                                                     /* Passive input filter is disabled */
                                                     .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                                     /* Open drain output is disabled */
                                                     .openDrainEnable = kPORT_OpenDrainDisable,
                                                     /* Low drive strength is configured */
                                                     .driveStrength = kPORT_LowDriveStrength,
                                                     /* Normal drive strength is configured */
                                                     .driveStrength1 = kPORT_NormalDriveStrength,
                                                     /* Pin is configured as LPSPI1_PCS0 */
                                                     .mux = kPORT_MuxAlt3,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    PORT_SetPinConfig(PORT1, 20U, &port1_20_config);
}

void BOARD_InitQTMRPins(void) {
    /* AONQTMR1_OUT1 used as output */
    const port_pin_config_t port0_13_config = {/* Internal pull-up/down resistor is disabled */
                                                     .pullSelect = kPORT_PullDisable,
                                                     /* Low internal pull resistor value is selected. */
                                                     .pullValueSelect = kPORT_LowPullResistor,
                                                     /* Fast slew rate is configured */
                                                     .slewRate = kPORT_FastSlewRate,
                                                     /* Passive input filter is disabled */
                                                     .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                                     /* Open drain output is disabled */
                                                     .openDrainEnable = kPORT_OpenDrainDisable,
                                                     /* Low drive strength is configured */
                                                     .driveStrength = kPORT_LowDriveStrength,
                                                     /* Normal drive strength is configured */
                                                     .driveStrength1 = kPORT_NormalDriveStrength,
                                                     /* Pin is configured as AONQTMR1_OUT1 */
                                                     .mux = kPORT_MuxAlt5,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    PORT_SetPinConfig(AON__PORT0, 13U, &port0_13_config);
    
    /* AONQTMR1_OUT0 used as input */
    const port_pin_config_t port0_12_config = {/* Internal pull-up/down resistor is disabled */
                                                     .pullSelect = kPORT_PullDisable,
                                                     /* Low internal pull resistor value is selected. */
                                                     .pullValueSelect = kPORT_LowPullResistor,
                                                     /* Fast slew rate is configured */
                                                     .slewRate = kPORT_FastSlewRate,
                                                     /* Passive input filter is disabled */
                                                     .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                                     /* Open drain output is disabled */
                                                     .openDrainEnable = kPORT_OpenDrainDisable,
                                                     /* Low drive strength is configured */
                                                     .driveStrength = kPORT_LowDriveStrength,
                                                     /* Normal drive strength is configured */
                                                     .driveStrength1 = kPORT_NormalDriveStrength,
                                                     /* Pin is configured as AONQTMR1_OUT0 */
                                                     .mux = kPORT_MuxAlt5,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    PORT_SetPinConfig(AON__PORT0, 12U, &port0_12_config);
}

void BOARD_InitLEDsPins(void) {
  
      gpio_pin_config_t output_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };

    CLOCK_EnableClock(kCLOCK_GatePORT1);
    CLOCK_EnableClock(kCLOCK_GatePORT2);
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kGPIO2_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);
    RESET_ReleasePeripheralReset(kPORT2_RST_SHIFT_RSTn);
  
    /* LED RED */
    const port_pin_config_t port1_15_config = {/* Internal pull-up/down resistor is disabled */
                                                     .pullSelect = kPORT_PullDisable,
                                                     /* Low internal pull resistor value is selected. */
                                                     .pullValueSelect = kPORT_LowPullResistor,
                                                     /* Fast slew rate is configured */
                                                     .slewRate = kPORT_FastSlewRate,
                                                     /* Passive input filter is disabled */
                                                     .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                                     /* Open drain output is disabled */
                                                     .openDrainEnable = kPORT_OpenDrainDisable,
                                                     /* Low drive strength is configured */
                                                     .driveStrength = kPORT_LowDriveStrength,
                                                     /* Normal drive strength is configured */
                                                     .driveStrength1 = kPORT_NormalDriveStrength,
                                                     /* Pin is configured as GPIO */
                                                     .mux = kPORT_MuxAlt0,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    PORT_SetPinConfig(PORT1, 15U, &port1_15_config);
    
    /* LED GREEN */
    const port_pin_config_t port1_16_config = {/* Internal pull-up/down resistor is disabled */
                                                     .pullSelect = kPORT_PullDisable,
                                                     /* Low internal pull resistor value is selected. */
                                                     .pullValueSelect = kPORT_LowPullResistor,
                                                     /* Fast slew rate is configured */
                                                     .slewRate = kPORT_FastSlewRate,
                                                     /* Passive input filter is disabled */
                                                     .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                                     /* Open drain output is disabled */
                                                     .openDrainEnable = kPORT_OpenDrainDisable,
                                                     /* Low drive strength is configured */
                                                     .driveStrength = kPORT_LowDriveStrength,
                                                     /* Normal drive strength is configured */
                                                     .driveStrength1 = kPORT_NormalDriveStrength,
                                                     /* Pin is configured as GPIO */
                                                     .mux = kPORT_MuxAlt0,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    PORT_SetPinConfig(PORT1, 16U, &port1_16_config);
    
    /* LED BLUE */
    const port_pin_config_t port2_12_config = {/* Internal pull-up/down resistor is disabled */
                                                     .pullSelect = kPORT_PullDisable,
                                                     /* Low internal pull resistor value is selected. */
                                                     .pullValueSelect = kPORT_LowPullResistor,
                                                     /* Fast slew rate is configured */
                                                     .slewRate = kPORT_FastSlewRate,
                                                     /* Passive input filter is disabled */
                                                     .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                                     /* Open drain output is disabled */
                                                     .openDrainEnable = kPORT_OpenDrainDisable,
                                                     /* Low drive strength is configured */
                                                     .driveStrength = kPORT_LowDriveStrength,
                                                     /* Normal drive strength is configured */
                                                     .driveStrength1 = kPORT_NormalDriveStrength,
                                                     /* Pin is configured as GPIO */
                                                     .mux = kPORT_MuxAlt0,
                                                     /* Digital input enabled */
                                                     .inputBuffer = kPORT_InputBufferEnable,
                                                     /* Digital input is not inverted */
                                                     .invertInput = kPORT_InputNormal,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     .lockRegister = kPORT_UnlockRegister};
    PORT_SetPinConfig(PORT2, 12U, &port2_12_config); 
    
    GPIO_PinInit(GPIO1, 15, &output_config);
    GPIO_PinInit(GPIO1, 16, &output_config);
    GPIO_PinInit(GPIO2, 12, &output_config);
}

void BOARD_InitACMPPins()
{
    CLOCK_EnableClock(kCLOCK_GatePORT2);
    RESET_ReleasePeripheralReset(kPORT2_RST_SHIFT_RSTn);
    
    const port_pin_config_t port2_15_config = {/* Internal pull-up/down resistor is disabled */
                                                 kPORT_PullDisable,
                                                 /* Low internal pull resistor value is selected. */
                                                 kPORT_LowPullResistor,
                                                 /* Fast slew rate is configured */
                                                 kPORT_FastSlewRate,
                                                 /* Passive input filter is disabled */
                                                 kPORT_PassiveFilterDisable,
                                                 /* Open drain output is disabled */
                                                 kPORT_OpenDrainDisable,
                                                 /* Low drive strength is configured */
                                                 kPORT_LowDriveStrength,
                                                 /* Normal drive strength is configured */
                                                 kPORT_NormalDriveStrength,
                                                 /* Pin is configured as main ACMP IN3 */
                                                 kPORT_MuxAlt0,
                                                 /* Digital input disabled; it is required for analog functions */
                                                 kPORT_InputBufferDisable,
                                                 /* Digital input is not inverted */
                                                 kPORT_InputNormal,
                                                 /* Pin Control Register fields [15:0] are not locked */
                                                 kPORT_UnlockRegister};
    /* PORT2_15 is configured as main ACMP IN3 */
    PORT_SetPinConfig(PORT2, 15U, &port2_15_config);
}

void BOARD_InitBUTTONsPins(void)
{
#if 0
    /* GPIO0: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateAonGPIO);
    /* GPIO1: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateGPIO1);
    /* PORT0: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateAonPORT);
    /* PORT1: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GatePORT1);
    /* GPIO1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    /* PORT1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);

    gpio_pin_config_t SW2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO0_9   */
    GPIO_PinInit(BOARD_INITBUTTONSPINS_SW2_GPIO, BOARD_INITBUTTONSPINS_SW2_PIN, &SW2_config);

    gpio_pin_config_t SW5_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO1_14   */
    GPIO_PinInit(BOARD_INITBUTTONSPINS_SW5_GPIO, BOARD_INITBUTTONSPINS_SW5_PIN, &SW5_config);


    const port_pin_config_t SW2 = {/* Internal pull-up/down resistor is disabled */
                                   .pullSelect = kPORT_PullDisable,
                                   /* Low internal pull resistor value is selected. */
                                   .pullValueSelect = kPORT_LowPullResistor,
                                   /* Fast slew rate is configured */
                                   .slewRate = kPORT_FastSlewRate,
                                   /* Passive input filter is disabled */
                                   .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                   /* Open drain output is disabled */
                                   .openDrainEnable = kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   .driveStrength = kPORT_LowDriveStrength,
                                   /* Normal drive strength is configured */
                                   .driveStrength1 = kPORT_NormalDriveStrength,
                                   /* Pin is configured as P0_9 */
                                   .mux = kPORT_MuxAlt0,
                                   /* Digital input enabled */
                                   .inputBuffer = kPORT_InputBufferEnable,
                                   /* Digital input is not inverted */
                                   .invertInput = kPORT_InputNormal,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   .lockRegister = kPORT_UnlockRegister};
    /* PORT0_9  is configured as P0_9 */
    PORT_SetPinConfig(BOARD_INITBUTTONSPINS_SW2_PORT, BOARD_INITBUTTONSPINS_SW2_PIN, &SW2);

    const port_pin_config_t SW5 = {/* Internal pull-up/down resistor is disabled */
                                   .pullSelect = kPORT_PullDisable,
                                   /* Low internal pull resistor value is selected. */
                                   .pullValueSelect = kPORT_LowPullResistor,
                                   /* Fast slew rate is configured */
                                   .slewRate = kPORT_FastSlewRate,
                                   /* Passive input filter is disabled */
                                   .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                   /* Open drain output is disabled */
                                   .openDrainEnable = kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   .driveStrength = kPORT_LowDriveStrength,
                                   /* Normal drive strength is configured */
                                   .driveStrength1 = kPORT_NormalDriveStrength,
                                   /* Pin is configured as P1_14 */
                                   .mux = kPORT_MuxAlt0,
                                   /* Digital input enabled */
                                   .inputBuffer = kPORT_InputBufferEnable,
                                   /* Digital input is not inverted */
                                   .invertInput = kPORT_InputNormal,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   .lockRegister = kPORT_UnlockRegister};
    /* PORT1_14 is configured as P1_14 */
    PORT_SetPinConfig(BOARD_INITBUTTONSPINS_SW5_PORT, BOARD_INITBUTTONSPINS_SW5_PIN, &SW5);
#endif
}

void BOARD_InitLPCMPPins()
{
    const port_pin_config_t port2_15_config = {/* Internal pull-up/down resistor is disabled */
                                                 kPORT_PullDisable,
                                                 /* Low internal pull resistor value is selected. */
                                                 kPORT_LowPullResistor,
                                                 /* Fast slew rate is configured */
                                                 kPORT_FastSlewRate,
                                                 /* Passive input filter is disabled */
                                                 kPORT_PassiveFilterDisable,
                                                 /* Open drain output is disabled */
                                                 kPORT_OpenDrainDisable,
                                                 /* Low drive strength is configured */
                                                 kPORT_LowDriveStrength,
                                                 /* Normal drive strength is configured */
                                                 kPORT_NormalDriveStrength,
                                                 /* Pin is configured as CMP0_IN5 */
                                                 kPORT_MuxAlt0,
                                                 /* Digital input disabled; it is required for analog functions */
                                                 kPORT_InputBufferDisable,
                                                 /* Digital input is not inverted */
                                                 kPORT_InputNormal,
                                                 /* Pin Control Register fields [15:0] are not locked */
                                                 kPORT_UnlockRegister};
    /* PORT2_15 is configured as CMP0_IN3 */
    PORT_SetPinConfig(PORT2, 5U, &port2_15_config);
}

void BOARD_InitLCDPins()
{
    const port_pin_config_t port_config = {/* Internal pull-up/down resistor is disabled */
                                                 kPORT_PullDisable,
                                                 /* Low internal pull resistor value is selected. */
                                                 kPORT_LowPullResistor,
                                                 /* Fast slew rate is configured */
                                                 kPORT_FastSlewRate,
                                                 /* Passive input filter is disabled */
                                                 kPORT_PassiveFilterDisable,
                                                 /* Open drain output is disabled */
                                                 kPORT_OpenDrainDisable,
                                                 /* Low drive strength is configured */
                                                 kPORT_LowDriveStrength,
                                                 /* Normal drive strength is configured */
                                                 kPORT_NormalDriveStrength,
                                                 /* Pin is configured as LCD */
                                                 kPORT_MuxAlt0,
                                                 /* Digital input disabled; it is required for analog functions */
                                                 kPORT_InputBufferDisable,
                                                 /* Digital input is not inverted */
                                                 kPORT_InputNormal,
                                                 /* Pin Control Register fields [15:0] are not locked */
                                                 kPORT_UnlockRegister};

    PORT_SetPinConfig(PORT3, 14U, &port_config);
    PORT_SetPinConfig(PORT1, 21U, &port_config);
    PORT_SetPinConfig(PORT1, 22U, &port_config);
    PORT_SetPinConfig(PORT2,  7U, &port_config);
    PORT_SetPinConfig(PORT1, 12U, &port_config);
    PORT_SetPinConfig(PORT1, 13U, &port_config);
    PORT_SetPinConfig(PORT2, 19U, &port_config);
    PORT_SetPinConfig(PORT2, 20U, &port_config);
    PORT_SetPinConfig(PORT3,  5U, &port_config);
    PORT_SetPinConfig(PORT3,  6U, &port_config);
    PORT_SetPinConfig(PORT3,  7U, &port_config);
    PORT_SetPinConfig(PORT3, 15U, &port_config);
    PORT_SetPinConfig(PORT2, 21U, &port_config);
    PORT_SetPinConfig(PORT1,  0U, &port_config);
    PORT_SetPinConfig(PORT2, 13U, &port_config);
    PORT_SetPinConfig(PORT1, 10U, &port_config);
    PORT_SetPinConfig(PORT1, 11U, &port_config);
    PORT_SetPinConfig(PORT2,  4U, &port_config);
    PORT_SetPinConfig(PORT2,  5U, &port_config);
    PORT_SetPinConfig(PORT2, 16U, &port_config);
}

void BOARD_InitKPPPins()
{
    /* AonKPP peripheral is released from reset */
    RESET_ReleasePeripheralReset(kAonKPP_RST_SHIFT_RSTn);

    const port_pin_config_t port0_13_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullUp,
                                                      /* Low internal pull resistor value is selected. */
                                                      kPORT_LowPullResistor,
                                                      /* Fast slew rate is configured */
                                                      kPORT_FastSlewRate,
                                                      /* Passive input filter is disabled */
                                                      kPORT_PassiveFilterDisable,
                                                      /* Open drain output is disabled */
                                                      kPORT_OpenDrainDisable,
                                                      /* Low drive strength is configured */
                                                      kPORT_LowDriveStrength,
                                                      /* Normal drive strength is configured */
                                                      kPORT_NormalDriveStrength,
                                                      /* Pin is configured as KPP_ROW_1 */
                                                      kPORT_MuxAlt3,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};

    PORT_SetPinConfig(AON__PORT0, 13U, &port0_13_config);

    const port_pin_config_t port0_12_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullUp,
                                                      /* Low internal pull resistor value is selected. */
                                                      kPORT_LowPullResistor,
                                                      /* Fast slew rate is configured */
                                                      kPORT_FastSlewRate,
                                                      /* Passive input filter is disabled */
                                                      kPORT_PassiveFilterDisable,
                                                      /* Open drain output is disabled */
                                                      kPORT_OpenDrainDisable,
                                                      /* Low drive strength is configured */
                                                      kPORT_LowDriveStrength,
                                                      /* Normal drive strength is configured */
                                                      kPORT_NormalDriveStrength,
                                                      /* Pin is configured as KPP_ROW_0 */
                                                      kPORT_MuxAlt3,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};

    PORT_SetPinConfig(AON__PORT0, 12U, &port0_12_config);

      const port_pin_config_t port0_7_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullDisable,
                                                      /* Low internal pull resistor value is selected. */
                                                      kPORT_LowPullResistor,
                                                      /* Fast slew rate is configured */
                                                      kPORT_FastSlewRate,
                                                      /* Passive input filter is disabled */
                                                      kPORT_PassiveFilterDisable,
                                                      /* Open drain output is disabled */
                                                      kPORT_OpenDrainEnable,
                                                      /* Low drive strength is configured */
                                                      kPORT_LowDriveStrength,
                                                      /* Normal drive strength is configured */
                                                      kPORT_NormalDriveStrength,
                                                      /* Pin is configured as KPP_COL_1 */
                                                      kPORT_MuxAlt3,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};

    PORT_SetPinConfig(AON__PORT0, 7U, &port0_7_config);

    const port_pin_config_t port0_6_config = {/* Internal pull-up/down resistor is disabled */
                                                      kPORT_PullDisable,
                                                      /* Low internal pull resistor value is selected. */
                                                      kPORT_LowPullResistor,
                                                      /* Fast slew rate is configured */
                                                      kPORT_FastSlewRate,
                                                      /* Passive input filter is disabled */
                                                      kPORT_PassiveFilterDisable,
                                                      /* Open drain output is disabled */
                                                      kPORT_OpenDrainEnable,
                                                      /* Low drive strength is configured */
                                                      kPORT_LowDriveStrength,
                                                      /* Normal drive strength is configured */
                                                      kPORT_NormalDriveStrength,
                                                      /* Pin is configured as KPP_COL_0 */
                                                      kPORT_MuxAlt3,
                                                      /* Digital input enabled */
                                                      kPORT_InputBufferEnable,
                                                      /* Digital input is not inverted */
                                                      kPORT_InputNormal,
                                                      /* Pin Control Register fields [15:0] are not locked */
                                                      kPORT_UnlockRegister};

    PORT_SetPinConfig(AON__PORT0, 6U, &port0_6_config);
}

void BOARD_InitAonLpadcPins(void)
{
    const port_pin_config_t port0_13_config = {/* Internal pull-up/down resistor is disabled */
                                                 kPORT_PullDisable,
                                                 /* Low internal pull resistor value is selected. */
                                                 kPORT_LowPullResistor,
                                                 /* Fast slew rate is configured */
                                                 kPORT_FastSlewRate,
                                                 /* Passive input filter is disabled */
                                                 kPORT_PassiveFilterDisable,
                                                 /* Open drain output is disabled */
                                                 kPORT_OpenDrainDisable,
                                                 /* Low drive strength is configured */
                                                 kPORT_LowDriveStrength,
                                                 /* Normal drive strength is configured */
                                                 kPORT_NormalDriveStrength,
                                                 /* Pin is configured as AON_LPADC0_A3 */
                                                 kPORT_MuxAlt0,
                                                 /* Digital input disabled; it is required for analog functions */
                                                 kPORT_InputBufferDisable,
                                                 /* Digital input is not inverted */
                                                 kPORT_InputNormal,
                                                 /* Pin Control Register fields [15:0] are not locked */
                                                 kPORT_UnlockRegister};
    /* PORT0_13 is configured as AON_LPADC0_A3 */
    PORT_SetPinConfig(AON__PORT0, 13U, &port0_13_config);
}
