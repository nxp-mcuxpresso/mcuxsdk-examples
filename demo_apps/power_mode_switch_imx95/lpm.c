/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_lptmr.h"
#include "fsl_mu.h"
#include "lpm.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"
#include "scmi.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SYSTICK_SOURCE_CLOCK (24000000U)

void SysTick_Handler(void);

struct _lpm_power_mode_listener
{
    lpm_power_mode_callback_t callback;
    void *data;
    struct _lpm_power_mode_listener *next;
};

typedef struct _lpm_power_mode_listener lpm_power_mode_listener_t;

lpm_power_mode_t s_curMode;

static SemaphoreHandle_t s_mutex;
#if configSUPPORT_STATIC_ALLOCATION
static StaticSemaphore_t s_staticMutex;
#endif
static lpm_power_mode_listener_t *s_listenerHead;
static lpm_power_mode_listener_t *s_listenerTail;

volatile uint32_t s_control;

/* LPM M7 suspend resume entry. */
uint32_t resume_entry;

volatile uint32_t s_psp;
volatile uint32_t s_msp;

uint32_t tmp_stack[0x100];

/* Init vector table at 0x0, the first boot of M7 stats from this original table. */
#if defined(__ICCARM__)
extern void  __RESERVED_START;
extern void  __VECTOR_TABLE;
extern void  __VECTOR_TABLE_SIZE;
#elif defined(__GNUC__)
extern void const __RESERVED_START;
extern void const __VECTOR_TABLE;
extern void const __VECTOR_TABLE_SIZE;
#endif

typedef struct _lpm_nvic_context
{
    uint32_t PriorityGroup;
    uint32_t ISER[8];
    uint8_t IPR[240];
    uint8_t SHPR[12];
    uint32_t ICSR;
    uint32_t VTOR;
    uint32_t AIRCR;
    uint32_t SCR;
    uint32_t CCR;
    uint32_t SHCSR;
    uint32_t MMFAR;
    uint32_t BFAR;
    uint32_t CPACR;

} lpm_nvic_context_t;
static lpm_nvic_context_t s_nvicContext;

bool LPM_Init(void)
{
#if configSUPPORT_STATIC_ALLOCATION
    s_mutex = xSemaphoreCreateMutexStatic(&s_staticMutex);
#else
    s_mutex = xSemaphoreCreateMutex();
#endif

    if (s_mutex == NULL)
    {
        return false;
    }

    s_listenerHead = s_listenerTail = NULL;
    s_curMode                       = LPM_PowerModeRun;

    return true;
}

void LPM_Deinit(void)
{
    if (s_mutex != NULL)
    {
        vSemaphoreDelete(s_mutex);
        s_mutex = NULL;
    }
}

lpm_power_mode_t LPM_GetPowerMode(void)
{
    return s_curMode;
}

bool LPM_SetPowerMode(lpm_power_mode_t mode)
{
    lpm_power_mode_listener_t *l1, *l2;
    bool ret = true;

    if (mode == s_curMode)
    {
        return ret;
    }

    xSemaphoreTake(s_mutex, portMAX_DELAY);

    for (l1 = s_listenerHead; l1 != NULL; l1 = l1->next)
    {
        if (l1->callback == NULL)
        {
            continue;
        }

        if (!l1->callback(s_curMode, mode, l1->data))
        {
            /* One stakeholder doesn't allow new mode */
            ret = false;
            break;
        }
    }

    if (ret)
    {
        s_curMode = mode;
    }
    else
    {
        /* roll back the state change of previous listeners */
        for (l2 = s_listenerHead; l2 != l1; l2 = l2->next)
        {
            if (l2->callback == NULL)
            {
                continue;
            }

            l2->callback(mode, s_curMode, l2->data);
        }
    }

    xSemaphoreGive(s_mutex);

    return ret;
}

void LPM_NvicStateSave(void)
{
    uint32_t i;
    uint32_t irqRegs;
    uint32_t irqNum;

    irqRegs = (SCnSCB->ICTR & SCnSCB_ICTR_INTLINESNUM_Msk) + 1;
    irqNum  = irqRegs * 32;

    s_nvicContext.PriorityGroup = NVIC_GetPriorityGrouping();

    for (i = 0; i < irqRegs; i++)
    {
        s_nvicContext.ISER[i] = NVIC->ISER[i];
    }

    for (i = 0; i < irqNum; i++)
    {
        s_nvicContext.IPR[i] = NVIC->IPR[i];
    }

    s_nvicContext.SHPR[0]  = SCB->SHPR[0];  /* MemManage */
    s_nvicContext.SHPR[1]  = SCB->SHPR[1];  /* BusFault */
    s_nvicContext.SHPR[2]  = SCB->SHPR[2];  /* UsageFault */
    s_nvicContext.SHPR[7]  = SCB->SHPR[7];  /* SVCall */
    s_nvicContext.SHPR[8]  = SCB->SHPR[8];  /* DebugMonitor */
    s_nvicContext.SHPR[10] = SCB->SHPR[10]; /* PendSV */
    s_nvicContext.SHPR[11] = SCB->SHPR[11]; /* SysTick */

    s_nvicContext.ICSR  = SCB->ICSR;
    s_nvicContext.VTOR  = SCB->VTOR;
    s_nvicContext.AIRCR = SCB->AIRCR;
    s_nvicContext.SCR   = SCB->SCR;
    s_nvicContext.CCR   = SCB->CCR;
    s_nvicContext.SHCSR = SCB->SHCSR;
    s_nvicContext.MMFAR = SCB->MMFAR;
    s_nvicContext.BFAR  = SCB->BFAR;
    s_nvicContext.CPACR = SCB->CPACR;
}

void LPM_NvicStateRestore(void)
{
    uint32_t i;
    uint32_t irqRegs;
    uint32_t irqNum;

    irqRegs = (SCnSCB->ICTR & SCnSCB_ICTR_INTLINESNUM_Msk) + 1;
    irqNum  = irqRegs * 32;

    NVIC_SetPriorityGrouping(s_nvicContext.PriorityGroup);

    for (i = 0; i < irqRegs; i++)
    {
        NVIC->ISER[i] = s_nvicContext.ISER[i];
    }

    for (i = 0; i < irqNum; i++)
    {
        NVIC->IPR[i] = s_nvicContext.IPR[i];
    }

    SCB->SHPR[0]  = s_nvicContext.SHPR[0];  /* MemManage */
    SCB->SHPR[1]  = s_nvicContext.SHPR[1];  /* BusFault */
    SCB->SHPR[2]  = s_nvicContext.SHPR[2];  /* UsageFault */
    SCB->SHPR[7]  = s_nvicContext.SHPR[7];  /* SVCall */
    SCB->SHPR[8]  = s_nvicContext.SHPR[8];  /* DebugMonitor */
    SCB->SHPR[10] = s_nvicContext.SHPR[10]; /* PendSV */
    SCB->SHPR[11] = s_nvicContext.SHPR[11]; /* SysTick */

    SCB->ICSR  = s_nvicContext.ICSR;
    SCB->VTOR  = s_nvicContext.VTOR;
    SCB->AIRCR = s_nvicContext.AIRCR;
    SCB->SCR   = s_nvicContext.SCR;
    SCB->CCR   = s_nvicContext.CCR;
    SCB->SHCSR = s_nvicContext.SHCSR;
    SCB->MMFAR = s_nvicContext.MMFAR;
    SCB->BFAR  = s_nvicContext.BFAR;
    SCB->CPACR = s_nvicContext.CPACR;
}

void LPM_SetReumeEntry(bool resume)
{
    int32_t status = SCMI_ERR_SUCCESS;
    uint32_t vectorLow, vectorhigh;
    /* M7 resume entry use low reg in sm side, high vector value is used by sm to deal with linux system. */
    vectorhigh = 0;
    vectorLow = 0;

    if (resume)
    {
        vectorLow = (uint32_t)&__RESERVED_START;
    }

    /* Setup M7 suspend resume Entry. */
    status = SCMI_CpuResetVectorSet(SCMI_A2P, CPU_IDX_M7P, SCMI_CPU_VEC_FLAGS_RESUME(1), vectorLow, vectorhigh);
    if (status != SCMI_ERR_SUCCESS)
    {
        PRINTF("set cpu reset vector by sm fail\r\n");
    }
}

bool LPM_Resume(void)
{
     /* Re-entry point of Power Down wake. */
    __asm volatile(
        "resume:\n"
        "CPSID   I\n"            /* Mask interrupts */
        "LDR     R2, =tmp_stack\n"  /* Setup temp stack for following init */
        "ADD     R2, R2, #0x300\n"
        "MSR     MSP, R2\n");

    LPM_NvicStateRestore();
    BOARD_ConfigMPU();

    __asm volatile(
        "LDR     R1, =s_control\n" /* #4 Restore CONTROL register from global variable s_control */
        "LDR     R0, [R1]\n"
        "MSR     CONTROL, R0\n"

        "LDR     R1, =s_msp\n" /* #3 Restore MSP register from global variable s_msp */
        "LDR     R0, [R1]\n"
        "MSR     MSP, R0\n"

        "LDR     R1, =s_psp\n" /* #2 Restore PSP register from global variable s_psp */
        "LDR     R0, [R1]\n"
        "MSR     PSP, R0\n"

        "POP    {R4-R11, LR}\n" /* #1 Restore core registers from current stack */
    );

    return true;
}

void LPM_Vector_Copy()
{
    uint32_t source = (uint32_t)&__VECTOR_TABLE;
    uint32_t des = (uint32_t)&__RESERVED_START;
    uint32_t vector_size = (uint32_t)&__VECTOR_TABLE_SIZE;

    /*
     * M7 ININVCTOR is different with M33, VTOR[31:7] indicates the value of signal INITVTOR[31:7]. VTOR[6:0] are RAZ.
     * To assure M7 can resume into previous usecase environment, copy vector table into another memory.
     * And update M7 reset handler to LPM_Resume when M7 resume from suspend mode.
     */
     for(uint32_t i = 0x0; i < vector_size; i++)
     {
        if (i != 1)
        {
            *((volatile uint32_t *)(des)) = *((volatile uint32_t *)(source));
        }
        else
        {
            /* set resume entry from default Reset_Handler into LPM_Resume. */
            *((volatile uint32_t *)(des)) = (uint32_t)LPM_Resume;
        }
        des += 4;
        source += 4;
     }
}

void LPM_M7_Suspend()
{
    /* Put M7 core into suspend mode. */
    SCMI_CpuSleepModeSet(SCMI_A2P, CPU_IDX_M7P, SCMI_CPU_FLAGS_IRQ_MUX(0), SCMI_CPU_SLEEP_SUSPEND);

    __DSB();
    __WFI();
    __ISB();
}

bool LPM_Suspend()
{
    /* Save current M7 core state register, can't add variable in this function, it will impact stack state. */
    __asm volatile(
        "loop:\n"
        "PUSH    {R4-R11, LR}\n" /* #1 Save core registers to current stack */

        "MRS     R0, PSP\n"      /* #2 Save PSP register to global variable s_psp */
        "LDR     R1, =s_psp\n"
        "STR     R0, [R1]\n"

        "MRS     R0, MSP\n" /* #3 Save MSP register to global variable s_msp */
        "LDR     R1, =s_msp\n"
        "STR     R0, [R1]\n"

        "MRS     R0, CONTROL\n" /* #4 Save CONTROL register to global variable s_control */
        "LDR     R1, =s_control\n"
        "STR     R0, [R1]\n");

    LPM_Vector_Copy();
    LPM_NvicStateSave();   /* #5 Save NVIC setting */
    LPM_SetReumeEntry(true);

    LPM_M7_Suspend();

    /* Successful Power Down exit will never come here, but jump to LPM_Resume() */

    /* Power Down entering fail, retore core register before return */
    __asm volatile("POP    {R4-R11, LR}\n");

    return false;
}

bool LPM_SystemPowerDown()
{
    uint32_t status;

    status = LPM_Suspend();
    LPM_SetReumeEntry(false);

    return status;
}

bool LPM_WaitForInterrupt(uint32_t timeoutMilliSec)
{
    uint32_t irqMask;
    status_t status = kStatus_Success;

    irqMask = DisableGlobalIRQ();

    DisableIRQ(SYSTICK_IRQn);
    LPTMR_StopTimer(SYSTICK_BASE);

    switch (s_curMode)
    {
        case LPM_PowerModeWait:
             status = SCMI_CpuSleepModeSet(SCMI_A2P, CPU_IDX_M7P, SCMI_CPU_FLAGS_IRQ_MUX(0), SCMI_CPU_SLEEP_WAIT);
            __DSB();
            __WFI();
            __ISB();
            status = kStatus_Success;
            break;
        case LPM_PowerModeStop:
            status = SCMI_CpuSleepModeSet(SCMI_A2P, CPU_IDX_M7P, SCMI_CPU_FLAGS_IRQ_MUX(0), SCMI_CPU_SLEEP_STOP);
            __DSB();
            __WFI();
            __ISB();
            status = kStatus_Success;
            break;
        case LPM_PowerModeSuspend:
            if (!LPM_SystemPowerDown())
            {
                status = kStatus_Fail;
            }
            status = kStatus_Success;
            break;
        default:
            break;
    }

    LPTMR_StartTimer(SYSTICK_BASE);
    NVIC_EnableIRQ(SYSTICK_IRQn);
    EnableGlobalIRQ(irqMask);

    return status == kStatus_Success;
}

void LPM_RegisterPowerListener(lpm_power_mode_callback_t callback, void *data)
{
    lpm_power_mode_listener_t *l = (lpm_power_mode_listener_t *)pvPortMalloc(sizeof(lpm_power_mode_listener_t));
    assert(l);

    l->callback = callback;
    l->data     = data;
    l->next     = NULL;

    xSemaphoreTake(s_mutex, portMAX_DELAY);

    if (s_listenerHead)
    {
        s_listenerTail->next = l;
        s_listenerTail       = l;
    }
    else
    {
        s_listenerHead = s_listenerTail = l;
    }

    xSemaphoreGive(s_mutex);
}

void LPM_UnregisterPowerListener(lpm_power_mode_callback_t callback, void *data)
{
    lpm_power_mode_listener_t *l, *p = NULL;

    xSemaphoreTake(s_mutex, portMAX_DELAY);

    for (l = s_listenerHead; l != NULL; l = l->next)
    {
        if (l->callback == callback && l->data == data)
        {
            if (p)
            {
                p->next = l->next;
            }
            else
            {
                s_listenerHead = l->next;
            }

            if (l->next == NULL)
            {
                s_listenerTail = p;
            }

            vPortFree(l);
            break;
        }
        p = l;
    }

    xSemaphoreGive(s_mutex);
}

/************ Internal public API start **************/
/* The systick interrupt handler. */
void SYSTICK_HANDLER(void)
{
    /* Clear the interrupt. */
    LPTMR_ClearStatusFlags(SYSTICK_BASE, kLPTMR_TimerCompareFlag);

    if (SYSTICK_BASE->CSR & LPTMR_CSR_TFC_MASK)
    {
        /* Freerun timer means this is the first tick after tickless exit. */
        LPTMR_StopTimer(SYSTICK_BASE);
        LPTMR_SetTimerPeriod(SYSTICK_BASE, SYSTICK_SOURCE_CLOCK / configTICK_RATE_HZ);
        SYSTICK_BASE->CSR &= ~LPTMR_CSR_TFC_MASK;
        LPTMR_StartTimer(SYSTICK_BASE);
    }
    /* Call FreeRTOS tick handler. */
    xPortSysTickHandler();
}

/* Override the default definition of vPortSetupTimerInterrupt() that is weakly
 * defined in the FreeRTOS Cortex-M7 port layer with a version that configures LPTMR1
 * to generate the tick interrupt. */
void vPortSetupTimerInterrupt(void)
{
    lptmr_config_t lptmrConfig;

    /*
     * lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrConfig.enableFreeRunning = false;
     * lptmrConfig.bypassPrescaler = true;
     * lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrConfig.value = kLPTMR_Prescale_Glitch_0;
     */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    /* Select SIRC as tick timer clock source */
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_0;
    /* Initialize the LPTMR */
    LPTMR_Init(SYSTICK_BASE, &lptmrConfig);

    /* Set timer period */
    LPTMR_SetTimerPeriod(SYSTICK_BASE, SYSTICK_SOURCE_CLOCK / configTICK_RATE_HZ);

    /* Enable timer interrupt */
    LPTMR_EnableInterrupts(SYSTICK_BASE, kLPTMR_TimerInterruptEnable);
    NVIC_SetPriority(SYSTICK_IRQn, configLIBRARY_LOWEST_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ(SYSTICK_IRQn);

    /* Start counting */
    LPTMR_StartTimer(SYSTICK_BASE);
}

