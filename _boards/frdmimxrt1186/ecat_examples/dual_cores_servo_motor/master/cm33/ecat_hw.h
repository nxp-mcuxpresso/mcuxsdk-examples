/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __ECAT_HW___H__
#define __ECAT_HW___H__

#include "ecat_def.h"
#include "fsl_ecat.h"

#ifdef ECAT
#define HW_ECAT_INSTANCE ECAT
#else
#define HW_ECAT_INSTANCE ETHERCAT
#endif

#include <string.h>

extern volatile uint32_t ecatAccessLock;
/*
 * The workaround for ECAT bus occupancy issue (EtherCAT slave side).
 *
 * Prior to each ESC register access, waitting if the bus is
 * currently occupied by the motor control side.
 *
 */
static inline void Ecat_WaitForUnlock(void)
{
    /* Wait until the motor control core releases the bus. */
    while (ecatAccessLock != 0)
    {
        __NOP();
    }
}

static inline uint32_t Ecat_ReadDWord(ECAT_Type *ecat, uint32_t Address)
{
    Ecat_WaitForUnlock();
    return ((uint32_t *)(ecat))[(Address) >> 2];
}

static inline void Ecat_WriteDWord(ECAT_Type *ecat, uint32_t DWordValue, uint32_t Address)
{
    Ecat_WaitForUnlock();
    ((uint32_t *)(ecat))[(Address) >> 2] = DWordValue;
}

static inline uint16_t Ecat_GetALEventRegister(ECAT_Type *ecat)
{
	Ecat_WaitForUnlock();
	return ecat->AL_EVENT_REQUEST;
}

/**< \brief Returns the first 16Bit of the AL Event register (0x220)*/
#define HW_GetALEventRegister()                    Ecat_GetALEventRegister(HW_ECAT_INSTANCE)
#define HW_GetALEventRegister_Isr()                Ecat_GetALEventRegister(HW_ECAT_INSTANCE)

/**< \brief Generic ESC (register and DPRAM) read access.*/
#define HW_EscRead(pData, Address, Len)            Ecat_Read(HW_ECAT_INSTANCE, (uint8_t *)(pData), Address, Len)
#define HW_EscReadIsr(pData, Address, Len)         Ecat_Read(HW_ECAT_INSTANCE, (uint8_t *)(pData), Address, Len)

/**< \brief 32Bit specific ESC (register and DPRAM) read access.*/
#define HW_EscReadDWord(DWordValue, Address)       ((DWordValue) = Ecat_ReadDWord(HW_ECAT_INSTANCE, Address))
#define HW_EscReadDWordIsr(DWordValue, Address)    ((DWordValue) = Ecat_ReadDWord(HW_ECAT_INSTANCE, Address))

/**< \brief Macro to copy data from the application mailbox memory(not the ESC memory, this access is handled by
 * HW_EscRead).*/
#define HW_EscReadMbxMem(pData, Address, Len)      Ecat_Read(HW_ECAT_INSTANCE, (uint8_t *)(pData), Address, Len)

/**< \brief Generic ESC (register and DPRAM) write access.*/
#define HW_EscWrite(pData, Address, Len)           Ecat_Write(HW_ECAT_INSTANCE, (uint8_t *)(pData), Address, Len)
#define HW_EscWriteIsr(pData, Address, Len)        Ecat_Write(HW_ECAT_INSTANCE, (uint8_t *)(pData), Address, Len)

/**< \brief 32Bit specific ESC (register and DPRAM) write access.*/
#define HW_EscWriteDWord(DWordValue, Address)      Ecat_WriteDWord(HW_ECAT_INSTANCE, DWordValue, Address)
#define HW_EscWriteDWordIsr(DWordValue, Address)   Ecat_WriteDWord(HW_ECAT_INSTANCE, DWordValue, Address)

/**< \brief Macro to copy data from the application mailbox memory (not the ESC memory, this access is handled by
 * HW_EscWrite).*/
#define HW_EscWriteMbxMem(pData, Address, Len)     Ecat_Write(HW_ECAT_INSTANCE, (uint8_t *)(pData), Address, Len)

#define ECAT_TIMER_INC_P_MS 0x01

UINT16 HW_Init(void);
void HW_Release(void);
UINT16 HW_GetTimer(void);
void HW_ClearTimer(void);
void ENABLE_ESC_INT(void);
void DISABLE_ESC_INT(void);
void Ecat_WaitForUnlock(void);
void Ecat_Read(ECAT_Type *ecat, uint8_t *pData, uint32_t addr, uint32_t len);
void Ecat_Write(ECAT_Type *ethercat, uint8_t *pData, uint32_t addr, uint32_t len);
void HW_SetLed(UINT8 RunLed, UINT8 ErrorLed);

#endif /* __ECAT_HW___H__ */
