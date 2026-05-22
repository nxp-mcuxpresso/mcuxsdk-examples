/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_xbar.h"
#include "fsl_tpm.h"
#include "t-format.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PWM_FREQUENCY (5U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
encoder_t_format_t encoder;
status_t status;
static bool cmdFlag = false;
flexio_t_format_handle_t g_tformatHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
void FLEXIO_T_Format_UserCallback(FLEXIO_T_FORMAT_Type *base,
       	                          flexio_t_format_handle_t *handle,
                                  status_t status, void *userData)
{
    userData = userData;

    if (kStatus_FLEXIO_T_FORMAT_TxIdle == status)
    {
    }

    if (kStatus_FLEXIO_T_FORMAT_RxIdle == status)
    {
        T_Format_CMD_Parse();
        cmdFlag = true;
    }
}

void tpm_init(void)
{
    tpm_config_t tpmInfo;
    tpm_chnl_pwm_signal_param_t tpmParam;
    int updatedDutycycle = 50;
    uint8_t control;

    TPM_GetDefaultConfig(&tpmInfo);
    tpmInfo.prescale = TPM_CalculateCounterClkDiv(BOARD_TPM_BASEADDR, DEMO_PWM_FREQUENCY, BOARD_TPM_SOURCE_CLOCK);
    TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);
    tpmParam.chnlNumber       = (tpm_chnl_t)BOARD_TPM_CHANNEL;
    tpmParam.level            = kTPM_HighTrue;
    tpmParam.dutyCyclePercent = updatedDutycycle;
    if (kStatus_Success != TPM_SetupPwm(BOARD_TPM_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, DEMO_PWM_FREQUENCY, BOARD_TPM_SOURCE_CLOCK))
    {
        PRINTF("\r\nSetup PWM fail!\r\n");
        return;
    }
    control = TPM_GetChannelControlBits(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL);
    TPM_EnableChannel(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, control);
    TPM_EnableInterrupts(BOARD_TPM_BASEADDR, BOARD_TPM_INTERRUPT_ENABLE);
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize T-format encoder */
    flexio_t_format_config_t devConfig;
    FLEXIO_T_FORMAT_Type encDev;
    t_format_all_info_t enc_abs = {0}, abs_save = {0};
    uint32_t clock;

    BOARD_InitHardware();
    clock = FLEXIO_CLOCK_FREQUENCY / 1000000;

    XBAR_Init(kXBAR_DSC1);
    XBAR_SetSignalsConnection(kXBAR1_InputTpm6LptpmChTrigger0, kXBAR1_OutputFlexio1FlexioTriggerIn0);
    PRINTF("Encoder T-format example (Sync mode)\r\n");
    PRINTF("FlexIO Root Clock is %d MHz\r\n", clock);

    tpm_init();

    /* 
     * Config->enableT_Format   = true;
     * Config->enableInDoze     = false;
     * Config->enableInDebug    = true;
     * Config->enableFastAccess = false;
     * Config->baudRate_bps     = kFLEXIO_A_FORMAT_2_5MHZ;
     * Config->userMode         = kFLEXIO_A_FORMAT_USERMODE_ONESHOT;
     */
    FLEXIO_T_Format_GetDefaultConfig(&devConfig);
    devConfig.userMode = kFLEXIO_T_FORMAT_USERMODE_SYNC;

    encDev.flexioBase      = BOARD_FLEXIO_BASE;
    encDev.TxPinIndex      = FLEXIO_T_FORMAT_TX_PIN;
    encDev.RxPinIndex      = FLEXIO_T_FORMAT_RX_PIN;
    encDev.DRPinIndex      = FLEXIO_T_FORMAT_DR_PIN;
    encDev.shifterIndex[0] = T_FORMAT_TX_SHIFTER_INDEX;
    encDev.shifterIndex[1] = T_FORMAT_RX_SHIFTER_INDEX;
    encDev.timerIndex[0]   = T_FORMAT_TX_TIMER_INDEX;
    encDev.timerIndex[1]   = T_FORMAT_RX_TIMER_INDEX;
    encDev.timerIndex[2]   = T_FORMAT_DR_TIMER_INDEX;
    encDev.triggerIn       = 0;

    encoder.controller = &encDev;
    encoder.singleTurnRevolution  = 17;
    encoder.multiTunrRevolution   = 16;
    encoder.single_turn_sign_mask = 0x0001FFFF;
    encoder.multi_turn_sign_mask  = 0x0000FFFF;

    status = FLEXIO_T_Format_Init(&encDev, &devConfig, FLEXIO_CLOCK_FREQUENCY);
    if (status != kStatus_Success)
    {
        return -1;
    }

    FLEXIO_T_Format_TransferCreateHandle(&encDev, &g_tformatHandle, FLEXIO_T_Format_UserCallback, NULL);
    T_Format_Readout_ABS_ABM_IRQ(&encoder, &enc_abs);

    TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);
    while (1)
    {
        if (cmdFlag)
        {
            if ((abs(enc_abs.singleTurn - abs_save.singleTurn) > 500))
            {
                PRINTF("Multi-turn data: %d, single-turn data: %ld\r\n", enc_abs.multiTurn, enc_abs.singleTurn);
                abs_save = enc_abs;
            }
            cmdFlag = false;
            T_Format_Readout_ABS_ABM_IRQ(&encoder, &enc_abs);
        }
    }
}
