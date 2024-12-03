/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_cache.h"
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#include "fsl_lpi2c.h"
#endif
#if defined(BOARD_USE_PCAL6524) && BOARD_USE_PCAL6524
#include "fsl_pcal6524.h"
#endif /* BOARD_USE_PCAL6524 */
#include "hal_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialize debug console. */
void BOARD_InitDebugConsoleWithArgs(unsigned int uart_instance, unsigned int uart_clk_freq, hal_clk_t *p_hal_clk)
{
    HAL_ClockSetRootClk(p_hal_clk);
    DbgConsole_Init(uart_instance, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE,
                    uart_clk_freq);
}

void BOARD_InitDebugConsoleForCM7WithSM(void)
{
    /* clang-format off */
    hal_clk_t hal_clk = {
        .clk_id = BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITH_SM,
	.pclk_id = hal_clock_osc24m,
	.div = 1,
	.enable_clk = true,
	.clk_round_opt = hal_clk_round_auto,
    };
    /* clang-format on */

    //BOARD_InitDebugConsoleWithArgs(BOARD_DEBUG_UART_INSTANCE_FOR_CM7_WITH_SM, BOARD_DEBUG_UART_CLK_FREQ_FOR_CM7_WITH_SM, &hal_clk);
    /* Don't know why the demo's uart is not workable when call BOARD_InitDebugConsoleWithArgs directly on zebu for cortex-m7  */
    HAL_ClockSetRootClk(&hal_clk);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE_FOR_CM7_WITH_SM, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE,
                    BOARD_DEBUG_UART_CLK_FREQ_FOR_CM7_WITH_SM);
}

void BOARD_InitDebugConsoleForCM7WithoutSM(void)
{
    /* clang-format off */
    hal_clk_t hal_clk = {
        .clk_id = BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITHOUT_SM,
	.pclk_id = hal_clock_osc24m,
	.div = 1,
	.enable_clk = true,
	.clk_round_opt = hal_clk_round_auto,
    };
    /* clang-format on */

    //BOARD_InitDebugConsoleWithArgs(BOARD_DEBUG_UART_INSTANCE_FOR_CM7_WITHOUT_SM, BOARD_DEBUG_UART_CLK_FREQ_FOR_CM7_WITHOUT_SM, &hal_clk);
    /* Don't know why the demo's uart is not workable when call BOARD_InitDebugConsoleWithArgs directly on zebu for cortex-m7  */
    HAL_ClockSetRootClk(&hal_clk);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE_FOR_CM7_WITHOUT_SM, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE,
                    BOARD_DEBUG_UART_CLK_FREQ_FOR_CM7_WITHOUT_SM);
}

void BOARD_InitDebugConsole(void)
{
    /* clang-format off */
    hal_clk_t hal_clk = {
        .clk_id = BOARD_DEBUG_UART_CLOCK_ROOT,
	.pclk_id = hal_clock_osc24m,
	.div = 1,
	.enable_clk = true,
	.clk_round_opt = hal_clk_round_auto,
    };
    /* clang-format on */

    //BOARD_InitDebugConsoleWithArgs(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_CLK_FREQ, &hal_clk);
    /* Don't know why the demo's uart is not workable when call BOARD_InitDebugConsoleWithArgs directly on zebu for cortex-m7  */
    HAL_ClockSetRootClk(&hal_clk);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE,
                    BOARD_DEBUG_UART_CLK_FREQ);
}

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz)
{
    lpi2c_master_config_t lpi2cConfig = {0};

    /*
     * lpi2cConfig.debugEnable = false;
     * lpi2cConfig.ignoreAck = false;
     * lpi2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * lpi2cConfig.baudRate_Hz = 100000U;
     * lpi2cConfig.busIdleTimeout_ns = 0;
     * lpi2cConfig.pinLowTimeout_ns = 0;
     * lpi2cConfig.sdaGlitchFilterWidth_ns = 0;
     * lpi2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&lpi2cConfig);
    LPI2C_MasterInit(base, &lpi2cConfig, clkSrc_Hz);
}

status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subAddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize,
                          uint32_t flags)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = flags;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subAddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize,
                             uint32_t flags)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = flags;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = rxBuff;
    xfer.dataSize       = rxBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t BOARD_Display_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(LPI2C2, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize, 0);
}

status_t BOARD_Display_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(LPI2C2, deviceAddress, subAddress, subAddressSize, rxBuff,
                               rxBuffSize, 0);
}

#if defined(BOARD_USE_ADP5585) && BOARD_USE_ADP5585
void BOARD_ADP5585_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_ADP5585_I2C, BOARD_ADP5585_I2C_CLOCK_FREQ);
}

status_t BOARD_ADP5585_I2C_Send(uint8_t deviceAddress,
                                uint32_t subAddress,
                                uint8_t subAddressSize,
                                const uint8_t *txBuff,
                                uint8_t txBuffSize,
                                uint32_t flags)
{
    return BOARD_LPI2C_Send(BOARD_ADP5585_I2C, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff, txBuffSize,
                            flags);
}

status_t BOARD_ADP5585_I2C_Receive(uint8_t deviceAddress,
                                   uint32_t subAddress,
                                   uint8_t subAddressSize,
                                   uint8_t *rxBuff,
                                   uint8_t rxBuffSize,
                                   uint32_t flags)
{
    return BOARD_LPI2C_Receive(BOARD_ADP5585_I2C, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize, flags);
}

void BOARD_InitADP5585(adp5585_handle_t *handle)
{
    BOARD_ADP5585_I2C_Init();

    static const adp5585_config_t config = {
        .i2cAddr         = BOARD_ADP5585_I2C_ADDR,
        .I2C_SendFunc    = BOARD_ADP5585_I2C_Send,
        .I2C_ReceiveFunc = BOARD_ADP5585_I2C_Receive,
    };

    ADP5585_Init(handle, &config);
}

#endif /* BOARD_USE_ADP5585. */

#if defined(BOARD_USE_PCAL6524) && BOARD_USE_PCAL6524
void BOARD_PCAL6524_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_PCAL6524_I2C, BOARD_PCAL6524_I2C_CLOCK_FREQ);
}

status_t BOARD_PCAL6524_I2C_Send(uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 const uint8_t *txBuff,
                                 uint8_t txBuffSize,
                                 uint32_t flags)
{
    return BOARD_LPI2C_Send(BOARD_PCAL6524_I2C, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize, flags);
}

status_t BOARD_PCAL6524_I2C_Receive(uint8_t deviceAddress,
                                    uint32_t subAddress,
                                    uint8_t subAddressSize,
                                    uint8_t *rxBuff,
                                    uint8_t rxBuffSize,
                                    uint32_t flags)
{
    return BOARD_LPI2C_Receive(BOARD_PCAL6524_I2C, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize,
                               flags);
}

void BOARD_InitPCAL6524(pcal6524_handle_t *handle)
{
    BOARD_PCAL6524_I2C_Init();

    static const pcal6524_config_t config = {
        .i2cAddr         = BOARD_PCAL6524_I2C_ADDR,
        .I2C_SendFunc    = BOARD_PCAL6524_I2C_Send,
        .I2C_ReceiveFunc = BOARD_PCAL6524_I2C_Receive,
    };

    PCAL6524_Init(handle, &config);
}

#endif /* BOARD_USE_PCAL6524. */

#if defined(BOARD_USE_PCAL6408) && BOARD_USE_PCAL6408
void BOARD_PCAL6408_I2C_Init(void *base, uint32_t clkSrc_Hz)
{
    BOARD_LPI2C_Init(base, clkSrc_Hz);
}

status_t BOARD_PCAL6408_I2C_Send(void *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 const uint8_t *txBuff,
                                 uint8_t txBuffSize,
                                 uint32_t flags)
{
    return BOARD_LPI2C_Send(base, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize, flags);
}

status_t BOARD_PCAL6408_I2C_Receive(void *base,
                                    uint8_t deviceAddress,
                                    uint32_t subAddress,
                                    uint8_t subAddressSize,
                                    uint8_t *rxBuff,
                                    uint8_t rxBuffSize,
                                    uint32_t flags)
{
    return BOARD_LPI2C_Receive(base, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize,
                               flags);
}

void BOARD_InitPCAL6408_I2C1(pcal6408_handle_t *handle)
{
    BOARD_PCAL6408_I2C_Init(BOARD_PCAL6408_I2C1, BOARD_PCAL6408_I2C1_CLOCK_FREQ);

    static const pcal6408_config_t config = {
        .i2cBase         = BOARD_PCAL6408_I2C1,
        .i2cAddr         = BOARD_PCAL6408_I2C1_ADDR,
        .I2C_SendFunc    = BOARD_PCAL6408_I2C_Send,
        .I2C_ReceiveFunc = BOARD_PCAL6408_I2C_Receive,
    };

    PCAL6408_Init(handle, &config);
}

void BOARD_InitPCAL6408_I2C3(pcal6408_handle_t *handle)
{
    BOARD_PCAL6408_I2C_Init(BOARD_PCAL6408_I2C3, BOARD_PCAL6408_I2C3_CLOCK_FREQ);

    static const pcal6408_config_t config = {
        .i2cBase         = BOARD_PCAL6408_I2C3,
        .i2cAddr         = BOARD_PCAL6408_I2C3_ADDR,
        .I2C_SendFunc    = BOARD_PCAL6408_I2C_Send,
        .I2C_ReceiveFunc = BOARD_PCAL6408_I2C_Receive,
    };

    PCAL6408_Init(handle, &config);
}

void BOARD_InitPCAL6408_I2C4(pcal6408_handle_t *handle)
{
    BOARD_PCAL6408_I2C_Init(BOARD_PCAL6408_I2C4, BOARD_PCAL6408_I2C4_CLOCK_FREQ);

    static const pcal6408_config_t config = {
        .i2cBase         = BOARD_PCAL6408_I2C4,
        .i2cAddr         = BOARD_PCAL6408_I2C4_ADDR,
        .I2C_SendFunc    = BOARD_PCAL6408_I2C_Send,
        .I2C_ReceiveFunc = BOARD_PCAL6408_I2C_Receive,
    };

    PCAL6408_Init(handle, &config);
}

void BOARD_InitPCAL6408_I2C5(pcal6408_handle_t *handle)
{
    BOARD_PCAL6408_I2C_Init(BOARD_PCAL6408_I2C5, BOARD_PCAL6408_I2C5_CLOCK_FREQ);

    static const pcal6408_config_t config = {
        .i2cBase         = BOARD_PCAL6408_I2C5,
        .i2cAddr         = BOARD_PCAL6408_I2C5_ADDR,
        .I2C_SendFunc    = BOARD_PCAL6408_I2C_Send,
        .I2C_ReceiveFunc = BOARD_PCAL6408_I2C_Receive,
    };

    PCAL6408_Init(handle, &config);
}
#endif /* BOARD_USE_PCAL6408. */

#if defined(BOARD_USE_PCA6416A) && BOARD_USE_PCA6416A
void BOARD_PCA6416A_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_PCA6416A_I2C, BOARD_PCA6416A_I2C_CLOCK_FREQ);
}

status_t BOARD_PCA6416A_I2C_Send(uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 const uint8_t *txBuff,
                                 uint8_t txBuffSize,
                                 uint32_t flags)
{
    return BOARD_LPI2C_Send(BOARD_PCA6416A_I2C, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize, flags);
}

status_t BOARD_PCA6416A_I2C_Receive(uint8_t deviceAddress,
                                    uint32_t subAddress,
                                    uint8_t subAddressSize,
                                    uint8_t *rxBuff,
                                    uint8_t rxBuffSize,
                                    uint32_t flags)
{
    return BOARD_LPI2C_Receive(BOARD_PCA6416A_I2C, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize,
                               flags);
}

void BOARD_InitPCA6416A(pca6416a_handle_t *handle)
{
    BOARD_PCA6416A_I2C_Init();

    static const pca6416a_config_t config = {
        .i2cAddr         = BOARD_PCA6416A_I2C_ADDR,
        .I2C_SendFunc    = BOARD_PCA6416A_I2C_Send,
        .I2C_ReceiveFunc = BOARD_PCA6416A_I2C_Receive,
    };

    PCA6416A_Init(handle, &config);
}
#endif /* BOARD_USE_PCA6416A. */
#endif /* SDK_I2C_BASED_COMPONENT_USED */

void BOARD_McoreSUSPEND(void)
{
    uint32_t value = 0U;
    /* Config OSCPLL LPM setting for M33 SUSPEND */
    for (unsigned int i = OSCPLL_LPM_START; i <= OSCPLL_LPM_END; i++)
    {
        CCM_CTRL->OSCPLL[i].LPM0 |= CCM_OSCPLL_LPM0_LPM_SETTING_D2_MASK;
    }

    /* Config GPC_CTRL_CM33 to set Mcore as SUSPEND mode */
    GPC_CTRL_CM33->CMC_MODE_CTRL |= GPC_CMC_MODE_CTRL_CPU_MODE_TARGET_MASK;

    /* Stop SysTick and enter WFI
     * System will goes into system sleep flow
     * The STBY LED on board will light with red color
     */
    /* Save old value of SYST_CSR */
    value            = SYSTICK_CTRL_REG;
    SYSTICK_CTRL_REG = SYSTICK_CLOSE;
    __WFI();
    /* Enable systick */
    SYSTICK_CTRL_REG = value;
}

#if __CORTEX_M == 7
void BOARD_ConfigMPU(void)
{
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RW_m_ncache$$Base[];
    /* RW_m_ncache_unused is a auxiliary region which is used to get the whole size of noncache section */
    extern uint32_t Image$$RW_m_ncache_unused$$Base[];
    extern uint32_t Image$$RW_m_ncache_unused$$ZI$$Limit[];
    uint32_t nonCacheStart = (uint32_t)Image$$RW_m_ncache$$Base;
    uint32_t size          = ((uint32_t)Image$$RW_m_ncache_unused$$Base == nonCacheStart) ?
                        0 :
                        ((uint32_t)Image$$RW_m_ncache_unused$$ZI$$Limit - nonCacheStart);
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __NCACHE_REGION_START[];
    extern uint32_t __NCACHE_REGION_SIZE[];
    uint32_t nonCacheStart = (uint32_t)__NCACHE_REGION_START;
    uint32_t size          = (uint32_t)__NCACHE_REGION_SIZE;
#endif
    volatile uint32_t i = 0;

#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
    }
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
    }
#endif

    /* Disable MPU */
    ARM_MPU_Disable();

    /* MPU configure:
     * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable,
     * SubRegionDisable, Size)
     * API in mpu_armv7.h.
     * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches
     * disabled.
     * param AccessPermission  Data access permissions, allows you to configure read/write access for User and
     * Privileged mode.
     *      Use MACROS defined in mpu_armv7.h:
     * ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
     * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
     *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribute    Shareability        Cache
     *     0             x           0           0             Strongly Ordered    shareable
     *     0             x           0           1              Device             shareable
     *     0             0           1           0              Normal             not shareable   Outer and inner write
     * through no write allocate
     *     0             0           1           1              Normal             not shareable   Outer and inner write
     * back no write allocate
     *     0             1           1           0              Normal             shareable       Outer and inner write
     * through no write allocate
     *     0             1           1           1              Normal             shareable       Outer and inner write
     * back no write allocate
     *     1             0           0           0              Normal             not shareable   outer and inner
     * noncache
     *     1             1           0           0              Normal             shareable       outer and inner
     * noncache
     *     1             0           1           1              Normal             not shareable   outer and inner write
     * back write/read acllocate
     *     1             1           1           1              Normal             shareable       outer and inner write
     * back write/read acllocate
     *     2             x           0           0              Device              not shareable
     *  Above are normal use settings, if your want to see more details or want to config different inner/outer cache
     * policy.
     *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
     * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
     * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in
     * mpu_armv7.h.
     */

    /*
     * Add default region to deny access to whole address space to workaround speculative prefetch.
     * Refer to Arm errata 1013783-B for more details.
     *
     */
    /* Region 0 setting: Instruction access disabled, No data access permission. */
    MPU->RBAR = ARM_MPU_RBAR(0, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(1, ARM_MPU_AP_NONE, 0, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_4GB);

    /* Region 1 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 2 setting: Memory with Device type, not shareable,  non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x4C800000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 3 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 4 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

    /* Region 5 setting: Memory with Normal type, shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(5, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 1, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

#if defined(CACHE_MODE_WRITE_THROUGH) && CACHE_MODE_WRITE_THROUGH
    /* Region 6 setting: Memory with Normal type, not shareable, write through */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 0, 0, ARM_MPU_REGION_SIZE_1MB);
#else
    /* Region 6 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_1MB);
#endif

    /* Region 8 setting: Memory with Normal type, not shareable, outer/inner write back. */
    MPU->RBAR = ARM_MPU_RBAR(8, 0x28000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128MB);

#ifdef USE_SDRAM
#if defined(CACHE_MODE_WRITE_THROUGH) && CACHE_MODE_WRITE_THROUGH
    /* Region 9 setting: Memory with Normal type, not shareable, write trough */
    MPU->RBAR = ARM_MPU_RBAR(9, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 0, 0, ARM_MPU_REGION_SIZE_64MB);
#else
    /* Region 9 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(9, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_64MB);
#endif
#endif

    while ((size >> i) > 0x1U)
    {
        i++;
    }

    if (i != 0)
    {
        /* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
        //assert(!(nonCacheStart % size));
        assert(size == (uint32_t)(1 << i));
        assert(i >= 5);

        /* Region 10 setting: Memory with Normal type, shareable, non-cacheable */
        MPU->RBAR = ARM_MPU_RBAR(10, nonCacheStart);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 1, 0, 0, 0, i - 1);
    }

    /* Region 11 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(11, 0x40000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512KB);

    /* Region 12 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(12, 0x42000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_64MB);

    /* Region 13 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(13, 0x49000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_4MB);

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);

    /* Enable I cache and D cache */
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    SCB_EnableDCache();
#endif
#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    SCB_EnableICache();
#endif
}
#elif __CORTEX_M == 33
void BOARD_ConfigMPU(void)
{
    uint8_t attr;

    /* Disable code cache(ICache) and system cache(DCache) */
    XCACHE_DisableCache(M33_CACHE_CTRLPC);
    XCACHE_DisableCache(M33_CACHE_CTRLPS);

    /* NOTE: All TCRAM is non-cacheable regardless of MPU setting. */

    /*
     * default cache policy(default memory access behavior) after enable mpu on cortex-m33(according to RM of
     * cortex-m33): 0x00000000-0x1FFFFFFF Normal memory, Non-shareable, Write-Through, not Write Allocate
     * 0x20000000-0x3FFFFFFF Normal memory, Non-shareable, Write-Back, Write Allocate
     * 0x40000000-0x5FFFFFFF Device, Shareable
     * 0x60000000-0x7FFFFFFF Normal memory, Non-shareable, Write-Back, Write Allocate
     * 0x80000000-0x9FFFFFFF Normal memory, Non-shareable, Write-Through, not Write Allocate
     * 0xA0000000-0xDFFFFFFF Device, Shareable
     * 0xE0000000-0xE003FFFF Device, Shareable
     * 0xE0040000-0xE0043FFF Device, Shareable
     * 0xE0044000-0xE00EFFFF Device, Shareable
     * 0xF0000000-0xFFFFFFFF Device, Shareable
     */
    /* Disable MPU */
    ARM_MPU_Disable();

    /* Attr0: Device-nGnRnE */
    ARM_MPU_SetMemAttr(0U, ARM_MPU_ATTR(ARM_MPU_ATTR_DEVICE, ARM_MPU_ATTR_DEVICE));

    /* Attr1: Normal memory, Outer non-cacheable, Inner non-cacheable */
    ARM_MPU_SetMemAttr(1U, ARM_MPU_ATTR(ARM_MPU_ATTR_NON_CACHEABLE, ARM_MPU_ATTR_NON_CACHEABLE));

    /* Attr2: Normal memory, Inner write-through transient, read allocate. Inner write-through transient, read allocate
     */
    attr = ARM_MPU_ATTR_MEMORY_(0U, 0U, 1U, 0U);
    ARM_MPU_SetMemAttr(2U, ARM_MPU_ATTR(attr, attr));

    /* Attr3: Normal memory, Outer write-back transient, read/write allocate. Inner write-back transient, read/write
     * allocate */
    attr = ARM_MPU_ATTR_MEMORY_(0U, 1U, 1U, 1U);
    ARM_MPU_SetMemAttr(3U, ARM_MPU_ATTR(attr, attr));

        /* Attr0: device. */
    ARM_MPU_SetMemAttr(4U, ARM_MPU_ATTR(ARM_MPU_ATTR_DEVICE, ARM_MPU_ATTR_DEVICE));

    /*
     * Change macro definitions as follows when choose cache policy as non-cacheable:
     * #define DDR_NONCACHEABLE (1U)
     * #define DDR_WRITE_THROUGH (0U)
     * #define DDR_WRITE_BACK (0U)
     *
     *
     * Change macro definitions as follows when choose cache policy as Write-Through:
     * #define DDR_NONCACHEABLE (0U)
     * #define DDR_WRITE_THROUGH (1U)
     * #define DDR_WRITE_BACK (0U)
     *
     *
     * Change macro definitions as follows when choose cache policy as Write-Back:
     * #define DDR_NONCACHEABLE (0U)
     * #define DDR_WRITE_THROUGH (0U)
     * #define DDR_WRITE_BACK (1U)
     */
#define DDR_NONCACHEABLE  (1U)
#define DDR_WRITE_THROUGH (0U)
#define DDR_WRITE_BACK    (0U)
#if DDR_NONCACHEABLE
    /* NOTE: DDR is used as shared memory for A/M core communication, set it to non-cacheable. */
    /* Region 0: [0x80000000, 0xDFFFFFFF](DRAM), outer shareable, read/write, any privileged, executable. Attr 1
     * (non-cacheable). */
    ARM_MPU_SetRegion(0U, ARM_MPU_RBAR(0x80000000, ARM_MPU_SH_OUTER, 0U, 1U, 0U), ARM_MPU_RLAR(0xDFFFFFFF, 1U));
#elif DDR_WRITE_THROUGH
    /* Region 0: [0x80000000, 0xDFFFFFFF](DRAM), outer shareable, read/write, any privileged, executable. Attr 2
     * (Normal memory, Inner write-through transient, read allocate. Inner write-through transient, read allocate). */
    ARM_MPU_SetRegion(0U, ARM_MPU_RBAR(0x80000000, ARM_MPU_SH_OUTER, 0U, 1U, 0U), ARM_MPU_RLAR(0xDFFFFFFF, 2U));
#elif DDR_WRITE_BACK
    /* Region 0: [0x80000000, 0xDFFFFFFF](DRAM), outer shareable, read/write, any privileged, executable. Attr 3
     * (Normal memory, Outer write-back transient, read/write allocate. Inner write-back transient, read/write
     * allocate). */
    ARM_MPU_SetRegion(0U, ARM_MPU_RBAR(0x80000000, ARM_MPU_SH_OUTER, 0U, 1U, 0U), ARM_MPU_RLAR(0xDFFFFFFF, 3U));
#endif
    ARM_MPU_SetRegion(1U, ARM_MPU_RBAR(0x28000000, ARM_MPU_SH_OUTER, 0U, 1U, 0U), ARM_MPU_RLAR(0x3FFFFFFF, 4U));

    /* Enable MPU(use default memory map when access the memory within region) */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable ICache and DCache */
    XCACHE_EnableCache(M33_CACHE_CTRLPC);
    XCACHE_EnableCache(M33_CACHE_CTRLPS);
    /* flush pipeline */
    __DSB();
    __ISB();
}
#endif
