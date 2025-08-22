/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "mc_periph_init.h"
#include "freemaster.h"
#include "freemaster_serial_lpuart.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "fsl_rgpio.h"
#include "fsl_lpuart.h"
#include "m1_sm_servo.h"
#include "m2_sm_servo.h"
#include "board.h"
#include "mu_ipc_shm_client.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER "2.0.0" /* motor control package version */

/* Example's feature set in form of bits inside ui16featureSet.
   This feature set is expected to be growing over time.
   ... | FEATURE_S_RAMP | FEATURE_FIELD_WEAKENING | FEATURE_ENC
*/
#define FEATURE_ENC (1)               /* Encoder feature flag */
#define FEATURE_FIELD_WEAKENING (0)   /* Field weakening feature flag */
#define FEATURE_S_RAMP (0)            /* S-ramp feature flag */

#define FEATURE_SET (FEATURE_ENC << (0) | \
                     FEATURE_FIELD_WEAKENING << (1) | \
                     FEATURE_S_RAMP << (2))

/*! @brief The UART to use for FreeMASTER communication */
#define _FMSTR_LPUART_BASE_ADDR(x)   LPUART##x
#define _FMSTR_LPUART_IRQn(x)        LPUART##x##_IRQn
#define _FMSTR_LPUART_IRQ_HANDLER(x) LPUART##x##_IRQHandler

#define FMSTR_LPUART_BASE_ADDR(x)    _FMSTR_LPUART_BASE_ADDR(x)
#define FMSTR_LPUART_IRQn(x)         _FMSTR_LPUART_IRQn(x)
#define FMSTR_LPUART_IRQ_HANDLER(x)  _FMSTR_LPUART_IRQ_HANDLER(x)

#define BOARD_USER_BUTTON_PRIORITY 4

/* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT(par1)   \
    uint32_t val  = SysTick->VAL;  \
    uint32_t load = SysTick->LOAD; \
    par1          = load - val

/* Three instruction added after interrupt flag clearing as required */
#define M1_END_OF_ISR \
    {                 \
        __DSB();      \
        __ISB();      \
    }

/* Init SDK HW */
//static void BOARD_Init(void);
RAM_FUNC_LIB
void PWM1_0_IRQHandler(void);
RAM_FUNC_LIB
void PWM2_0_IRQHandler(void);
/* TMR1 reload ISR called with 1ms period */
RAM_FUNC_LIB
void TMR1_IRQHandler(void);
/* SW8 Button interrupt handler */
RAM_FUNC_LIB
void GPIO1_0_IRQHandler(void);
/* SINC conversation interrupt handler */
RAM_FUNC_LIB
void SINC1_CH0_IRQHandler(void);

/* EnDat2.2 interrupt */
RAM_FUNC_LIB
void ENDAT2P2_IRQHandler(void);

RAM_FUNC_LIB
void SINC2_CH0_IRQHandler(void);
/* BiSS OET interrupt handler */
RAM_FUNC_LIB
void BISS_EOT_IRQHandler(void);

static void BOARD_InitSysTick(void);
static void Application_Control_BL(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* CPU load measurement using Systick */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;
uint32_t g_ui32M2NumberOfCycles    = 0U;
uint32_t g_ui32M2MaxNumberOfCycles = 0U;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "../../../examples/_boards/imx943evk/ecat_examples/dual_cores_servo_motor/remote/cm7_core1",        /* User Path 1- the highest priority */
    "",       /* User Path 2 */
    "imx943evk", /* board id */
    "pmsm_servo_dual",     /* example id */
    MCRSP_VER,      /* sw version */
    FEATURE_SET,    /* example's feature-set */
};

//ctrl_m1_mid_t g_sSpinMidSwitch;           /* Control Spin/MID switching */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void init_freemaster_lpuart(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
int32_t GetM1PositionActualValue(void)
{
    return g_sM1Enc.data.position.position & 0xFFFFFFFF;
}

int32_t GetM2PositionActualValue(void)
{
    return g_sM2Enc.data.position.position & 0xFFFFFFFF;
    // return g_sM2Enc.i64EndatPositionMT & 0xFFFFFFFF;
}

static int64_t i64EthercatPosTargetM1 = 0x0;
static int64_t i64EthercatPosTargetM2 = 0x0;

acc32_t GetM1PositionCmdValue(int32_t targetPos)
{
    int32_t i32MultTurn = 0;
    uint16_t u16SingleTurn = 0;
    int32_t delta;
    delta = ((int32_t)targetPos - (int32_t)(i64EthercatPosTargetM1 & 0xFFFFFFFF));
    i64EthercatPosTargetM1 += delta;
    i32MultTurn = (i64EthercatPosTargetM1 >> 25) & 0x1FFFFFFFF ;
    u16SingleTurn = (i64EthercatPosTargetM1 >> 10) & 0x7FFF;
    return (acc32_t)((i32MultTurn << 15 ) + u16SingleTurn);
}

acc32_t GetM2PositionCmdValue(int32_t targetPos)
{
    int32_t i32MultTurn = 0;
    uint16_t u16SingleTurn = 0;
    int32_t delta;
    delta = ((int32_t)targetPos - (int32_t)(i64EthercatPosTargetM2 & 0xFFFFFFFF));
    i64EthercatPosTargetM2 += delta;
    i32MultTurn = (i64EthercatPosTargetM2 >> 25) & 0x1FFFFFFFF ;
    u16SingleTurn = (i64EthercatPosTargetM2 >> 10) & 0x7FFF;
    return (acc32_t)((i32MultTurn << 15 ) + u16SingleTurn);
}

int Cia402_status_machine_trans(uint8_t axis, uint8_t trans_id)
{
   int ret = 0;
   if (axis == 0) {
       switch (trans_id) {
           case 0: break;
           case 1: break;
           case 2: break;
           case 3:
               M1_OpenPWM();
               break;
           case 4:
               M1_SetAppSwitch(1);
               i64EthercatPosTargetM1 = g_sM1Enc.i64EndatPositionMT;
               if (g_sM1Ctrl.eState != kSM_AppRun || g_eM1StateRun != kRunState_Spin)
               {
                   ret = -1;
               }
               break;
           case 5:
               M1_SetAppSwitch(0);
               break;
           case 6:
               M1_ClosePWM();
               break;
           case 7:
           case 8:
           case 9:
           case 10:
           case 11:
           case 12:
           case 13:
           case 14:
           case 15:
               M2_ClosePWM();
               break;
       }
   } else if (axis == 1){
       switch (trans_id) {
           case 0: break;
           case 1: break;
           case 2: break;
           case 3:
               M2_OpenPWM();
               break;
           case 4:
               M2_SetAppSwitch(1);
               i64EthercatPosTargetM2 = g_sM2Enc.i64EndatPositionMT;
               if (g_sM2Ctrl.eState != kSM_AppRun || g_eM2StateRun != kRunState_Spin) {
                   ret = -1;
               }
               break;
           case 5:
               M2_SetAppSwitch(0);
               break;
           case 6:
               M2_ClosePWM();
               break;
           case 7:
           case 8:
           case 9:
           case 10:
           case 11:
           case 12:
           case 13:
           case 14:
           case 15:
               M2_ClosePWM();
               break;
       }
   }
   return ret;
}

int obj_write_callback(uint16_t Index, uint8_t Subindex, uint8_t size, void *pData)
{
    uint8_t *p = pData;
    if (Index == 0x6060 && Subindex == 0x0) {
        if (p[0] == 0x08) {
            g_sM1Drive.eControl = kControlMode_PositionFOC;
        }
    } else if (Index == 0x6860 && Subindex == 0x0) {
        if (p[0] == 0x08) {
            g_sM2Drive.eControl = kControlMode_PositionFOC;
        }
    } 
    return 0;
}

int obj_read_callback(uint16_t Index, uint8_t Subindex, uint8_t size, void *pData)
{
    PRINTF("read %d %d\r\n", Index, Subindex);
    uint32_t *p = pData;
    if (Index == 0x6064 && Subindex == 0x0) {
        *p = GetM1PositionActualValue();
        return sizeof(*p);
    } else if (Index == 0x606c && Subindex == 0x0) {
        *p = (uint32_t)g_sM1Drive.sSpeed.fltSpeedFilt;
        return sizeof(*p);
    }  else  if (Index == 0x6864 && Subindex == 0x0) {
        *p = GetM2PositionActualValue();
        return sizeof(*p);
    } else if (Index == 0x686c && Subindex == 0x0) {
        *p = (uint32_t)g_sM2Drive.sSpeed.fltSpeedFilt;
        return sizeof(*p);
    }
    return 0;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /*Accessing ID structure to prevent optimization*/
    g_sAppIdFM.ui16FeatureSet = FEATURE_SET;

    uint32_t ui32PrimaskReg;

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();

    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
    MU_ipc_shm_client_init();
    /* FreeMASTER communication layer initialization */
   // init_freemaster_lpuart();
   // FMSTR_Init();

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();
    /* Init peripheral motor control driver for motors M1 and M2 */
    MCDRV_Init();
    /* Turn off application */
    M1_SetAppSwitch(FALSE);
    M2_SetAppSwitch(FALSE);

    /* Spin state machine is default */
  //  g_sSpinMidSwitch.eAppState = kAppStateSpin;

    /* Enable interrupts */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Enable PWM clock */
    g_sM1Pwm3ph.pui32PwmBaseAddress->MCTRL |= PWM_MCTRL_RUN(0xF);    /* Enable PWM for Motor 1 */
    g_sM2Pwm3ph.pui32PwmBaseAddress->MCTRL |= PWM_MCTRL_RUN(0xF);    /* Enable PWM for Motor 2 */
    PRINTF("%s %s %d\r\n", __FILE__, __func__, __LINE__);
    /* Infinite loop */
    while (1)
    {
        SDK_DelayAtLeastUs((50 * 1000), SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        /* FreeMASTER Polling function */
       // FMSTR_Poll();
    }
}

// RAM_FUNC_LIB
// void SINC1_CH0_IRQHandler(void)
// {
//     M1_MCDRV_SINC_GET(&g_sM1Curr3phDcBus);
//     SDK_ISR_EXIT_BARRIER;
// }

// RAM_FUNC_LIB
// void SINC2_CH0_IRQHandler(void)
// {
//     M2_MCDRV_SINC_GET(&g_sM2Curr3phDcBus);
//     SDK_ISR_EXIT_BARRIER;
// }

RAM_FUNC_LIB
void M1_ENDAT2P2_IRQHandler(void)
{
   // PRINTF("s");
    /* Set M1 TP1 */
    M1_MCDRV_SINC_GET(&g_sM1Curr3phDcBus);
    /* get position from EnDat2.2 */
    M1_MCDRV_ENDAT2P2_GET(&g_sM1Enc);
    /* M2 State machine */
    SM_StateMachineFast(&g_sM1Ctrl);

    SDK_ISR_EXIT_BARRIER;
}

RAM_FUNC_LIB
void M2_ENDAT2P2_IRQHandler(void)
{
    /* Set M1 TP1 */
   // PRINTF("k");
    M2_MCDRV_SINC_GET(&g_sM2Curr3phDcBus);
    /* get position from EnDat2.2 */
    M2_MCDRV_ENDAT2P2_GET(&g_sM2Enc);
    /* M2 State machine */
    SM_StateMachineFast(&g_sM2Ctrl);

    SDK_ISR_EXIT_BARRIER;
}
/*!
 * @brief   motor_slow_task called with EtherCAT DC period and processes following functions:
 *           - motor slow application machine function
 *
 * @param   void
 *
 * @return  none
 */
RAM_FUNC_LIB
int motor_slow_task(int axis, struct pdo_to_motor_t *pdo_m, struct pdo_to_esc_t *pdo_e, struct param_t *param)
{
    if (axis == 0) {
	M1_SetPosition(GetM1PositionCmdValue(pdo_m->Target_pos));
        SM_StateMachineSlow(&g_sM1Ctrl);
        pdo_e->Actual_pos = GetM1PositionActualValue();
        pdo_e->Actual_vel = (uint32_t)(g_sM1Drive.sSpeed.fltSpeedFilt);

    } else if (axis == 1) {
        M2_SetPosition(GetM2PositionCmdValue(pdo_m->Target_pos));
        SM_StateMachineSlow(&g_sM2Ctrl);
        pdo_e->Actual_pos = GetM2PositionActualValue();
        pdo_e->Actual_vel = (uint32_t)(g_sM2Drive.sSpeed.fltSpeedFilt);
    }
}

/*!
 * @brief LPUART Module initialization (LPUART is a the standard block included e.g. in K66F)
 */
static void init_freemaster_lpuart(void)
{
    lpuart_config_t config;

    /* clang-format off */
    clk_t clk = {
        .clkId = kCLOCK_IpInvalid,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    /* clang-format on */

    clk.clkId = BOARD_GetUartClkId(BOARD_DEBUG_CONSOLE_PORT);

    CLOCK_SetRate(&clk);
    CLOCK_EnableClock(clk.clkId);

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200U;
    config.enableTx     = false;
    config.enableRx     = false;

    LPUART_Init(FMSTR_LPUART_BASE_ADDR(BOARD_DEBUG_UART_INSTANCE), &config, CLOCK_GetRate(clk.clkId));

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress(FMSTR_LPUART_BASE_ADDR(BOARD_DEBUG_UART_INSTANCE));

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(FMSTR_LPUART_IRQn(BOARD_DEBUG_UART_INSTANCE));
    EnableGlobalIRQ(0);
#endif
}

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
/*
 *   Application interrupt handler of communication peripheral used in interrupt modes
 *   of FreeMASTER communication.
 *
 *   NXP MCUXpresso SDK framework defines interrupt vector table as a part of "startup_XXXXXX.x"
 *   assembler/C file. The table points to weakly defined symbols, which may be overwritten by the
 *   application specific implementation. FreeMASTER overrides the original weak definition and
 *   redirects the call to its own handler.
 *
 */

void FMSTR_LPUART_IRQ_HANDLER(BOARD_DEBUG_UART_INSTANCE)(void)
{
    /* Call FreeMASTER Interrupt routine handler */
    FMSTR_SerialIsr();
}
#endif

/*!
 *@brief      SysTick initialization for CPU cycle measurement
 *
 *@param      none
 *
 *@return     none
 */
static void BOARD_InitSysTick(void)
{
    /* Initialize SysTick core timer to run free */
    /* Set period to maximum value 2^24*/
    SysTick->LOAD = 0xFFFFFF;

    /*Clock source - System Clock*/
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    /*Start Sys Timer*/
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}
