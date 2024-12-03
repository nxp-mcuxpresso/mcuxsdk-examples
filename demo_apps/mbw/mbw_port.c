#include "mbw.h"
#include "clock_config.h"
#include "mbw_def.h"
#if defined PIT0 || defined PIT1
#include "fsl_pit.h"
#elif defined LPIT0 || defined LPIT1
#include "fsl_lpit.h"
#elif defined GPT0 || defined GPT1
#include "fsl_gpt.h"
#elif defined(CTIMER0)
#include "fsl_ctimer.h"
#else
#error "Pls support the new timer"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined PIT1 && !defined DEMO_PIT_BASE
/* For PIT, you need define macros as below: */
#define DEMO_PIT_BASE       (PIT1)
#define DEMO_PIT_CLOCK_NAME (kCLOCK_Pit1)
#define DEMO_PIT_CLOCK_FREQ CLOCK_GetIpFreq(DEMO_PIT_CLOCK_NAME)
#elif defined LPIT0 && !defined DEMO_LPIT_BASE
/* For LPIT, you need define macros as below: */
#define DEMO_LPIT_BASE       LPIT0
#define DEMO_LPIT_CLOCK_NAME (kCLOCK_Lpit0)
#define DEMO_LPIT_CLOCK_FREQ CLOCK_GetIpFreq(DEMO_LPIT_CLOCK_NAME)
#elif defined GPT1 && !defined DEMO_GPT_BASE
/* For GPT, you need define macros as below: */
#define DEMO_GPT_BASE       GPT1
#define DEMO_GPT_CLOCK_NAME (kCLOCK_Gpt1)
#define DEMO_GPT_CLOCK_FREQ                                                                 \
    (CLOCK_GetPllFreq(kCLOCK_SystemPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootGpt1)) / \
     (CLOCK_GetRootPostDivider(kCLOCK_RootGpt1)) / 2)
#define DEMO_GPT_IRQ        (GPT1_IRQn)
#define DEMO_GPT_CLK_DIV    (3U)
#define DEMO_GPT_IRQHandler GPT1_IRQHandler
#elif defined(CTIMER0) && (!defined(DEMO_CTIMER_BASE))
#define DEMO_CTIMER_BASE CTIMER0
#define DEMO_CTIMER_CLOCK_FREQ CLOCK_GetCTimerClkFreq(0U)
#endif


#ifdef DEMO_GPT_BASE
void DEMO_GPT_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    GPT_ClearStatusFlags(DEMO_GPT_BASE, kGPT_OutputCompare1Flag);

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F Store immediate overlapping
       exception return operation might vector to incorrect interrupt */
    SDK_ISR_EXIT_BARRIER;
}
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void timer_init(void)
{
#ifdef DEMO_PIT_BASE
    pit_config_t pitConfig;

    // PIT clock gate control ON
    CLOCK_EnableClock(DEMO_PIT_CLOCK_NAME);

    PIT_GetDefaultConfig(&pitConfig);
    // Init pit module
    PIT_Init(DEMO_PIT_BASE, &pitConfig);
    // Set max timer period for channel 1
    PIT_SetTimerPeriod(DEMO_PIT_BASE, kLPIT_Chnl_1, (uint32_t)~0);
    // Disable timer interrupts for channel 1
    PIT_DisableInterrupts(DEMO_PIT_BASE, kLPIT_Chnl_1, kLPIT_TimerInterruptEnable);
    // Clear timer channel 1 flag
    PIT_ClearStatusFlags(DEMO_PIT_BASE, kLPIT_Chnl_1, kLPIT_TimerFlag);
    // Chain timer channel 1 to channel 0
    PIT_SetTimerChainMode(DEMO_PIT_BASE, kLPIT_Chnl_1, true);
    // Start timer channel 1
    PIT_StartTimer(DEMO_PIT_BASE, kLPIT_Chnl_1);

    // Set max timer period for channel 0
    PIT_SetTimerPeriod(DEMO_PIT_BASE, kLPIT_Chnl_0, (uint32_t)~0);
    // Clear timer channel 0 flag
    PIT_ClearStatusFlags(DEMO_PIT_BASE, kLPIT_Chnl_0, kLPIT_TimerFlag);
    // Start timer channel 0
    PIT_StartTimer(DEMO_PIT_BASE, kLPIT_Chnl_0);
#elif defined(DEMO_LPIT_BASE)
    lpit_config_t lpitConfig;
    lpit_chnl_params_t lpitChannelConfig;

    lpitChannelConfig.chainChannel          = false;
    lpitChannelConfig.enableReloadOnTrigger = false;
    lpitChannelConfig.enableStartOnTrigger  = false;
    lpitChannelConfig.enableStopOnTimeout   = false;
    lpitChannelConfig.timerMode             = kLPIT_PeriodicCounter;
    /* Set default values for the trigger source */
    lpitChannelConfig.triggerSelect = kLPIT_Trigger_TimerChn0;
    lpitChannelConfig.triggerSource = kLPIT_TriggerSource_External;
    LPIT_GetDefaultConfig(&lpitConfig);
    /* Init LPIT module */
    LPIT_Init(DEMO_LPIT_BASE, &lpitConfig);
    /* Init LPIT channel 0 */
    LPIT_SetupChannel(DEMO_LPIT_BASE, kLPIT_Chnl_0, &lpitChannelConfig);
    /* Set timer period for channel 0 */
    LPIT_SetTimerPeriod(DEMO_LPIT_BASE, kLPIT_Chnl_0, (uint32_t)~0);
    LPIT_ClearStatusFlags(DEMO_LPIT_BASE, kLPIT_Channel0TimerFlag);
    /* For LPIT channel 1 */
    lpitChannelConfig.chainChannel = true;
    /* Init lpit channel 1 */
    LPIT_SetupChannel(DEMO_LPIT_BASE, kLPIT_Chnl_1, &lpitChannelConfig);
    /* Set timer period for channel 1 */
    LPIT_SetTimerPeriod(DEMO_LPIT_BASE, kLPIT_Chnl_1, (uint32_t)~0);
    LPIT_ClearStatusFlags(DEMO_LPIT_BASE, kLPIT_Channel1TimerFlag);
    /* Disable timer interrupt for channel 0 and 1 */
    LPIT_DisableInterrupts(DEMO_LPIT_BASE, kLPIT_Channel0TimerInterruptEnable | kLPIT_Channel1TimerInterruptEnable);

    /* Start channel 1 */
    LPIT_StartTimer(DEMO_LPIT_BASE, kLPIT_Chnl_1);
    /* Start channel 0 */
    LPIT_StartTimer(DEMO_LPIT_BASE, kLPIT_Chnl_0);
#elif defined(DEMO_GPT_BASE)
    gpt_config_t gptConfig;
    uint32_t gptFreq = 0U;

    GPT_GetDefaultConfig(&gptConfig);
    gptConfig.divider = DEMO_GPT_CLK_DIV;
    /* Initialize GPT module */
    GPT_Init(DEMO_GPT_BASE, &gptConfig);

    /* Get GPT clock frequency */
    gptFreq = DEMO_GPT_CLOCK_FREQ;

    /* GPT frequency is divided by 3 inside module */
    gptFreq /= DEMO_GPT_CLK_DIV;

    /* Set both GPT modules to 1 second duration */
    GPT_SetOutputCompareValue(DEMO_GPT_BASE, kGPT_OutputCompare_Channel1, gptFreq);

    /* Enable GPT Output Compare1 interrupt */
    GPT_EnableInterrupts(DEMO_GPT_BASE, kGPT_OutputCompare1InterruptEnable);

    /* Enable at the Interrupt */
    EnableIRQ(DEMO_GPT_IRQ);

    /* Start Timer */
    GPT_StartTimer(DEMO_GPT_BASE);
#elif defined(DEMO_CTIMER_BASE)
    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(DEMO_CTIMER_BASE, &config);
    CTIMER_StartTimer(DEMO_CTIMER_BASE);
#else
#error "Pls support the new timer(timer_init)"
#endif
}

void timer_deinit(void)
{
#ifdef DEMO_PIT_BASE
    // Stop timer channel 1
    PIT_StopTimer(DEMO_PIT_BASE, kLPIT_Chnl_1);
    // Stop timer channel 0
    PIT_StopTimer(DEMO_PIT_BASE, kLPIT_Chnl_0);

    // Deinit pit module
    PIT_Deinit(DEMO_PIT_BASE);
#elif defined(DEMO_LPIT_BASE)
    /* Stop channel 0 */
    LPIT_StopTimer(DEMO_LPIT_BASE, kLPIT_Chnl_0);
    /* Stop channel 1 */
    LPIT_StopTimer(DEMO_LPIT_BASE, kLPIT_Chnl_1);

    /* Deinit LPIT module */
    LPIT_Deinit(DEMO_LPIT_BASE);
#elif defined(DEMO_GPT_BASE)
    /* Stop Timer */
    GPT_StopTimer(DEMO_GPT_BASE);
    /* Deinit GPT module */
    GPT_Deinit(DEMO_GPT_BASE);
#elif defined(DEMO_CTIMER_BASE)
    CTIMER_StopTimer(DEMO_CTIMER_BASE);
    CTIMER_Deinit(DEMO_CTIMER_BASE);
#else
#error "Pls support the new timer in timer_deinit"
#endif
}

uint64_t timer_clock(void)
{
#if defined(DEMO_PIT_BASE) && (FSL_FEATURE_PIT_HAS_LIFETIME_TIMER) && (FSL_FEATURE_PIT_HAS_LIFETIME_TIMER == 1)
    // Note: first read LTMR64H and then LTMR64L. LTMR64H will have the value
    //  of CVAL1 at the time of the first access, LTMR64L will have the value of CVAL0 at the
    //  time of the first access, therefore the application does not need to worry about carry-over
    //  effects of the running counter.
    return ~PIT_GetLifetimeTimerCount(DEMO_PIT_BASE);
#elif defined(DEMO_PIT_BASE) &&                                                                 \
    ((defined FSL_FEATURE_PIT_HAS_LIFETIME_TIMER && FSL_FEATURE_PIT_HAS_LIFETIME_TIMER == 0) || \
     !defined FSL_FEATURE_PIT_HAS_LIFETIME_TIMER)
    uint64_t valueH;
    volatile uint32_t valueL;
    // Make sure that there are no rollover of valueL.
    // Because the valueL always decreases, so, if the formal valueL is greater than
    // current value, that means the valueH is updated during read valueL.
    // In this case, we need to re-update valueH and valueL.
    do
    {
        valueL = PIT_GetCurrentTimerCount(DEMO_PIT_BASE, kLPIT_Chnl_0);
        valueH = PIT_GetCurrentTimerCount(DEMO_PIT_BASE, kLPIT_Chnl_1);
    } while (valueL < PIT_GetCurrentTimerCount(DEMO_PIT_BASE, kLPIT_Chnl_0));

    // Invert to turn into an up counter
    return ~((valueH << 32) | valueL);
#elif defined(DEMO_LPIT_BASE) && (FSL_FEATURE_LPIT_HAS_LIFETIME_TIMER) && (FSL_FEATURE_LPIT_HAS_LIFETIME_TIMER == 1)
    // Note: first read LTMR64H and then LTMR64L. LTMR64H will have the value
    //  of CVAL1 at the time of the first access, LTMR64L will have the value of CVAL0 at the
    //  time of the first access, therefore the application does not need to worry about carry-over
    //  effects of the running counter.
    return ~LPIT_GetLifetimeTimerCount(DEMO_LPIT_BASE);
#elif defined DEMO_LPIT_BASE &&                                                                   \
    ((defined FSL_FEATURE_LPIT_HAS_LIFETIME_TIMER && FSL_FEATURE_LPIT_HAS_LIFETIME_TIMER == 0) || \
     !defined FSL_FEATURE_LPIT_HAS_LIFETIME_TIMER)
    uint64_t valueH;
    volatile uint32_t valueL;
    // Make sure that there are no rollover of valueL.
    // Because the valueL always decreases, so, if the formal valueL is greater than
    // current value, that means the valueH is updated during read valueL.
    // In this case, we need to re-update valueH and valueL.
    do
    {
        valueL = LPIT_GetCurrentTimerCount(DEMO_LPIT_BASE, kLPIT_Chnl_0);
        valueH = LPIT_GetCurrentTimerCount(DEMO_LPIT_BASE, kLPIT_Chnl_1);
    } while (valueL < LPIT_GetCurrentTimerCount(DEMO_LPIT_BASE, kLPIT_Chnl_0));

    // Invert to turn into an up counter
    return ~((valueH << 32) | valueL);
#elif defined DEMO_GPT_BASE
    uint32_t cnt;
    static uint32_t last_cnt   = 0U;
    static uint64_t last_value = 0ULL;
    uint64_t new_value;

    cnt = GPT_GetCurrentTimerCount(DEMO_GPT_BASE);
    if (cnt < last_cnt && last_value + cnt > 0xFFFFFFFFU)
    {
        new_value = last_value + cnt;
    }
    else
    {
        new_value = cnt;
    }
    last_value = new_value;
    return new_value;
#elif defined(DEMO_CTIMER_BASE)
    uint32_t cnt;
    static uint32_t last_cnt   = 0U;
    static uint64_t last_value = 0ULL;
    uint64_t new_value;

    cnt = CTIMER_GetTimerCountValue(DEMO_CTIMER_BASE);
    if (cnt < last_cnt && last_value + cnt > 0xFFFFFFFFU)
    {
        new_value = last_value + cnt;
    }
    else
    {
        new_value = cnt;
    }
    last_value = new_value;
    return new_value;
#else
#error "Pls support the new timer in timer_clock"
#endif
}

uint32_t timer_clocks_per_sec(void)
{
#ifdef DEMO_PIT_CLOCK_FREQ
    return DEMO_PIT_CLOCK_FREQ;
#elif defined DEMO_LPIT_CLOCK_FREQ
    return DEMO_LPIT_CLOCK_FREQ;
#elif defined DEMO_GPT_CLOCK_FREQ
    return DEMO_GPT_CLOCK_FREQ;
#elif defined(DEMO_CTIMER_CLOCK_FREQ)
    return DEMO_CTIMER_CLOCK_FREQ;
#else
#error "Pls support the new timer in timer_clocks_per_sec"
#endif
}
