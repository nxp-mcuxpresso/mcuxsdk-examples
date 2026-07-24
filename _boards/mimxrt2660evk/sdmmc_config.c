/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sdmmc_config.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_pcal6524.h"
#include "fsl_pca9555.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* CLOCK_SetRootClock is defined in fsl_clock.c but missing from fsl_clock.h
 * in the current MIMXRT2663 device package. Declare it here until the header
 * is fixed upstream. */
extern void CLOCK_SetRootClock(clock_root_t root, const clock_root_config_t *config);
void BOARD_SDCardPowerControl(bool enable);
#if defined(SDIO_ENABLED) || defined(SD_ENABLED)
void BOARD_SDCardIoVoltageControl(sdmmc_operation_voltage_t voltage);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*!brief sdmmc dma buffer */
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_sdmmcHostDmaBuffer[BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE],
                              SDMMCHOST_DMA_DESCRIPTOR_BUFFER_ALIGN_SIZE);
#if defined SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER && SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER
/* two cache line length for sdmmc host driver maintain unalign transfer */
SDK_ALIGN(static uint8_t s_sdmmcCacheLineAlignBuffer[BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE * 2U],
          BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE);
#endif
#if defined(SDIO_ENABLED) || defined(SD_ENABLED)
static sd_detect_card_t s_cd;
static sd_io_voltage_t s_ioVoltage = {
    .type = BOARD_SDMMC_SD_IO_VOLTAGE_CONTROL_TYPE,
    .func = BOARD_SDCardIoVoltageControl,
};
/* Card detect is wired to PCA9555 P0_4 (active-low). SD_PWREN is on PCAL6524
 * P2_5 driving an NX3P1100 load switch (active-high). Both IO expanders are
 * board-level peripherals; their handles, lockFunc, MCU GPIO INT ISR, and
 * PCA9555 INT GPIO pin setup all live in board.c. We just borrow the
 * singletons via BOARD_GetPCA9555Handle / BOARD_GetPCAL6524Handle and install
 * SDHC-specific per-pin callbacks on top. */
#endif
static sdmmchost_t s_host;

#ifdef SDIO_ENABLED
static sdio_card_int_t s_sdioInt;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
uint32_t BOARD_USDHC0ClockConfiguration(void)
{
    clock_root_config_t rootCfg = {0};

    /* USDHC0 root clock <- COMM_PERI1_DIV2 (mux 0) divided by 2. */
    rootCfg.mux = (uint32_t)kCLOCK_USDHC0_ClockRoot_COMM_PERI1_DIV2;
    rootCfg.div = 2U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_usdhc0_fclk, &rootCfg);
    CLOCK_EnableClock(kCLOCK_COMM_usdhc0);

    return CLOCK_GetRootClockFreq(kCLOCK_Root_COMM_usdhc0_fclk);
}

uint32_t BOARD_USDHC1ClockConfiguration(void)
{
    clock_root_config_t rootCfg = {0};

    /* USDHC1 (WiFi M.2 SDIO) root <- COMMPFDX_DIV2, div 4. */
    rootCfg.mux = (uint32_t)kCLOCK_USDHC1_ClockRoot_COMMPFDX_DIV2;
    rootCfg.div = 4U;
    CLOCK_SetRootClock(kCLOCK_Root_COMM_usdhc1_fclk, &rootCfg);
    CLOCK_EnableClock(kCLOCK_COMM_usdhc1);

    return CLOCK_GetRootClockFreq(kCLOCK_Root_COMM_usdhc1_fclk);
}

#if defined(SDIO_ENABLED) || defined(SD_ENABLED)
/*
 * Card detect on MIMXRT2660-EVK is wired to PCA9555 pin P0_4 (active-low:
 * 0 = card present). The PCA9555 INT line (EXP_nIRQ2) is open-drain and
 * connects to PIO3_29 (HSP_GPIO1 pin 29), giving a falling-edge on the
 * MCU side whenever any input on the PCA9555 changes state.
 *
 * The PCA9555 handle, lockFunc, MCU GPIO ISR, and INT pin enable/mux live
 * in board.c (BOARD_GetPCA9555Handle / BOARD_EnablePCA9555Interrupt /
 * BOARD_PCA9555_INT_IRQ_HANDLER). Here we just install the SDHC per-pin
 * callback on the singleton and let board.c handle the IRQ plumbing.
 */
bool BOARD_SDCardGetDetectStatus(void)
{
    uint16_t pins = 0U;

    if (PCA9555_ReadPins(BOARD_GetPCA9555Handle(), &pins) != kStatus_Success)
    {
        return false;
    }

    uint16_t level = (uint16_t)((pins >> BOARD_SDMMC_SD_CD_PCA9555_PIN) & 0x1U);
    return (level == BOARD_SDMMC_SD_CD_INSERT_LEVEL);
}

void SDMMC_SD_CD_Callback(void *param)
{
    (void)param;
    if (s_cd.callback != NULL)
    {
        s_cd.callback(BOARD_SDCardGetDetectStatus(), s_cd.userData);
    }
}

/*
 * PCA9555 per-pin driver callback: invoked from PCA9555_InterruptHandler
 * (which is driven from board.c's MCU GPIO ISR) for the card-detect pin
 * whenever its state changes. Forward the event into the SDMMC stack.
 */
static void BOARD_SDCardPCA9555Callback(uint8_t pin, bool pinState, void *userData)
{
    (void)pin;
    (void)pinState;
    SDMMC_SD_CD_Callback(userData);
}

void BOARD_SDCardDAT3PullFunction(uint32_t status)
{
    if (status == kSD_DAT3PullDown)
    {
        /* TODO: set DAT3 pad config for pull-down (RT2660 IOMUXC pad-config
         * bit layout differs from RT11xx; the right pull-down config value
         * is device-specific and must be verified against the RT2660 IOMUXC
         * reference). Mux+config are in one register on RT2660, so
         * IOMUXC_SetPin_Mux_Config re-applies both. */
        IOMUXC_SetPin_Mux_Config(IOMUXC_PIO7_11_COMM_uSDHC0_DAT3, 0U);
        BOARD_SDCardPowerControl(false);
        SDK_DelayAtLeastUs(1000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        BOARD_SDCardPowerControl(true);
        SDK_DelayAtLeastUs(1000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }
    else
    {
        /* TODO: set DAT3 pad config for pull-up (default DAT3 line state). */
        IOMUXC_SetPin_Mux_Config(IOMUXC_PIO7_11_COMM_uSDHC0_DAT3, 0U);
    }
}

void BOARD_SDCardDetectInit(sd_cd_t cd, void *userData)
{
    s_cd.cdDebounce_ms = BOARD_SDMMC_SD_CARD_DETECT_DEBOUNCE_DELAY_MS;
    s_cd.type          = BOARD_SDMMC_SD_CD_TYPE;
    s_cd.cardDetected  = BOARD_SDCardGetDetectStatus;
    s_cd.callback      = cd;
    s_cd.userData      = userData;

    if (BOARD_SDMMC_SD_CD_TYPE == kSD_DetectCardByGpioCD)
    {
        /* Bring up the shared PCA9555 singleton (lazy-inits on first call),
         * configure P0_4 as input, install the SDHC card-detect callback,
         * then let board.c enable the MCU GPIO INT line. After that, the
         * board.c-owned BOARD_PCA9555_INT_IRQ_HANDLER will drive
         * PCA9555_InterruptHandler → BOARD_SDCardPCA9555Callback →
         * SDMMC_SD_CD_Callback automatically on every card-detect edge. */
        pca9555_handle_t *cdHandle = BOARD_GetPCA9555Handle();
        (void)PCA9555_SetDirection(cdHandle,
                                   (uint16_t)(1U << BOARD_SDMMC_SD_CD_PCA9555_PIN),
                                   kPCA9555_Input);
        (void)PCA9555_InstallPinCallback(cdHandle,
                                         (uint8_t)BOARD_SDMMC_SD_CD_PCA9555_PIN,
                                         BOARD_SDCardPCA9555Callback,
                                         userData);
        BOARD_EnablePCA9555Interrupt();

        /* If a card is already present at boot, synthesize a one-shot
         * "inserted" event so the SDMMC stack starts consistent. Don't
         * fire cd(false, ...) on an empty slot — there was no prior
         * "inserted" state, and a spurious "removed" callback would
         * wake the SDMMC task (or worse, mislead its state machine).
         * Mirrors the evkbmimxrt1170 sdmmc_config.c pattern. */
        if ((cd != NULL) && BOARD_SDCardGetDetectStatus())
        {
            cd(true, userData);
        }
    }

    if (BOARD_SDMMC_SD_CD_TYPE == kSD_DetectCardByHostDATA3)
    {
        s_cd.dat3PullFunc = BOARD_SDCardDAT3PullFunction;
        BOARD_SDCardPowerControl(true);
    }
}

void BOARD_SDCardPowerResetInit(void)
{
    pcal6524_handle_t *pwrHandle = BOARD_GetPCAL6524Handle();

    /* Drive LOW before flipping direction so the card starts unpowered; the
     * SDMMC stack flips it on via BOARD_SDCardPowerControl(true) as needed. */
    (void)PCAL6524_ClearPins(pwrHandle, 1UL << BOARD_PCAL6524_SD_PWREN);
    (void)PCAL6524_SetDirection(pwrHandle, 1UL << BOARD_PCAL6524_SD_PWREN, kPCAL6524_Output);
}

void BOARD_SDCardPowerControl(bool enable)
{
    pcal6524_handle_t *pwrHandle = BOARD_GetPCAL6524Handle();

    if (enable)
    {
        (void)PCAL6524_SetPins(pwrHandle, 1UL << BOARD_PCAL6524_SD_PWREN);
    }
    else
    {
        (void)PCAL6524_ClearPins(pwrHandle, 1UL << BOARD_PCAL6524_SD_PWREN);
    }
}

void BOARD_SDCardIoVoltageControlInit(void)
{
    /* Start at 3.3V (level-shifter VSELECT LOW). SD cards initialize at
     * 3.3V; the SDMMC stack flips this to 1.8V via the voltage callback
     * once UHS-I signaling is negotiated. */
    const gpio_pin_config_t vselectConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 0U,
    };
    GPIO_PinInit(BOARD_SDMMC_SD_VSELECT_GPIO, BOARD_SDMMC_SD_VSELECT_GPIO_PIN, &vselectConfig);
}

void BOARD_SDCardIoVoltageControl(sdmmc_operation_voltage_t voltage)
{
    if (voltage == kSDMMC_OperationVoltage330V)
    {
        GPIO_PinWrite(BOARD_SDMMC_SD_VSELECT_GPIO, BOARD_SDMMC_SD_VSELECT_GPIO_PIN, 0U);
    }
    else if (voltage == kSDMMC_OperationVoltage180V)
    {
        GPIO_PinWrite(BOARD_SDMMC_SD_VSELECT_GPIO, BOARD_SDMMC_SD_VSELECT_GPIO_PIN, 1U);
    }
}
#endif

#ifdef SD_ENABLED
void BOARD_SD_Config(void *card, sd_cd_t cd, uint32_t hostIRQPriority, void *userData)
{
    assert(card);

    s_host.dmaDesBuffer         = s_sdmmcHostDmaBuffer;
    s_host.dmaDesBufferWordsNum = BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE;
#if ((defined __DCACHE_PRESENT) && __DCACHE_PRESENT) || (defined FSL_FEATURE_HAS_L1CACHE && FSL_FEATURE_HAS_L1CACHE)
    s_host.enableCacheControl = BOARD_SDMMC_HOST_CACHE_CONTROL;
#endif
#if defined SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER && SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER
    s_host.cacheAlignBuffer     = s_sdmmcCacheLineAlignBuffer;
    s_host.cacheAlignBufferSize = BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE * 2U;
#endif

    ((sd_card_t *)card)->host                                = &s_host;
    ((sd_card_t *)card)->host->hostController.base           = BOARD_SDMMC_SD_HOST_BASEADDR;
    ((sd_card_t *)card)->host->hostController.sourceClock_Hz = BOARD_USDHC0ClockConfiguration();

    ((sd_card_t *)card)->usrParam.cd         = &s_cd;
    ((sd_card_t *)card)->usrParam.pwr        = BOARD_SDCardPowerControl;
    ((sd_card_t *)card)->usrParam.ioStrength = NULL;
    ((sd_card_t *)card)->usrParam.ioVoltage  = &s_ioVoltage;
    ((sd_card_t *)card)->usrParam.maxFreq    = BOARD_SDMMC_SD_HOST_SUPPORT_SDR104_FREQ;

    BOARD_SDCardPowerResetInit();
    BOARD_SDCardIoVoltageControlInit();
    BOARD_SDCardDetectInit(cd, userData);

    NVIC_SetPriority(BOARD_SDMMC_SD_HOST_IRQ, hostIRQPriority);
}
#endif

#ifdef SDIO_ENABLED
void BOARD_SDIO_Config(void *card, sd_cd_t cd, uint32_t hostIRQPriority, sdio_int_t cardInt)
{
    assert(card);

    s_host.dmaDesBuffer         = s_sdmmcHostDmaBuffer;
    s_host.dmaDesBufferWordsNum = BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE;
#if ((defined __DCACHE_PRESENT) && __DCACHE_PRESENT) || (defined FSL_FEATURE_HAS_L1CACHE && FSL_FEATURE_HAS_L1CACHE)
    s_host.enableCacheControl = BOARD_SDMMC_HOST_CACHE_CONTROL;
#endif
#if defined SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER && SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER
    s_host.cacheAlignBuffer     = s_sdmmcCacheLineAlignBuffer;
    s_host.cacheAlignBufferSize = BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE * 2U;
#endif

    ((sdio_card_t *)card)->host                                = &s_host;
    ((sdio_card_t *)card)->host->hostController.base           = BOARD_SDMMC_SDIO_HOST_BASEADDR;
    ((sdio_card_t *)card)->host->hostController.sourceClock_Hz = BOARD_USDHC0ClockConfiguration();

    ((sdio_card_t *)card)->usrParam.cd         = &s_cd;
    ((sdio_card_t *)card)->usrParam.pwr        = NULL;
    ((sdio_card_t *)card)->usrParam.ioStrength = NULL;
    ((sdio_card_t *)card)->usrParam.ioVoltage  = &s_ioVoltage;
    ((sdio_card_t *)card)->usrParam.maxFreq    = BOARD_SDMMC_SD_HOST_SUPPORT_SDR104_FREQ;
    if (cardInt != NULL)
    {
        s_sdioInt.cardInterrupt                 = cardInt;
        ((sdio_card_t *)card)->usrParam.sdioInt = &s_sdioInt;
    }

    BOARD_SDCardDetectInit(cd, NULL);

    NVIC_SetPriority(BOARD_SDMMC_SDIO_HOST_IRQ, hostIRQPriority);
}
#endif

#ifdef MMC_ENABLED
void BOARD_MMC_Config(void *card, uint32_t hostIRQPriority)
{
    assert(card);

    s_host.dmaDesBuffer         = s_sdmmcHostDmaBuffer;
    s_host.dmaDesBufferWordsNum = BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE;
#if ((defined __DCACHE_PRESENT) && __DCACHE_PRESENT) || (defined FSL_FEATURE_HAS_L1CACHE && FSL_FEATURE_HAS_L1CACHE)
    s_host.enableCacheControl = BOARD_SDMMC_HOST_CACHE_CONTROL;
#endif
#if defined SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER && SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER
    s_host.cacheAlignBuffer     = s_sdmmcCacheLineAlignBuffer;
    s_host.cacheAlignBufferSize = BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE * 2U;
#endif

    ((mmc_card_t *)card)->host                                = &s_host;
    ((mmc_card_t *)card)->host->hostController.base           = BOARD_SDMMC_MMC_HOST_BASEADDR;
    ((mmc_card_t *)card)->host->hostController.sourceClock_Hz = BOARD_USDHC0ClockConfiguration();
    ((mmc_card_t *)card)->host->tuningType                    = BOARD_SDMMC_MMC_TUNING_TYPE;
    ((mmc_card_t *)card)->usrParam.ioStrength                 = NULL;
    ((mmc_card_t *)card)->usrParam.maxFreq                    = BOARD_SDMMC_MMC_HOST_SUPPORT_HS200_FREQ;

    ((mmc_card_t *)card)->usrParam.capability |= BOARD_SDMMC_MMC_SUPPORT_8_BIT_DATA_WIDTH;

    ((mmc_card_t *)card)->hostVoltageWindowVCC  = BOARD_SDMMC_MMC_VCC_SUPPLY;
    ((mmc_card_t *)card)->hostVoltageWindowVCCQ = BOARD_SDMMC_MMC_VCCQ_SUPPLY;

    NVIC_SetPriority(BOARD_SDMMC_MMC_HOST_IRQ, hostIRQPriority);
}
#endif
