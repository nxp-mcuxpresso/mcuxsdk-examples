/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v4.0
processor: LPC51U68
package_id: LPC51U68JBD64
mcu_data: ksdk2_0
processor_version: 0.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iocon.h"
#include "fsl_inputmux.h"
#include "pin_mux.h"

/*FUNCTION**********************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 *END**************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();
}

#define IOCON_PIO_DIGITAL_EN          0x80u   /*!< Enables digital function */
#define IOCON_PIO_FUNC0               0x00u   /*!< Selects pin function 0 */
#define IOCON_PIO_FUNC1               0x01u   /*!< Selects pin function 1 */
#define IOCON_PIO_INPFILT_OFF       0x0100u   /*!< Input filter disabled */
#define IOCON_PIO_INV_DI              0x00u   /*!< Input function is not inverted */
#define IOCON_PIO_MODE_INACT          0x00u   /*!< No addition pin function */
#define IOCON_PIO_OPENDRAIN_DI        0x00u   /*!< Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD       0x00u   /*!< Standard mode, output slew rate control is enabled */
#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port 0 */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port 0 */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port 1 */
#define PORT0_IDX                        0u   /*!< Port index */
#define PORT1_IDX                        1u   /*!< Port index */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '31', peripheral: FLEXCOMM0, signal: RXD_SDA_MOSI, pin_signal: PIO0_0/FC0_RXD_SDA_MOSI/FC3_CTS_SDA_SSEL0/CTIMER0_CAP0/SCT0_OUT3, mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: '32', peripheral: FLEXCOMM0, signal: TXD_SCL_MISO, pin_signal: PIO0_1/FC0_TXD_SCL_MISO/FC3_RTS_SCL_SSEL1/CTIMER0_CAP1/SCT0_OUT1, mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: '62', peripheral: GPIO, signal: 'PIO1, 15', pin_signal: PIO1_15/SCT0_OUT5/CTIMER1_CAP3/FC7_CTS_SDA_SSEL0, mode: inactive, invert: disabled, glitch_filter: disabled,
    slew_rate: standard, open_drain: disabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) { /* Function assigned for the Cortex-M0P */
  CLOCK_EnableClock(kCLOCK_Iocon);                           /* Enables the clock for the IOCON block. 0 = Disable; 1 = Enable.: 0x01u */

  const uint32_t port0_pin0_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as FC0_RXD_SDA_MOSI */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN0_IDX, port0_pin0_config); /* PORT0 PIN0 (coords: 31) is configured as FC0_RXD_SDA_MOSI */
  const uint32_t port0_pin1_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as FC0_TXD_SCL_MISO */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN1_IDX, port0_pin1_config); /* PORT0 PIN1 (coords: 32) is configured as FC0_TXD_SCL_MISO */
  const uint32_t port1_pin15_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO1_15 */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN15_IDX, port1_pin15_config); /* PORT1 PIN15 (coords: 62) is configured as PIO1_15 */
}


#define IOCON_PIO_DIGITAL_EN          0x80u   /*!< Enables digital function */
#define IOCON_PIO_FUNC1               0x01u   /*!< Selects pin function 1 */
#define IOCON_PIO_I2CDRIVE_LOW        0x00u   /*!< Low drive: 4 mA */
#define IOCON_PIO_I2CFILTER_EN        0x00u   /*!< I2C 50 ns glitch filter enabled */
#define IOCON_PIO_I2CSLEW_I2C         0x00u   /*!< I2C mode */
#define IOCON_PIO_INPFILT_OFF       0x0100u   /*!< Input filter disabled */
#define IOCON_PIO_INV_DI              0x00u   /*!< Input function is not inverted */
#define PIN25_IDX                       25u   /*!< Pin number for pin 25 in a port 0 */
#define PIN26_IDX                       26u   /*!< Pin number for pin 26 in a port 0 */
#define PORT0_IDX                        0u   /*!< Port index */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C4_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '4', peripheral: FLEXCOMM4, signal: CTS_SDA_SSEL0, pin_signal: PIO0_26/FC4_CTS_SDA_SSEL0/CTIMER0_CAP3, invert: disabled, glitch_filter: disabled, i2c_slew: i2c,
    i2c_drive: low, i2c_filter: enabled}
  - {pin_num: '3', peripheral: FLEXCOMM4, signal: RTS_SCL_SSEL1, pin_signal: PIO0_25/FC4_RTS_SCL_SSEL1/FC6_CTS_SDA_SSEL0/CTIMER0_CAP2/CTIMER1_CAP1, invert: disabled,
    glitch_filter: disabled, i2c_slew: i2c, i2c_drive: low, i2c_filter: enabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C4_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C4_InitPins(void) { /* Function assigned for the Cortex-M0P */
  CLOCK_EnableClock(kCLOCK_Iocon);                           /* Enables the clock for the IOCON block. 0 = Disable; 1 = Enable.: 0x01u */

  const uint32_t port0_pin25_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as FC4_RTS_SCL_SSEL1 */
    IOCON_PIO_I2CSLEW_I2C |                                  /* I2C mode */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_I2CDRIVE_LOW |                                 /* Low drive: 4 mA */
    IOCON_PIO_I2CFILTER_EN                                   /* I2C 50 ns glitch filter enabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN25_IDX, port0_pin25_config); /* PORT0 PIN25 (coords: 3) is configured as FC4_RTS_SCL_SSEL1 */
  const uint32_t port0_pin26_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as FC4_CTS_SDA_SSEL0 */
    IOCON_PIO_I2CSLEW_I2C |                                  /* I2C mode */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_I2CDRIVE_LOW |                                 /* Low drive: 4 mA */
    IOCON_PIO_I2CFILTER_EN                                   /* I2C 50 ns glitch filter enabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN26_IDX, port0_pin26_config); /* PORT0 PIN26 (coords: 4) is configured as FC4_CTS_SDA_SSEL0 */
}


#define PIO025_DIGIMODE_DIGITAL       0x01u   /*!< Select Analog/Digital mode.: Digital mode. */
#define PIO025_FUNC_ALT0              0x00u   /*!< Selects pin function.: Alternative connection 0. */
#define PIO026_DIGIMODE_DIGITAL       0x01u   /*!< Select Analog/Digital mode.: Digital mode. */
#define PIO026_FUNC_ALT0              0x00u   /*!< Selects pin function.: Alternative connection 0. */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C4_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '4', peripheral: GPIO, signal: 'PIO0, 26', pin_signal: PIO0_26/FC4_CTS_SDA_SSEL0/CTIMER0_CAP3}
  - {pin_num: '3', peripheral: GPIO, signal: 'PIO0, 25', pin_signal: PIO0_25/FC4_RTS_SCL_SSEL1/FC6_CTS_SDA_SSEL0/CTIMER0_CAP2/CTIMER1_CAP1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : I2C4_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void I2C4_DeinitPins(void) { /* Function assigned for the Cortex-M0P */
  CLOCK_EnableClock(kCLOCK_Iocon);                           /* Enables the clock for the IOCON block. 0 = Disable; 1 = Enable.: 0x01u */

  IOCON->PIO[0][25] = ((IOCON->PIO[0][25] &
    (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))      /* Mask bits to zero which are setting */
      | IOCON_PIO_FUNC(PIO025_FUNC_ALT0)                     /* Selects pin function.: PORT025 (pin 3) is configured as PIO0_25 */
      | IOCON_PIO_DIGIMODE(PIO025_DIGIMODE_DIGITAL)          /* Select Analog/Digital mode.: Digital mode. */
    );
  IOCON->PIO[0][26] = ((IOCON->PIO[0][26] &
    (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))      /* Mask bits to zero which are setting */
      | IOCON_PIO_FUNC(PIO026_FUNC_ALT0)                     /* Selects pin function.: PORT026 (pin 4) is configured as PIO0_26 */
      | IOCON_PIO_DIGIMODE(PIO026_DIGIMODE_DIGITAL)          /* Select Analog/Digital mode.: Digital mode. */
    );
}


#define IOCON_PIO_DIGITAL_EN          0x80u   /*!< Enables digital function */
#define IOCON_PIO_FUNC1               0x01u   /*!< Selects pin function 1 */
#define IOCON_PIO_FUNC4               0x04u   /*!< Selects pin function 4 */
#define IOCON_PIO_INPFILT_OFF       0x0100u   /*!< Input filter disabled */
#define IOCON_PIO_INV_DI              0x00u   /*!< Input function is not inverted */
#define IOCON_PIO_MODE_PULLUP         0x10u   /*!< Selects pull-up function */
#define IOCON_PIO_OPENDRAIN_DI        0x00u   /*!< Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD       0x00u   /*!< Standard mode, output slew rate control is enabled */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port 1 */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port 0 */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port 0 */
#define PIN20_IDX                       20u   /*!< Pin number for pin 20 in a port 0 */
#define PORT0_IDX                        0u   /*!< Port index */
#define PORT1_IDX                        1u   /*!< Port index */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
SPI5_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '15', peripheral: FLEXCOMM5, signal: SSEL2, pin_signal: PIO1_1/SCT0_OUT4/FC5_SSEL2/FC4_TXD_SCL_MISO/ADC0_4, mode: pullUp, invert: disabled, glitch_filter: disabled,
    open_drain: disabled}
  - {pin_num: '58', peripheral: FLEXCOMM5, signal: TXD_SCL_MISO, pin_signal: PIO0_18/FC5_TXD_SCL_MISO/SCT0_OUT0/CTIMER0_MAT0, mode: pullUp, invert: disabled, glitch_filter: disabled,
    slew_rate: standard, open_drain: disabled}
  - {pin_num: '59', peripheral: FLEXCOMM5, signal: SCK, pin_signal: PIO0_19/FC5_SCK/SCT0_OUT1/CTIMER0_MAT1, mode: pullUp, invert: disabled, glitch_filter: disabled,
    slew_rate: standard, open_drain: disabled}
  - {pin_num: '60', peripheral: FLEXCOMM5, signal: RXD_SDA_MOSI, pin_signal: PIO0_20/FC5_RXD_SDA_MOSI/FC0_SCK/CTIMER3_CAP0, mode: pullUp, invert: disabled, glitch_filter: disabled,
    slew_rate: standard, open_drain: disabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI5_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void SPI5_InitPins(void) { /* Function assigned for the Cortex-M0P */
  CLOCK_EnableClock(kCLOCK_Iocon);                           /* Enables the clock for the IOCON block. 0 = Disable; 1 = Enable.: 0x01u */

  const uint32_t port0_pin18_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as FC5_TXD_SCL_MISO */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN18_IDX, port0_pin18_config); /* PORT0 PIN18 (coords: 58) is configured as FC5_TXD_SCL_MISO */
  const uint32_t port0_pin19_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as FC5_SCK */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN19_IDX, port0_pin19_config); /* PORT0 PIN19 (coords: 59) is configured as FC5_SCK */
  const uint32_t port0_pin20_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as FC5_RXD_SDA_MOSI */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN20_IDX, port0_pin20_config); /* PORT0 PIN20 (coords: 60) is configured as FC5_RXD_SDA_MOSI */
  const uint32_t port1_pin1_config = (
    IOCON_PIO_FUNC4 |                                        /* Pin is configured as FC5_SSEL2 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN1_IDX, port1_pin1_config); /* PORT1 PIN1 (coords: 15) is configured as FC5_SSEL2 */
}


#define DMA_ITRIG_INMUX10_IDX           10u   /*!< Inputmux DMA_ITRIG_INMUX register index 10 */
#define DMA_ITRIG_INMUX11_IDX           11u   /*!< Inputmux DMA_ITRIG_INMUX register index 11 */
#define DMA_ITRIG_INMUX12_IDX           12u   /*!< Inputmux DMA_ITRIG_INMUX register index 12 */
#define DMA_ITRIG_INMUX13_IDX           13u   /*!< Inputmux DMA_ITRIG_INMUX register index 13 */
#define PINTSEL0_IDX                     0u   /*!< Inputmux PINTSEL register index 0 */
#define PINTSEL1_IDX                     1u   /*!< Inputmux PINTSEL register index 1 */
#define PINTSEL2_IDX                     2u   /*!< Inputmux PINTSEL register index 2 */
#define PINTSEL3_IDX                     3u   /*!< Inputmux PINTSEL register index 3 */
#define PIO018_DIGIMODE_DIGITAL       0x01u   /*!< Select Analog/Digital mode.: Digital mode. */
#define PIO018_FUNC_ALT0              0x00u   /*!< Selects pin function.: Alternative connection 0. */
#define PIO019_DIGIMODE_DIGITAL       0x01u   /*!< Select Analog/Digital mode.: Digital mode. */
#define PIO019_FUNC_ALT0              0x00u   /*!< Selects pin function.: Alternative connection 0. */
#define PIO020_DIGIMODE_DIGITAL       0x01u   /*!< Select Analog/Digital mode.: Digital mode. */
#define PIO020_FUNC_ALT0              0x00u   /*!< Selects pin function.: Alternative connection 0. */
#define PIO11_DIGIMODE_DIGITAL        0x01u   /*!< Select Analog/Digital mode.: Digital mode. */
#define PIO11_FUNC_ALT0               0x00u   /*!< Selects pin function.: Alternative connection 0. */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
SPI5_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '15', peripheral: DMA0, signal: 'TRIG, 10', pin_signal: PIO1_1/SCT0_OUT4/FC5_SSEL2/FC4_TXD_SCL_MISO/ADC0_4, mode: no_init, invert: no_init, glitch_filter: no_init,
    open_drain: no_init}
  - {pin_num: '58', peripheral: DMA0, signal: 'TRIG, 11', pin_signal: PIO0_18/FC5_TXD_SCL_MISO/SCT0_OUT0/CTIMER0_MAT0, mode: no_init, invert: no_init, glitch_filter: no_init,
    slew_rate: no_init, open_drain: no_init}
  - {pin_num: '59', peripheral: DMA0, signal: 'TRIG, 12', pin_signal: PIO0_19/FC5_SCK/SCT0_OUT1/CTIMER0_MAT1, mode: no_init, invert: no_init, glitch_filter: no_init,
    slew_rate: no_init, open_drain: no_init}
  - {pin_num: '60', peripheral: DMA0, signal: 'TRIG, 13', pin_signal: PIO0_20/FC5_RXD_SDA_MOSI/FC0_SCK/CTIMER3_CAP0, mode: no_init, invert: no_init, glitch_filter: no_init,
    slew_rate: no_init, open_drain: no_init}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI5_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void SPI5_DeinitPins(void) { /* Function assigned for the Cortex-M0P */
  CLOCK_EnableClock(kCLOCK_InputMux);                        /* Enables the clock for the input muxes. 0 = Disable; 1 = Enable.: 0x01u */
  CLOCK_EnableClock(kCLOCK_Iocon);                           /* Enables the clock for the IOCON block. 0 = Disable; 1 = Enable.: 0x01u */

  INPUTMUX_AttachSignal(INPUTMUX,DMA_ITRIG_INMUX10_IDX,kINPUTMUX_PinInt1ToDma); /* Pin interrupt 1 is selected as trigger input for DMA channel 10 */
  INPUTMUX_AttachSignal(INPUTMUX,DMA_ITRIG_INMUX11_IDX,kINPUTMUX_PinInt2ToDma); /* Pin interrupt 2 is selected as trigger input for DMA channel 11 */
  INPUTMUX_AttachSignal(INPUTMUX,DMA_ITRIG_INMUX12_IDX,kINPUTMUX_PinInt3ToDma); /* Pin interrupt 3 is selected as trigger input for DMA channel 12 */
  INPUTMUX_AttachSignal(INPUTMUX,DMA_ITRIG_INMUX13_IDX,kINPUTMUX_PinInt0ToDma); /* Pin interrupt 0 is selected as trigger input for DMA channel 13 */
  INPUTMUX_AttachSignal(INPUTMUX,PINTSEL0_IDX,kINPUTMUX_GpioPort0Pin20ToPintsel); /* PIO0_20 is selected for PINT input 0 */
  INPUTMUX_AttachSignal(INPUTMUX,PINTSEL1_IDX,kINPUTMUX_GpioPort1Pin1ToPintsel); /* PIO1_1 is selected for PINT input 1 */
  INPUTMUX_AttachSignal(INPUTMUX,PINTSEL2_IDX,kINPUTMUX_GpioPort0Pin18ToPintsel); /* PIO0_18 is selected for PINT input 2 */
  INPUTMUX_AttachSignal(INPUTMUX,PINTSEL3_IDX,kINPUTMUX_GpioPort0Pin19ToPintsel); /* PIO0_19 is selected for PINT input 3 */
  IOCON->PIO[0][18] = ((IOCON->PIO[0][18] &
    (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))      /* Mask bits to zero which are setting */
      | IOCON_PIO_FUNC(PIO018_FUNC_ALT0)                     /* Selects pin function.: PORT018 (pin 58) is configured as PIO0_18 */
      | IOCON_PIO_DIGIMODE(PIO018_DIGIMODE_DIGITAL)          /* Select Analog/Digital mode.: Digital mode. */
    );
  IOCON->PIO[0][19] = ((IOCON->PIO[0][19] &
    (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))      /* Mask bits to zero which are setting */
      | IOCON_PIO_FUNC(PIO019_FUNC_ALT0)                     /* Selects pin function.: PORT019 (pin 59) is configured as PIO0_19 */
      | IOCON_PIO_DIGIMODE(PIO019_DIGIMODE_DIGITAL)          /* Select Analog/Digital mode.: Digital mode. */
    );
  IOCON->PIO[0][20] = ((IOCON->PIO[0][20] &
    (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))      /* Mask bits to zero which are setting */
      | IOCON_PIO_FUNC(PIO020_FUNC_ALT0)                     /* Selects pin function.: PORT020 (pin 60) is configured as PIO0_20 */
      | IOCON_PIO_DIGIMODE(PIO020_DIGIMODE_DIGITAL)          /* Select Analog/Digital mode.: Digital mode. */
    );
  IOCON->PIO[1][1] = ((IOCON->PIO[1][1] &
    (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_DIGIMODE_MASK)))      /* Mask bits to zero which are setting */
      | IOCON_PIO_FUNC(PIO11_FUNC_ALT0)                      /* Selects pin function.: PORT11 (pin 15) is configured as PIO1_1 */
      | IOCON_PIO_DIGIMODE(PIO11_DIGIMODE_DIGITAL)           /* Select Analog/Digital mode.: Digital mode. */
    );
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
