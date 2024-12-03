/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "haptic_pwm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Input audio sampling rate */
#define HAPTIC_PWM_FTM_AUDIO_FS ( HAPTIC_FS )

/* PWM output sampling rate: N * AUDIO_FS: 32kHz */
#define HAPTIC_PWM_FTM_INPUT_TO_OUTPUT_N (2)
#define HAPTIC_PWM_FTM_OUTPUT_FS ( HAPTIC_FS*HAPTIC_PWM_FTM_INPUT_TO_OUTPUT_N )

/* Silence detector definition: Enable/Disable output during silence */
#define HAPTIC_PWM_SILENCE_DETECTOR_ON
#define HAPTIC_PWM_SILENCE_DETECTION_COUNTER ( (size_t) ( 0.05 * HAPTIC_PWM_FTM_AUDIO_FS ) )

/* Flextimer clock source */
#define HAPTIC_PWM_FTM_CLOCK kCLOCK_CoreSysClk

/* The Flextimer used for the PWM output - motor 1 */
#define HAPTIC_PWM_FTM_MOTOR1_HIGH                    (FTM1)
#define HAPTIC_PWM_FTM_MOTOR1_LOW                     (FTM0)
#define HAPTIC_PWM_CHANNEL_MOTOR1_HIGH                (kFTM_Chnl_2)
#define HAPTIC_PWM_CHANNEL_MOTOR1_LOW                 (kFTM_Chnl_6)

/* The Flextimer used for the PWM output - motor 2 */
#define HAPTIC_PWM_FTM_MOTOR2_HIGH                    (FTM1)
#define HAPTIC_PWM_FTM_MOTOR2_LOW                     (FTM2)
#define HAPTIC_PWM_CHANNEL_MOTOR2_HIGH                (kFTM_Chnl_3)
#define HAPTIC_PWM_CHANNEL_MOTOR2_LOW                 (kFTM_Chnl_0)
/* Audio interrupt setting */
#define HAPTIC_PWM_FTM_AUDIO                          (FTM1)
#define HAPTIC_PWM_FTM_AUDIO_IRQ_N                    (FTM1_IRQn)
#define HAPTIC_PWM_FTM_AUDIO_IRQ_HANDLER              (FTM1_IRQHandler)
#define HAPTIC_PWM_FTM_AUDIO_CHANNEL_INTERRUPT_ENABLE (kFTM_TimeOverflowInterruptEnable)
#define HAPTIC_PWM_FTM_AUDIO_CHANNEL_FLAG             (kFTM_TimeOverflowFlag)

/* Playback queue size to mimic I2S module behavior */
#define HAPTIC_PWM_QUEUE_SIZE (2)

/*******************************************************************************
 * Types
 ******************************************************************************/
typedef struct _haptic_pwm_transfer
{
    int16_t *data;   /*!< Data buffer start pointer */
    size_t dataSize; /*!< Bytes to be transferred. */
} haptic_pwm_transfer;

typedef struct _haptic_pwm_state_t
{
    haptic_pwm_transfer playQueue[HAPTIC_PWM_QUEUE_SIZE];
    haptic_callback_t pwmCallbackFun;
    size_t cSmp;
    size_t cZer;
    size_t callbackCounter;
    size_t cQueueUser;
    size_t cQueueDriver;
    uint32_t pwmVec[4];
} haptic_pwm_state_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

const ftm_chnl_pwm_signal_param_t pwm_ftm_channel_motor1[2] =
{
    {
        .chnlNumber            = HAPTIC_PWM_CHANNEL_MOTOR1_HIGH, /*!< Channel L positive setup*/
        .level                 = kFTM_HighTrue,                  /*!< PWM output active-high.*/
        .dutyCyclePercent      = 0U,                             /*!< '1' pulse for 0%*/
        .firstEdgeDelayPercent = 0U,                             /*!< Symmetrical PWM */
    },
    {
        .chnlNumber            = HAPTIC_PWM_CHANNEL_MOTOR1_LOW, /*!< Channel L negative setup*/
        .level                 = kFTM_LowTrue,                  /*!< PWM output active-low. */
        .dutyCyclePercent      = 100U,                          /*!< '0' pulse for 100%*/
        .firstEdgeDelayPercent = 0U,                            /*!< Symmetrical PWM */
    },
};

const ftm_chnl_pwm_signal_param_t pwm_ftm_channel_motor2[2] =
{
    {
        .chnlNumber            = HAPTIC_PWM_CHANNEL_MOTOR2_HIGH, /*!< Channel L positive setup*/
        .level                 = kFTM_HighTrue,                  /*!< PWM output active-high.*/
        .dutyCyclePercent      = 0U,                             /*!< '1' pulse for 0%*/
        .firstEdgeDelayPercent = 0U,                             /*!< Symmetrical PWM */
    },
    {
        .chnlNumber            = HAPTIC_PWM_CHANNEL_MOTOR2_LOW, /*!< Channel L negative setup*/
        .level                 = kFTM_LowTrue,                  /*!< PWM output active-high. */
        .dutyCyclePercent      = 100U,                          /*!< '0' pulse for 100%*/
        .firstEdgeDelayPercent = 0U,                            /*!< Symmetrical PWM */
    }
};


/* Haptic PWM state */
static volatile haptic_pwm_state_t hapticPwmState = { 0 };
static volatile haptic_pwm_state_t* pS = &hapticPwmState;
static volatile int8_t audio_fs_to_pwm_fs_counter = 0;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Private functions
 ******************************************************************************/

uint32_t int16_to_pwm_h_bridge( int16_t x16, uint32_t pwmMod )
{
    uint32_t ret = 0;
    /* clip abs */
    x16 = (x16 == -32768) ? -32767 : x16;
    /* Conv [(-32768) 32767 ] to [0 65535] */
    x16 = ( x16 < 0 ) ? ( -(x16) ) : (x16);
    /* Scale [0 32768] to [ 0 pwmMod ] -> [0 - 100%] */
    ret = ( ((uint32_t)x16) * pwmMod + ( 1 << 14 ) ) >> 15;
    return ret;
}

void haptic_pwm_audio_to_pwm( )
{
    if( pS->playQueue[pS->cQueueDriver].dataSize > 0 )
    {
        int16_t xL = pS->playQueue[pS->cQueueDriver].data[pS->cSmp+0];
        int16_t xR = pS->playQueue[pS->cQueueDriver].data[pS->cSmp+1];

        uint32_t mod50L  = (HAPTIC_PWM_FTM_MOTOR1_LOW ->MOD + 1) >> 1;
        uint32_t mod50R  = (HAPTIC_PWM_FTM_MOTOR2_HIGH->MOD + 1) >> 1;
        uint32_t modAddL = int16_to_pwm_h_bridge( xL, mod50L );
        uint32_t modAddR = int16_to_pwm_h_bridge( xR, mod50R );

        /* Set Duty as 50% +/- 50% */
        uint32_t modL = mod50L + modAddL;
        uint32_t modR = mod50R + modAddR;
        if( xL < 0 ) { modL = mod50L - modAddL; }
        if( xR < 0 ) { modR = mod50R - modAddR; }

        pS->pwmVec[0] = modL;
        pS->pwmVec[1] = modL;
        pS->pwmVec[2] = modR;
        pS->pwmVec[3] = modR;


        /* Increment sample counter for all channels */
        pS->cSmp += 2;
        /* Callback and reset */
        if( pS->cSmp >= pS->playQueue[pS->cQueueDriver].dataSize )
        {
            pS->cSmp = 0;
            pS->cQueueDriver = (pS->cQueueDriver + 1) % HAPTIC_PWM_QUEUE_SIZE;
            /*pS->pwmCallbackFun(); */
            pS->callbackCounter = 1;
        }

        /* Dummy silence detector */
        if( xL == 0 && xR == 0 )
        {
            if( pS->cZer > HAPTIC_PWM_SILENCE_DETECTION_COUNTER )
            {
                pS->cZer = HAPTIC_PWM_SILENCE_DETECTION_COUNTER;

#if( defined(HAPTIC_PWM_SILENCE_DETECTOR_ON) )
                /* '1' for 0%   */
                pS->pwmVec[0] = 0;
                /* '0' for 100% */
                pS->pwmVec[1] = (HAPTIC_PWM_FTM_MOTOR1_HIGH->MOD + 1);
                /* '1' for 0%   */
                pS->pwmVec[2] = 0;
                /* '0' for 100% */
                pS->pwmVec[3] = (HAPTIC_PWM_FTM_MOTOR2_LOW->MOD + 1);
#endif
            }
            pS->cZer += 1;
        }else
        {
            pS->cZer = 0;
        }
    }
}

/* Convert audio data into PWM duty cycle and data into PWM output register.
 */
void HAPTIC_PWM_FTM_AUDIO_IRQ_HANDLER(void)
{
    uint32_t statusFlag = FTM_GetStatusFlags(HAPTIC_PWM_FTM_AUDIO);
    /* Audio FS flag */
    if( (statusFlag & HAPTIC_PWM_FTM_AUDIO_CHANNEL_FLAG) == HAPTIC_PWM_FTM_AUDIO_CHANNEL_FLAG)
    {
        audio_fs_to_pwm_fs_counter--;
        if (audio_fs_to_pwm_fs_counter <= 0 )
        {
           /* each Nth interrupts go here to execute the code */
            audio_fs_to_pwm_fs_counter = HAPTIC_PWM_FTM_INPUT_TO_OUTPUT_N;
            /* Get PWM duty cycle */
            haptic_pwm_audio_to_pwm( );
            if(    HAPTIC_PWM_FTM_MOTOR1_HIGH->CONTROLS[HAPTIC_PWM_CHANNEL_MOTOR1_HIGH].CnV  != pS->pwmVec[0]
                || HAPTIC_PWM_FTM_MOTOR1_LOW ->CONTROLS[HAPTIC_PWM_CHANNEL_MOTOR1_LOW ].CnV  != pS->pwmVec[1]
                || HAPTIC_PWM_FTM_MOTOR2_HIGH->CONTROLS[HAPTIC_PWM_CHANNEL_MOTOR2_HIGH].CnV != pS->pwmVec[2]
                || HAPTIC_PWM_FTM_MOTOR2_LOW ->CONTROLS[HAPTIC_PWM_CHANNEL_MOTOR2_LOW ].CnV != pS->pwmVec[3]
            )
            {
                /* Copy new samples to the output */
                HAPTIC_PWM_FTM_MOTOR1_HIGH->CONTROLS[HAPTIC_PWM_CHANNEL_MOTOR1_HIGH].CnV  = pS->pwmVec[0];
                HAPTIC_PWM_FTM_MOTOR1_LOW ->CONTROLS[HAPTIC_PWM_CHANNEL_MOTOR1_LOW ].CnV  = pS->pwmVec[1];
                HAPTIC_PWM_FTM_MOTOR2_HIGH->CONTROLS[HAPTIC_PWM_CHANNEL_MOTOR2_HIGH].CnV = pS->pwmVec[2];
                HAPTIC_PWM_FTM_MOTOR2_LOW ->CONTROLS[HAPTIC_PWM_CHANNEL_MOTOR2_LOW ].CnV = pS->pwmVec[3];

                HAPTIC_PWM_FTM_MOTOR1_HIGH->PWMLOAD = 0x200;  /*set LDOK bit */
                HAPTIC_PWM_FTM_MOTOR1_LOW ->PWMLOAD = 0x200;  /*set LDOK bit */
                HAPTIC_PWM_FTM_MOTOR2_HIGH->PWMLOAD = 0x200;  /*set LDOK bit */
                HAPTIC_PWM_FTM_MOTOR2_LOW ->PWMLOAD = 0x200;  /*set LDOK bit */
            }
        }
        /* Clear interrupt flag.*/
        FTM_ClearStatusFlags(HAPTIC_PWM_FTM_AUDIO, HAPTIC_PWM_FTM_AUDIO_CHANNEL_FLAG);
     }
    __DSB();
}

/*******************************************************************************
 * Public functions
 ******************************************************************************/

void haptic_pwm_send( uint8_t *pData, size_t dataBytes )
{
    pS->playQueue[ pS->cQueueUser ].data = (int16_t*) pData;
    pS->playQueue[ pS->cQueueUser ].dataSize = dataBytes >> 1;
    pS->cQueueUser = (pS->cQueueUser + 1) % HAPTIC_PWM_QUEUE_SIZE;
}

void haptic_pwm_init( haptic_callback_t callback )
{
    /* Set global callback function */
    pS->pwmCallbackFun = callback;

    /* Init FTM timers for motor control */
    ftm_config_t ftm_config;
    FTM_GetDefaultConfig(&ftm_config);
    ftm_config.useGlobalTimeBase = true;
    ftm_config.reloadPoints = kFTM_CntMax;
    ftm_config.swTriggerResetCount = false;
    FTM_Init(HAPTIC_PWM_FTM_MOTOR1_HIGH, &ftm_config);
    FTM_Init(HAPTIC_PWM_FTM_MOTOR1_LOW,  &ftm_config);
    FTM_Init(HAPTIC_PWM_FTM_MOTOR2_HIGH, &ftm_config);
    FTM_Init(HAPTIC_PWM_FTM_MOTOR2_LOW,  &ftm_config);
    /* Configure FTM channels */
    FTM_SetupPwm(HAPTIC_PWM_FTM_MOTOR1_HIGH, &pwm_ftm_channel_motor1[0], 1U, kFTM_EdgeAlignedPwm, HAPTIC_PWM_FTM_OUTPUT_FS, CLOCK_GetFreq(HAPTIC_PWM_FTM_CLOCK));
    FTM_SetupPwm(HAPTIC_PWM_FTM_MOTOR1_LOW , &pwm_ftm_channel_motor1[1], 1U, kFTM_EdgeAlignedPwm, HAPTIC_PWM_FTM_OUTPUT_FS, CLOCK_GetFreq(HAPTIC_PWM_FTM_CLOCK));
    FTM_SetupPwm(HAPTIC_PWM_FTM_MOTOR2_HIGH, &pwm_ftm_channel_motor2[0],  1U, kFTM_EdgeAlignedPwm, HAPTIC_PWM_FTM_OUTPUT_FS, CLOCK_GetFreq(HAPTIC_PWM_FTM_CLOCK));
    FTM_SetupPwm(HAPTIC_PWM_FTM_MOTOR2_LOW , &pwm_ftm_channel_motor2[1],  1U, kFTM_EdgeAlignedPwm, HAPTIC_PWM_FTM_OUTPUT_FS, CLOCK_GetFreq(HAPTIC_PWM_FTM_CLOCK));
    HAPTIC_PWM_FTM_MOTOR2_HIGH->CONTROLS[HAPTIC_PWM_CHANNEL_MOTOR2_HIGH].CnV = (HAPTIC_PWM_FTM_MOTOR2_HIGH->MOD + 1) >> 1;
    HAPTIC_PWM_FTM_MOTOR2_LOW ->CONTROLS[HAPTIC_PWM_CHANNEL_MOTOR2_LOW ].CnV = (HAPTIC_PWM_FTM_MOTOR2_LOW ->MOD + 1) >> 1;

    /* Enable channels interrupt flag.*/
    FTM_EnableInterrupts(HAPTIC_PWM_FTM_AUDIO, HAPTIC_PWM_FTM_AUDIO_CHANNEL_INTERRUPT_ENABLE);
    /* Enable at the NVIC */
    EnableIRQ(HAPTIC_PWM_FTM_AUDIO_IRQ_N);
    /* Set the highest priority */
    NVIC_SetPriority( HAPTIC_PWM_FTM_AUDIO_IRQ_N, 0 );

    /* Set initial values */
    pS->pwmVec[0] = HAPTIC_PWM_FTM_MOTOR1_HIGH->CONTROLS[HAPTIC_PWM_CHANNEL_MOTOR1_HIGH].CnV;
    pS->pwmVec[1] = HAPTIC_PWM_FTM_MOTOR1_LOW ->CONTROLS[HAPTIC_PWM_CHANNEL_MOTOR1_LOW ].CnV;
    pS->pwmVec[2] = HAPTIC_PWM_FTM_MOTOR2_HIGH->CONTROLS[HAPTIC_PWM_CHANNEL_MOTOR2_HIGH].CnV;
    pS->pwmVec[3] = HAPTIC_PWM_FTM_MOTOR2_LOW ->CONTROLS[HAPTIC_PWM_CHANNEL_MOTOR2_LOW ].CnV;

    /* Start FTM output and audio timers */
    FTM_StartTimer(HAPTIC_PWM_FTM_MOTOR1_HIGH, kFTM_SystemClock);
    FTM_StartTimer(HAPTIC_PWM_FTM_MOTOR1_LOW , kFTM_SystemClock);
    FTM_StartTimer(HAPTIC_PWM_FTM_MOTOR2_HIGH, kFTM_SystemClock);
    FTM_StartTimer(HAPTIC_PWM_FTM_MOTOR2_LOW , kFTM_SystemClock);

    /* Software triggers to update PWM registers */
    HAPTIC_PWM_FTM_MOTOR1_HIGH->PWMLOAD = 0x200;  /*set LDOK bit */
    HAPTIC_PWM_FTM_MOTOR1_LOW ->PWMLOAD = 0x200;  /*set LDOK bit */
    HAPTIC_PWM_FTM_MOTOR2_HIGH->PWMLOAD = 0x200;  /*set LDOK bit */
    HAPTIC_PWM_FTM_MOTOR2_LOW ->PWMLOAD = 0x200;  /*set LDOK bit */

    FTM_SetGlobalTimeBaseOutputEnable(HAPTIC_PWM_FTM_AUDIO, true);
    audio_fs_to_pwm_fs_counter = 0;
}

size_t haptic_pwm_callback_ready( void )
{
    size_t ret = pS->callbackCounter;
    /* Reset callback if read*/
    if( ret > 0 ) { pS->callbackCounter = 0; }
    return ret;
}

