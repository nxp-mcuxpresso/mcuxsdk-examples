/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "board.h"
#include "fsl_debug_console.h"
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#include "fsl_lpi2c.h"
#endif /* SDK_I2C_BASED_COMPONENT_USED */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_ConfigMPU(void)
{
#if defined(SDK_SRAM_NONCACHE_SECTION) /* If Non-cacheable section is located in SRAM. */
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RW_m_ncache$$Base[];
    /* RW_m_ncache_unused is a auxiliary region which is used to get the whole size of noncache section */
    extern uint32_t Image$$RW_m_ncache_unused$$Base[];
    extern uint32_t Image$$RW_m_ncache_unused$$ZI$$Limit[];
    extern uint32_t Image$$RW_m_ncache_aux$$Base[];
    uint32_t nonCacheStart = (uint32_t)Image$$RW_m_ncache$$Base;
    uint32_t nonCacheSize  = ((uint32_t)Image$$RW_m_ncache_unused$$Base == nonCacheStart) ?
                                 0 :
                                 ((uint32_t)Image$$RW_m_ncache_unused$$ZI$$Limit - nonCacheStart);
#elif defined(__MCUXPRESSO)
    extern uint32_t __base_NCACHE_REGION;
    extern uint32_t __top_NCACHE_REGION;
    uint32_t nonCacheStart = (uint32_t)(&__base_NCACHE_REGION);
    uint32_t nonCacheSize  = (uint32_t)(&__top_NCACHE_REGION) - nonCacheStart;
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __NCACHE_REGION_START[];
    extern uint32_t __NCACHE_REGION_SIZE[];
    uint32_t nonCacheStart = (uint32_t)__NCACHE_REGION_START;
    uint32_t nonCacheSize  = (uint32_t)__NCACHE_REGION_SIZE;
#endif

    volatile uint32_t i;
#endif

   /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
    }
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
    }

    /* Disable MPU */
    ARM_MPU_Disable();

    /* clang-format off */

    /* MPU configure:
     * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable, SubRegionDisable, Size)
     * API in mpu_armv7.h.
     * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches disabled.
     * param AccessPermission  Data access permissions, allows you to configure read/write access for User and Privileged mode.
     *                         Use MACROS defined in mpu_armv7.h:
     *                         ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
     *
     * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
     *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribute    Shareability        Cache
     *     0             x           0           0             Strongly Ordered    shareable
     *     0             x           0           1              Device             shareable
     *     0             0           1           0              Normal             not shareable   Outer and inner write
     *                                                                                             through no write allocate
     *     0             0           1           1              Normal             not shareable   Outer and inner write
     *                                                                                             back no write allocate
     *     0             1           1           0              Normal             shareable       Outer and inner write
     *                                                                                             through no write allocate
     *     0             1           1           1              Normal             shareable       Outer and inner write
     *                                                                                             back no write allocate
     *     1             0           0           0              Normal             not shareable   outer and inner
     *                                                                                             noncache
     *     1             1           0           0              Normal             shareable       outer and inner
     *                                                                                             noncache
     *     1             0           1           1              Normal             not shareable   outer and inner write
     *                                                                                             back write/read acllocate
     *     1             1           1           1              Normal             shareable       outer and inner write
     *                                                                                             back write/read acllocate
     *     2             x           0           0              Device             not shareable
     *   Above are normal use settings, if your want to see more details or want to config different inner/outer cache
     * policy, please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
     *
     * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
     * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in mpu_armv7.h.
     */

    /* clang-format on */

    /*
     * Add default region to deny access to whole address space to workaround speculative prefetch.
     * Refer to Arm errata 1013783-B for more details.
     */
    /* Region 0 setting: Instruction access disabled, No data access permission. */
    MPU->RBAR = ARM_MPU_RBAR(0, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(1, ARM_MPU_AP_NONE, 0, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_4GB);

    /* Region 1 setting: Memory with Normal type, not shareable, outer/inner noncache, ITCM */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_32KB);

    /* Region 2 setting: Memory with Normal type, not shareable, outer/inner write back, Flash */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x00400000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_4MB);
    
    /* Region 3 setting: Memory with Normal type, not shareable, outer/inner write back, DFlash */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x10000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

    /* Region 4 setting: Memory with Normal type, not shareable, outer/inner noncache, ITCM backdoor */
    MPU->RBAR = ARM_MPU_RBAR(4, 0x11000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_32KB);

    /* Region 5 setting: Memory with Normal type, not shareable, outer/inner noncache, ITCM1 backdoor */
    MPU->RBAR = ARM_MPU_RBAR(5, 0x11400000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_32KB);

    /* Region 6 setting: Memory with Normal type, not shareable, outer/inner noncache, DTCM */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_64KB);

    /* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back, SRAM */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x20400000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512KB);

    /* Region 8 setting: Memory with Normal type, not shareable, outer/inner noncache, DTCM backdoor */
    MPU->RBAR = ARM_MPU_RBAR(8, 0x21000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_64KB);
    
    /* Region 9 setting: Memory with Normal type, not shareable, outer/inner noncache, DTCM1 backdoor */
    MPU->RBAR = ARM_MPU_RBAR(9, 0x21400000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_64KB);

    /* Region 10 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(10, 0x40000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 11 setting: Memory with Device type, not shareable, non-cacheable, QSPI RX buffer. */
    MPU->RBAR = ARM_MPU_RBAR(11, 0x67000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1KB);

#if defined(SDK_USE_QSPI) /* Only configure QSPI memory when used. Refer to Arm errata 1013783-B */
    /* Region 12 setting: Memory with Normal type, not shareable, outer/inner write back, QSPI AHB */
    MPU->RBAR = ARM_MPU_RBAR(12, 0x68000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128MB);
#endif

#if defined(SDK_SRAM_NONCACHE_SECTION)
    i = 0;
    while ((nonCacheSize >> i) > 0x1U)
    {
        i++;
    }

    if (i != 0)
    {
        /* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
        assert(!(nonCacheStart % nonCacheSize));
        assert(nonCacheSize == (uint32_t)(1 << i));
        assert(i >= 5);

        /* Region 11 setting: Memory with Normal type, not shareable, non-cacheable */
        MPU->RBAR = ARM_MPU_RBAR(11, nonCacheStart);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, i - 1);
    }
#endif

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);

    /* Enable I cache and D cache */
    SCB_EnableDCache();
    SCB_EnableICache();
}

void BOARD_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq;
    
    uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

/* Don't access system RAM when configuring PRAM FT_DIS.  */
AT_QUICKACCESS_SECTION_CODE(void BOARD_EnableSRAMExtraLatency(bool en))
{
    if (en)
    {
        /* Configure SRAM read wait states. */
        PRAMC_0->PRCR1 |= PRAMC_PRCR1_FT_DIS_MASK;
#if defined(PRAMC_1)
        PRAMC_1->PRCR1 |= PRAMC_PRCR1_FT_DIS_MASK;
#endif
    }
    else
    {
        PRAMC_0->PRCR1 &= ~PRAMC_PRCR1_FT_DIS_MASK;
#if defined(PRAMC_1)
        PRAMC_1->PRCR1 &= ~PRAMC_PRCR1_FT_DIS_MASK;
#endif
    }
}

void BOARD_ClockPreConfig(void)
{
#if defined(FSL_FEATURE_PMC_HAS_LAST_MILE_REGULATOR) && (FSL_FEATURE_PMC_HAS_LAST_MILE_REGULATOR)
    /* Enables PMC last mile regulator before enable PLL.  */
    if ((PMC->LVSC & PMC_LVSC_LVD15S_MASK) != 0U)
    {
        /* External bipolar junction transistor is connected between external voltage and V15 input pin. */
        PMC->CONFIG |= PMC_CONFIG_LMBCTLEN_MASK;
    }
    while((PMC->LVSC & PMC_LVSC_LVD15S_MASK) != 0U)
    {
    }
    PMC->CONFIG |= PMC_CONFIG_LMEN_MASK;
    while((PMC->CONFIG & PMC_CONFIG_LMSTAT_MASK) == 0u)
    {
    }
#endif /* FSL_FEATURE_PMC_HAS_LAST_MILE_REGULATOR */
    BOARD_EnableSRAMExtraLatency(true);
}

void BOARD_ClockPostConfig(void)
{
    /* Change MUX0 DIV trigger type to Immediate update. */
    CLOCK_SetClkMux0DivTriggerType(KCLOCK_ImmediateUpdate);
}

inline static void i2c_release_bus_delay(void)
{
    SDK_DelayAtLeastUs(10U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

void BOARD_InitI2c0PinAsGpio(void)
{
    SIUL2->MSCR[BOARD_ACCEL_I2C_SCL_PIN_MSCR_OFFSET] = (uint32_t)(kPORT_MUX_AS_GPIO);
    SIUL2->MSCR[BOARD_ACCEL_I2C_SDA_PIN_MSCR_OFFSET] = (uint32_t)(kPORT_MUX_AS_GPIO);
}

void BOARD_RestoreI2c0PinMux(void)
{
    SIUL2->MSCR[BOARD_ACCEL_I2C_SCL_PIN_MSCR_OFFSET] = (uint32_t)(SIUL2_MSCR_OBE(1) | SIUL2_MSCR_IBE(1) | SIUL2_MSCR_PKE(1) | SIUL2_MSCR_PUE(1) | SIUL2_MSCR_PUS(1) | kPORT_MUX_ALT4);
    SIUL2->MSCR[BOARD_ACCEL_I2C_SDA_PIN_MSCR_OFFSET] = (uint32_t)(SIUL2_MSCR_OBE(1) | SIUL2_MSCR_IBE(1) | SIUL2_MSCR_PKE(1) | SIUL2_MSCR_PUE(1) | SIUL2_MSCR_PUS(1) | kPORT_MUX_ALT4);
    SIUL2->IMCR[BOARD_ACCEL_I2C_SCL_PIN_IMCR_OFFSET] = (uint32_t)(kPORT_INPUT_MUX_ALT2);
    SIUL2->IMCR[BOARD_ACCEL_I2C_SDA_PIN_IMCR_OFFSET] = (uint32_t)(kPORT_INPUT_MUX_ALT2);
}

void BOARD_I2c0RecoverBus(void)
{
    SIUL2_SetPinOutputBuffer(SIUL2, BOARD_ACCEL_I2C_SCL_PIN_MSCR_OFFSET, true, kPORT_MUX_AS_GPIO);
    SIUL2_SetPinDirection(SIUL2, BOARD_ACCEL_I2C_SCL_PIN_MSCR_OFFSET, kPORT_OUT);
    SIUL2_PinWrite(SIUL2, BOARD_ACCEL_I2C_SCL_PIN_MSCR_OFFSET, GPIO_OUTPUT_HIGH_LOGIC);
    i2c_release_bus_delay();
   
    SIUL2_SetPinOutputBuffer(SIUL2, BOARD_ACCEL_I2C_SDA_PIN_MSCR_OFFSET, true, kPORT_MUX_AS_GPIO);
    SIUL2_SetPinDirection(SIUL2, BOARD_ACCEL_I2C_SDA_PIN_MSCR_OFFSET, kPORT_IN);
 
    /* Send pulses on SCL until SDA is released and then send stop. */
    while(true)
    {
        /* SCL pulse - low */
        SIUL2_PinWrite(SIUL2, BOARD_ACCEL_I2C_SCL_PIN_MSCR_OFFSET, GPIO_OUTPUT_LOW_LOGIC);
        i2c_release_bus_delay();
 
        /* Check whether SDA line is released */
        if (1U == SIUL2_PortPinRead(SIUL2, BOARD_ACCEL_I2C_SDA_GPIO, BOARD_ACCEL_I2C_SDA_PIN))
        {
            /* SDA is released, hold it in low */
            SIUL2_SetPinDirection(SIUL2, BOARD_ACCEL_I2C_SDA_PIN_MSCR_OFFSET, kPORT_OUT);
            SIUL2_PinWrite(SIUL2, BOARD_ACCEL_I2C_SDA_PIN_MSCR_OFFSET, GPIO_OUTPUT_LOW_LOGIC);
 
            /* SCL pulse - high */
            SIUL2_PinWrite(SIUL2, BOARD_ACCEL_I2C_SCL_PIN_MSCR_OFFSET, GPIO_OUTPUT_HIGH_LOGIC);
            i2c_release_bus_delay();
 
            /* Set SDA to high from low - send stop */
            SIUL2_PinWrite(SIUL2, BOARD_ACCEL_I2C_SDA_PIN_MSCR_OFFSET, GPIO_OUTPUT_LOW_LOGIC);
            i2c_release_bus_delay();
 
            break;
        }
        else
        {
            /* SCL pulse - high */
            SIUL2_PinWrite(SIUL2, BOARD_ACCEL_I2C_SCL_PIN_MSCR_OFFSET, GPIO_OUTPUT_HIGH_LOGIC);
            i2c_release_bus_delay();
        }
    }
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
                          uint8_t txBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
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
                             uint8_t rxBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = rxBuff;
    xfer.dataSize       = rxBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

void BOARD_Accel_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_ACCEL_I2C_BASEADDR, BOARD_ACCEL_I2C_CLOCK_FREQ);
}

status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff)
{
    uint8_t data = (uint8_t)txBuff;

    return BOARD_LPI2C_Send(BOARD_ACCEL_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, &data, 1);
}

status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_ACCEL_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, rxBuff, rxBuffSize);
}

void BOARD_MagSwitch_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_MAGSWITCH_I2C_BASEADDR, BOARD_MAGSWITCH_I2C_CLOCK_FREQ);
}

status_t BOARD_MagSwitch_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t data)
{
    return BOARD_LPI2C_Send(BOARD_MAGSWITCH_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, &data, 1U);
}

status_t BOARD_MagSwitch_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_MAGSWITCH_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, rxBuff, rxBuffSize);
}
#endif /* SDK_I2C_BASED_COMPONENT_USED */
