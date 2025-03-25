/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_endat3.h"
#include "fsl_xbar.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
endat3_dev enc_dev;
/*******************************************************************************
 * Code
 ******************************************************************************/
uint32_t SYSTICK_GET_COUNT()
{
	uint32_t val  = SysTick->VAL;
	uint32_t load = SysTick->LOAD;
	return load - val;
}

void BOARD_InitSysTick(void)
{
	/* Initialize SysTick core timer to run free */
	/* Set period to maximum value 2^24*/
	SysTick->LOAD = 0xFFFFFF;

	/*Clock source - System Clock*/
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

	/*Start Sys Timer*/
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}


void ENDAT3_RspDump(endat3_rsp_t *rsp)
{
	uint16_t errCode;

	PRINTF("\tHPF.STATUS:   0x%02x(F: %d, W: %d, HPFV: %d,  RM: %d,  ERR_REQ: %d)\r\n", ENDAT3_READ_HPF_STATUS_F(rsp->hpf.hpf64),
			ENDAT3_READ_HPF_STATUS_W(rsp->hpf.hpf64), ENDAT3_READ_HPF_STATUS_HPFV(rsp->hpf.hpf64),
			ENDAT3_READ_HPF_STATUS_RM(rsp->hpf.hpf64), ENDAT3_READ_HPF_STATUS_ERR_REQ(rsp->hpf.hpf64));
	PRINTF("\tHPF.DATA:	 0x%x\" PRIx64 \"\r\n", ENDAT3_READ_HPF_DATA(rsp->hpf.hpf64));
	PRINTF("\tHPF.DATA:	 0x%x\r\n", ENDAT3_READ_HPF_DATA(rsp->hpf.hpf64));
	PRINTF("\tHPF.CS:	   0x%02x\r\n", ENDAT3_READ_HPF_CS(rsp->hpf.hpf64));

	if (ENDAT3_READ_HPF_STATUS_HPFV(rsp->hpf.hpf64) == 0) {
		errCode = ENDAT3_READ_HPF_ERRORCODE(rsp->hpf.hpf64);
		PRINTF("\tHPF invalid [0x%04x: %s]\r\n", errCode, ENDAT3_Err2str(errCode));
	}

	PRINTF("\tLPH.STATUS:   0x%02x (BG_STATUS: %d  BG_ERR_EXEC: %d  ZACT: %d)\r\n", ENDAT3_READ_LPH_STATUS(rsp->lph.lph32),
			ENDAT3_READ_LPH_STATUS_BG_STATUS(rsp->lph.lph32), ENDAT3_READ_LPH_STATUS_BG_ERR_EXEC(rsp->lph.lph32),
			ENDAT3_READ_LPH_STATUS_ZACT(rsp->lph.lph32));
	PRINTF("\tLPH.NLPF:	 0x%02x (YACT: %2d  XDIM: %2d)\r\n", ENDAT3_READ_LPH_NLPF(rsp->lph.lph32),
			ENDAT3_READ_LPH_NLPF_YACT(rsp->lph.lph32), ENDAT3_READ_LPH_NLPF_XDIM(rsp->lph.lph32));
	PRINTF("\tLPH.CS:	   0x%02x\r\n", ENDAT3_READ_LPH_CS(rsp->lph.lph32));

	uint8_t i;
	char *str;
	for (i = 0; i < rsp->xdim; i++) {
		uint8_t fid = ENDAT3_READ_LPF_STATUS_FID(rsp->lpf[i].lpf64);
		uint8_t lpfv = ENDAT3_READ_LPF_STATUS_LPFV(rsp->lpf[i].lpf64);
		str = ENDAT3_FID2str(fid);
		uint8_t status = ENDAT3_READ_LPF_STATUS(rsp->lpf[i].lpf64);
		PRINTF("\tLPF[%2d].STATUS:  0x%02x (FID: 0x%02x [%s]  LPFV: %d)\r\n", i,status , fid, str, lpfv);
		if (lpfv == 0) {
			errCode = ENDAT3_READ_LPF_ERRORCODE(rsp->lpf[i].lpf64);
			PRINTF("\t\tLPF invalid [0x%04x: %s]\r\n", errCode, ENDAT3_Err2str(errCode));
		}
		PRINTF("\tLPF[%2d].DATA:		0x%012llx\r\n", i, ENDAT3_READ_LPF_DATA(rsp->lpf[i].lpf64));
		PRINTF("\tLPF[%2d].CS:		  0x%02x\r\n", i, ENDAT3_READ_LPF_CS(rsp->lpf[i].lpf64));
	 }
}

void ENDAT3_PosDump(void)
{
	endat3_rsp_t rsp;
	uint64_t position;

	ENDAT3_getRsp(enc_dev.base, enc_dev.bus_addr, &rsp);
	position = ENDAT3_READ_HPF_DATA(rsp.hpf.hpf64);

	if ((rsp.hpf.hpf.status | 0x1F) != 0x0C) {
		if (ENDAT3_READ_HPF_STATUS_F(rsp.hpf.hpf64)) {
			PRINTF("The position data might be wrong and should therefore no longer be used. The cause for f being set can be read through the LPF ERRMSG\r\n");
			return;
		}

		if (ENDAT3_READ_HPF_STATUS_W(rsp.hpf.hpf64)) {
			PRINTF("The position data is correct, but errors might occur soon. The cause for W being set can be read through the LPF ERRMSG.\r\n");
			return;
		}

		if (ENDAT3_READ_HPF_STATUS_HPFV(rsp.hpf.hpf64) == 0x0) {
			PRINTF("The field HPF.DATA does not contain the configured contents. One of the error codes is being transmitted\r\n");
			return;
		}

		if (ENDAT3_READ_HPF_STATUS_RM(rsp.hpf.hpf64) == 0x0) {
			PRINTF("Absolute value cannot be made available\r\n");
			return;
		}

		if (ENDAT3_READ_HPF_STATUS_ERR_REQ(rsp.hpf.hpf64)) {
			PRINTF("The request code does not support or that is not defined\r\n");
			return;
		}
	}

#ifdef ENCODER_TYPE_ROTARY
	uint16_t multiturn  = position >> 32;
	uint32_t singleturn = position & 0xFFFFFFFF;
	float deg = 360.0 * singleturn / (float)0x100000000ull;
	PRINTF("MT: %5u ST: %10u (%6.2f deg)\r\n", multiturn, singleturn, deg);
#elif ENCODER_TYPE_LINEAR
	int64_t nm = position >> 8;
	/*Sign extension*/
	if (nm & (1ull << 39)) {
		nm |= 0xFFFFFF0000000000;
	}
	PRINTF("Pos[nm]: %15" PRId64 "\r\n", nm);
#endif
}

void ENDAT3_FIDMEM_Dump(struct FID *fid_res)
{
	PRINTF("FID_MEM: fid64 = 0x%x\r\n", fid_res->fid64);
	PRINTF("FID_MEM: timestamp = %d\r\n", ENDAT3_FIDMEM_GetTimestamp(fid_res));
	PRINTF("FID_MEM: lpfv = %d\r\n", ENDAT3_FIDMEM_GetLPFV(fid_res));
	PRINTF("FID_MEM: LPF_DATA = 0x%x\r\n", ENDAT3_FIDMEM_GetData(fid_res));
}

void ENDAT3_SAFETY_MEM_Dump(ENDAT3_Type *base, uint8_t bus_addr, uint8_t packet, uint8_t inHPF)
{
	struct LPF *lpf;
	uint8_t fid, lpfv, status;
	uint16_t errCode;
	char *str;

	PRINTF("FID_SD1:\r\n");
	if (inHPF) {
		struct HPF *hpf = ENDAT3_GET_SAFETY_FID_SD1_HPF(base, bus_addr, packet);
		PRINTF("\tHPF.STATUS:   0x%02x(F: %d, W: %d, HPFV: %d,  RM: %d,  ERR_REQ: %d)\r\n", ENDAT3_READ_HPF_STATUS_F(hpf->hpf64),
			ENDAT3_READ_HPF_STATUS_W(hpf->hpf64), ENDAT3_READ_HPF_STATUS_HPFV(hpf->hpf64),
			ENDAT3_READ_HPF_STATUS_RM(hpf->hpf64), ENDAT3_READ_HPF_STATUS_ERR_REQ(hpf->hpf64));
		PRINTF("\tHPF.DATA:	 0x%012\" PRIx64 \"\r\n", ENDAT3_READ_HPF_DATA(hpf->hpf64));
		PRINTF("\tHPF.DATA:	 0x%012llx\r\n", ENDAT3_READ_HPF_DATA(hpf->hpf64));
		PRINTF("\tHPF.CS:	   0x%02x\r\n", ENDAT3_READ_HPF_CS(hpf->hpf64));
	} else {
		lpf = ENDAT3_GET_SAFETY_FID_SD1_LPF(base, bus_addr, packet);
		fid = ENDAT3_READ_LPF_STATUS_FID(lpf->lpf64);
		lpfv = ENDAT3_READ_LPF_STATUS_LPFV(lpf->lpf64);
		str = ENDAT3_FID2str(fid);
		status = ENDAT3_READ_LPF_STATUS(lpf->lpf64);
		PRINTF("\tLPF.STATUS:  0x%02x (FID: 0x%02x [%s]  LPFV: %d)\r\n", status , fid, str, lpfv);
		if (lpfv == 0) {
			errCode = ENDAT3_READ_LPF_ERRORCODE(lpf->lpf64);
			PRINTF("\t\tLPF invalid [0x%04x: %s]\r\n", errCode, ENDAT3_Err2str(errCode));
		}
		PRINTF("\tLPF: singleturn = 0x%x.  multiturn = 0x%x\r\n", ENDAT3_READ_POS_SINGLETURN_LPF(lpf->lpf64), ENDAT3_READ_POS_MULTITURN_LPF(lpf->lpf64));
		PRINTF("\tLPF[%2d].CS:		  0x%02x\r\n", ENDAT3_READ_LPF_CS(lpf->lpf64));
	}
	PRINTF("FID_SD2:\r\n");
	lpf = ENDAT3_GET_SAFETY_FID_SD2_LPF(base, bus_addr, packet);
	fid = ENDAT3_READ_LPF_STATUS_FID(lpf->lpf64);
	lpfv = ENDAT3_READ_LPF_STATUS_LPFV(lpf->lpf64);
	str = ENDAT3_FID2str(fid);
	status = ENDAT3_READ_LPF_STATUS(lpf->lpf64);
	PRINTF("\tLPF.STATUS:  0x%02x (FID: 0x%02x [%s]  LPFV: %d)\r\n", status , fid, str, lpfv);
	if (lpfv == 0) {
		errCode = ENDAT3_READ_LPF_ERRORCODE(lpf->lpf64);
		PRINTF("\t\tLPF invalid [0x%04x: %s]\r\n", errCode, ENDAT3_Err2str(errCode));
	}
	PRINTF("\tLPF: singleturn = 0x%x.  multiturn = 0x%x\r\n", ENDAT3_READ_POS_SINGLETURN_LPF(lpf->lpf64), ENDAT3_READ_POS_MULTITURN_LPF(lpf->lpf64));
	PRINTF("\tLPF[%2d].CS:		  0x%02x\r\n", ENDAT3_READ_LPF_CS(lpf->lpf64));

	PRINTF("FID_SF:\r\n");
	lpf = ENDAT3_GET_SAFETY_FID_SF_LPF(base, bus_addr, packet);
	fid = ENDAT3_READ_LPF_STATUS_FID(lpf->lpf64);
	lpfv = ENDAT3_READ_LPF_STATUS_LPFV(lpf->lpf64);
	str = ENDAT3_FID2str(fid);
	status = ENDAT3_READ_LPF_STATUS(lpf->lpf64);
	PRINTF("\tLPF.STATUS:  0x%02x (FID: 0x%02x [%s]  LPFV: %d)\r\n", status , fid, str, lpfv);
	if (lpfv == 0) {
		errCode = ENDAT3_READ_LPF_ERRORCODE(lpf->lpf64);
		PRINTF("\t\tLPF invalid [0x%04x: %s]\r\n", errCode, ENDAT3_Err2str(errCode));
	}

	PRINTF("\tLPF: AA:0x%x F1:0x%x F2:0x%x IgF1:0x%x IgF2:0x%x SOL:0x%x CSS:0x%x\r\n",
				ENDAT3_READ_LPF_DATA_SF_POS1_AA(lpf->lpf64),
				ENDAT3_READ_LPF_DATA_SF_POS1_STATUS_F1(lpf->lpf64),
				ENDAT3_READ_LPF_DATA_SF_POS1_STATUS_F2(lpf->lpf64),
				ENDAT3_READ_LPF_DATA_SF_POS1_STATUS_IgF1(lpf->lpf64),
				ENDAT3_READ_LPF_DATA_SF_POS1_STATUS_IgF2(lpf->lpf64),
				ENDAT3_READ_LPF_DATA_SF_POS1_SOL(lpf->lpf64),
				ENDAT3_READ_LPF_DATA_SF_POS1_CSS(lpf->lpf64));
	PRINTF("\tLPF[%2d].CS:		  0x%02x\r\n", ENDAT3_READ_LPF_CS(lpf->lpf64));
}


/* Triggered by the second PWM trigger signal via XBAR */
void DEMO_XBARA_IRQHandler(void)
{
	ENDAT3_PosDump();
	XBAR_ClearOutputStatusFlag(kXBAR1_OutputEdma4IpdReq76);
	SDK_ISR_EXIT_BARRIER;
}

/* FG_IRQ0_IRQHandler */
void FG_IRQ0_IRQHandler(void)
{
	uint32_t time = SYSTICK_GET_COUNT();
	PRINTF("The minimal communication cycle: %d\r\n", time);
	ENDAT3_PosDump();
	ENDAT3_IRQ_Clear(enc_dev.base, CLEAR_FG_IRQ0);
}

/* FG_IRQ1_IRQHandler */
void FG_IRQ1_IRQHandler(void)
{
}

/* FG_IRQ2_IRQHandler */
void FG_IRQ2_IRQHandler(void)
{
}

void DEMO_BG_IRQ_IRQHandler (void)
{
	if (enc_dev.base->FG_STATUS | ENDAT3_FG_STATUS_BG_IRQ_STATE_MASK) {
		PRINTF("BG_RSP_1: 0x%x\r\n", enc_dev.base->BG_RSP_1);
	}
	ENDAT3_IRQ_Clear(enc_dev.base, CLEAR_BG_IRQ);
	SDK_ISR_EXIT_BARRIER;
}

void DEMO_FG_IRQ_IRQHandler(void)
{
	if (enc_dev.base->FG_STATUS | ENDAT3_FG_STATUS_FG_IRQ0_STATE_MASK) {
		FG_IRQ0_IRQHandler();
	}

	if (enc_dev.base->FG_STATUS | ENDAT3_FG_STATUS_FG_IRQ1_STATE_MASK) {
		FG_IRQ1_IRQHandler();
	}

	if (enc_dev.base->FG_STATUS | ENDAT3_FG_STATUS_FG_IRQ2_STATE_MASK) {
		FG_IRQ2_IRQHandler();
	}

	SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
	BOARD_InitHardware();
	PRINTF("Encoder Endat3 example\r\n");

	enc_dev.base = BOARD_ENDAT3_BASEADDR;
	enc_dev.bus_addr = 0;
	uint8_t fg_strobes = 1;
	endat3_rsp_t rsp;
	endat3_mem_cache_t mem_cache;
	endat3_mem_cache_t *cache = &mem_cache;
	uint16_t memory_cache_buf[0x200];
	cache->cacheMemSize = 0x200;
	uint16_t phyDatarate;

	// Master Version
	//PRINTF("Version infomation of the EnDat 3 Master:\r\n");
	//PRINTF("\tPHY Version: %d\r\n" (enc_dev.base->VERSION_INFO >> ENDAT3_PHY_VERSION_SHIFT) & ENDAT3_PHY_VERSION_MASK);
	//PRINTF("\tCore Version: %d\r\n", (enc_dev.base->VERSION_INFO >> ENDAT3_CORE_VERSION_SHIFT) & ENDAT3_CORE_VERSION_MASK);
	//PRINTF("\tMemory Interface Version: %d\r\n", (enc_dev.base->VERSION_INFO >> ENDAT3_MEMIF_VERSION_SHIFT) & ENDAT3_MEMIF_VERSION_MASK);
	//PRINTF("\tBus Adapter Version: %d\r\n", (enc_dev.base->VERSION_INFO >> ENDAT3_BUS_IF_VERSION_SHIFT) & ENDAT3_BUS_IF_VERSION_MASK);

	// Master Features
	PRINTF("Instantiation information of connected EnDat 3 Master:\r\n");
	PRINTF("\tNumber of supported bus nodes: %d\r\n", (enc_dev.base->INSTANTIATION_INFO) & ENDAT3_INSTANTIATION_INFO_NUM_BUS_NODES_SUPP_MASK);
	PRINTF("\tFID based memory enabled: %d\r\n", (enc_dev.base->INSTANTIATION_INFO  & ENDAT3_INSTANTIATION_INFO_FID_BASED_MEMORY_ENABLE_MASK)>> ENDAT3_INSTANTIATION_INFO_FID_BASED_MEMORY_ENABLE_SHIFT);
	PRINTF("\tOnly 12.5 MBit/s supported: %d\r\n", (enc_dev.base->INSTANTIATION_INFO  & ENDAT3_INSTANTIATION_INFO_USE_12_5_MBIT_ONLY_MASK) >> ENDAT3_INSTANTIATION_INFO_USE_12_5_MBIT_ONLY_SHIFT);
	PRINTF("\tIRQ auto clear enabled: %d\r\n", (enc_dev.base->INSTANTIATION_INFO & ENDAT3_INSTANTIATION_INFO_IRQ_AUTO_CLEAR_ENABLE_MASK) >> ENDAT3_INSTANTIATION_INFO_IRQ_AUTO_CLEAR_ENABLE_SHIFT) ;
	PRINTF("\tDebug functionality enabled: %d\r\n", (enc_dev.base->INSTANTIATION_INFO & ENDAT3_INSTANTIATION_INFO_DEBUG_ENABLE_MASK) >> ENDAT3_INSTANTIATION_INFO_DEBUG_ENABLE_SHIFT);
	PRINTF("\tNumber of supported safety packets: %d\r\n", (enc_dev.base->INSTANTIATION_INFO & ENDAT3_INSTANTIATION_INFO_NUM_SAFETY_PACKETS_MASK) >> ENDAT3_INSTANTIATION_INFO_NUM_SAFETY_PACKETS_SHIFT) ;

	PRINTF("Initialize master for 12.5 MBit operation.\r\n");
	/* Initialize master for 12.5 MBit operation */
	ENDAT3_RxTxClkConfig(enc_dev.base, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_12_5MBPS, 1000);


	PRINTF("Test FG: Reset Request\r\n");
	ENDAT3_FG_Reset(enc_dev.base);
	PRINTF("Test FG:  Reset Request successful\r\n");

	/* Sending HELLO request to initialize the encoder*/
	if (ENDAT3_FG_Hello(enc_dev.base) != kStatus_Success) {
		PRINTF("\tACTIVATION HELLO failed, encoder did not respond.\r\n");
	}

	/* Try again*/
	if (ENDAT3_FG_Hello(enc_dev.base) != kStatus_Success) {
		PRINTF("\tACTIVATION HELLO failed, encoder did not respond again.\r\n");
		return 1;
	}
	PRINTF("\tACTIVATION HELLO is active.\r\n");


	PRINTF("Test FG: Data0 Request under 12.5Mb/s.\r\n");
	/* Test on 12.5Mbit operation */
	ENDAT3_FG_Data(enc_dev.base, ENDAT3_FG_REQ_DATA0, &rsp);
	ENDAT3_RspDump(&rsp);

	 ENDAT3_BG_Handler_Enable(enc_dev.base);
	PRINTF("Try to switch 25Mb/s\r\n");
	/* Switch to 25mBit/s if encoder supported. */
	if (ENDAT3_memRead(enc_dev.base, enc_dev.bus_addr, ENDAT3_MEM_BASE_EL + ENDAT3_MEM_EL_PHYDATARATE_OFFSET,  1, &phyDatarate, 1) != kStatus_Success) {
		PRINTF("\tMemory Read Error\r\n");
		return 1;
	}

	if (phyDatarate & 2) {
		PRINTF("\tRate 25 MBit/s is supported\r\n");
		if (ENDAT3_FG_Req_Rsp(enc_dev.base, ENDAT3_FG_REQ_RATE, 1, &rsp) != kStatus_Success) {
			PRINTF("\tRate switching is failed.\r\n");
			return 1;
		}

		ENDAT3_RxTxClkConfig(enc_dev.base, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_25MBPS, 10);

		SDK_DelayAtLeastUs(30000, SystemCoreClock);
		if (ENDAT3_FG_Hello(enc_dev.base) != kStatus_Success) {
			PRINTF("\tFail to Communicate with 25Mb/s\r\n");
			return 1;
		}
		PRINTF("\tSucceed to switch to 25Mb/s.\r\n");
	} else {
		 PRINTF("\tThe encoder isn't supported for 25Mb/s.\r\n");
	}

	PRINTF("Test BG: Auth\r\n");
	// switch to OEM2 level
	if (ENDAT3_BG_Auth(enc_dev.base, 0, ENDAT3_BG_ACCLEVEL_OEM2, 0, fg_strobes) == kStatus_Success) {
		PRINTF("\tSucceed to switch to OEM2 level\r\n");
	}

	// switch to user level again
	if (ENDAT3_BG_Auth(enc_dev.base, 0, ENDAT3_BG_ACCLEVEL_USER, 0, fg_strobes) == kStatus_Success) {
		PRINTF("\tSucceed to switch to  user level again\r\n");

	}
	PRINTF("Test BG: Setpass\r\n");
	// set the password for user level
	if (ENDAT3_BG_Setpass(enc_dev.base, 0, ENDAT3_BG_ACCLEVEL_USER, 0x123, fg_strobes) == kStatus_Success) {
		PRINTF("\tSucceed to set the password for user level\r\n");
	}

	// recover the password for user level
	if (ENDAT3_BG_Setpass(enc_dev.base, 0, ENDAT3_BG_ACCLEVEL_USER, 0, fg_strobes) == kStatus_Success) {
		PRINTF("\tSucceed to cancel the password setting for user level\r\n");
	}

	PRINTF("Test BG: Nop");
	uint64_t  nop_rsp = 0;
	ENDAT3_BG_Nop(enc_dev.base, 0, 0x123567, &nop_rsp, fg_strobes);
	if (nop_rsp == ((0x01ULL << 40) | 0x123567)) {
		PRINTF("\tSucceed to NOP test.\r\n");
	} else {
		PRINTF("\tFail to NOP test. Recv: 0x%x\r\n", nop_rsp);
	}

	PRINTF("Test BG: Local\t\n");
	if (ENDAT3_BG_Locate(enc_dev.base, 0, 1, fg_strobes) == kStatus_Success) {
		PRINTF("\tSucceed to Local test.\r\n");
	} else {
		PRINTF("\tFail to Local test.\r\n");
	}

	PRINTF("Test BG: Protect\t\n");
	uint8_t al_write, al_read;
	if (ENDAT3_BG_Protect(enc_dev.base, 0, ENDAT3_MEM_BASE_LPFLIVE, ENDAT3_BG_PROTECT_MODE_SET_READ, ENDAT3_BG_ACCLEVEL_USER, &al_write, &al_read, fg_strobes) == kStatus_Success) {
		PRINTF("\tSucceed to Protect test. al_write=%d al_read=%d\r\n", al_write, al_read);
	} else {
		 PRINTF("\tFail to Protect test.\r\n");
	}


	PRINTF("Dump encoder identification data from electronic label\r\n");
	/*Dump encoder identification data from electronic label*/
	if (ENDAT3_memCacheInit(enc_dev.base, enc_dev.bus_addr, ENDAT3_MEM_BASE_EL, cache, memory_cache_buf, 200, 1) != kStatus_Success) {
		PRINTF("\tMemory cache initialization failed\r\n");
		return 1;
	}

	if (ENDAT3_memCacheFetch(enc_dev.base, enc_dev.bus_addr, cache, 1) != kStatus_Success) {
		PRINTF("\tMemory cache updating failed\r\n");
		return 1;
	}

	if (ENDAT3_memCacheCheckCS(cache) != kStatus_Success) {
		PRINTF("\tEL cache CS check error\t\n");
		return 1;
	}

	uint8_t devName[11] = {0};
	devName[0] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_0(cache);
	devName[1] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_1(cache);
	devName[2] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_2(cache);
	devName[3] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_3(cache);
	devName[4] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_4(cache);
	devName[5] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_5(cache);
	devName[6] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_6(cache);
	devName[7] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_7(cache);
	devName[8] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_8(cache);
	devName[9] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_9(cache);
	PRINTF("\tEL.deviceName   : \"%s\"\r\n", devName);

	uint64_t ident;
	ident = ENDAT3_MEM_CACHE_READ_EL_DEVICEIDENT(cache);
	PRINTF("\tEL.deviceIdent  : %6u-%c%c\r\n", (uint32_t)(ident>>16), (uint8_t)(ident >> 8) & 0xFF, (uint8_t)(ident & 0xFF));
//	uint64_t serial;
//	serial = ENDAT3_MEM_CACHE_READ_EL_DEVICESERIAL(cache);
//	PRINTF("\tEL.deviceSerial : %c%u%c\r\n", (uint8_t)(serial >> 40), (uint32_t)(serial>>8) & 0xFFFFFFFF, (uint8_t)(serial & 0xFF));

	PRINTF("Dump encoder identification data from enhanced electronic label\r\n");
	/*Dump encoder identification data from enhanced electronic label*/
	if (ENDAT3_memCacheInit(enc_dev.base, enc_dev.bus_addr, ENDAT3_MEM_BASE_XEL, cache, memory_cache_buf, 200, 1) != kStatus_Success) {
		PRINTF("\tMemory cache initialization failed\r\n");
		return 1;
	}

	if (ENDAT3_memCacheFetch(enc_dev.base, enc_dev.bus_addr, cache, 1) != kStatus_Success) {
		PRINTF("\tMemory cache updating failed\r\n");
		return 1;
	}

	if (ENDAT3_memCacheCheckCS(cache) != kStatus_Success) {
		PRINTF("\tXEL cache CS check error\r\n");
		return 1;
	}

	uint16_t encoder_type;
	encoder_type = ENDAT3_MEM_CACHE_READ_XEL_ENCODERTYPE(cache);
	switch (encoder_type) {
		case 0x0000: PRINTF("\tXEL.encoderType: Not available\r\n"); break;
		case 0x0001: PRINTF("\tXEL.encoderType: Rotary absolute\r\n"); break;
		case 0x0002: PRINTF("\tXEL.encoderType: Rotary incremental\r\n"); break;
		case 0x0003: PRINTF("\tXEL.encoderType: Linear absolute\r\n"); break;
		case 0x0004: PRINTF("\tXEL.encoderType: Linear incremental\r\n"); break;
		default:	 PRINTF("\tXEL.encoderType: Reserved\r\n"); break;
	}



	uint32_t LPFMemBase;
#ifdef LPF_CONFIGURE_XSET
	LPFMemBase = ENDAT3_MEM_BASE_XSET;
#else
	LPFMemBase = ENDAT3_MEM_BASE_LPFLIVE;
#endif

	if (ENDAT3_memCacheInit(enc_dev.base, enc_dev.bus_addr, LPFMemBase, cache, memory_cache_buf, 200, 1) != kStatus_Success) {
		PRINTF("\tMemory cache initialization failed\r\n");
		return 1;
	}

	if (ENDAT3_memCacheFetch(enc_dev.base, enc_dev.bus_addr, cache, 1) != kStatus_Success) {
		PRINTF("\tMemory cache updating failed\r\n");
		return 1;
	}

	if (ENDAT3_memCacheCheckCS(cache) != kStatus_Success) {
		PRINTF("\tLPFLIVE cache CS check error\r\n");
		return 1;
	}

	PRINTF("Dump current LPF configuration\r\n");
	uint8_t x_dim, y_dim, x, y, z;
	uint16_t pointer;
	uint8_t fid;
	/*Dump current LPF configuration*/
	for (z = 0; z < 7; z++) {
		ENDAT3_lpfCacheGetXdimYdim(z + 1, cache, &x_dim, &y_dim);
		pointer = ENDAT3_lpfCacheGetPointer(z + 1, cache);
		if (x_dim == 0) {
			PRINTF("\tList%d: no LPFS will be output\r\n");
		} else {
			PRINTF("\tList%d: pointer = 0x%x xdim = %d y_dim = %d\r\n", z + 1, pointer, x_dim, y_dim);
			for (y = 0; y < y_dim; y++) {
				for (x = 0; x < x_dim; x++) {
					fid = ENDAT3_lpfCacheListGetFid(cache, x_dim, x, y, pointer);
					PRINTF("\t\tFID(x = %d; y = %d) : %s\r\n", x, y, ENDAT3_FID2str(fid));
				}
			}
		}
	}

	PRINTF("LPF Reconfiguration\r\n");

	/*Reconfigurate new LPF Memory*/
	/* LPF Configuration
	 *
	 * DATA1:   x = 0		 x = 1		x = 2		 x = 3		  x = 4
	 *   y = 0: BGRSP   EVALNUM   SENSOR_TEMP_MAX  ERRMSG	 SENSOR_TEMP_INT
	 *
	 * DATA2:   x = 0	x = 1	x = 2		x = 3		  x = 4
	 *   y = 0: BGRSP   POS2	 POS1	 ERRMSG		   SF_POS1
	 *   y = 1: BGRSP   EVALNUM  ERRMSG   SENSOR_TEMP_MAX  SENSOR_TEMP_INT
	 *   y = 2: BGRSP   EVALNUM  ERRMSG   SENSOR_TEMP_MAX  SENSOR_TEMP_INT
	 *   y = 3: BGRSP   EVALNUM  ERRMSG   SENSOR_TEMP_MAX  SENSOR_TEMP_INT
	 *   y = 4: BGRSP   EVALNUM  ERRMSG   SENSOR_TEMP_MAX  SENSOR_TEMP_INT
	 *   y = 5: BGRSP   EVALNUM  ERRMSG   SENSOR_TEMP_MAX  SENSOR_TEMP_INT
	 *   y = 6: BGRSP   EVALNUM  ERRMSG   SENSOR_TEMP_MAX  SENSOR_TEMP_INT
	 *   y = 7: BGRSP   EVALNUM  ERRMSG   SENSOR_TEMP_MAX  SENSOR_TEMP_INT
	 *   y = 8: BGRSP   EVALNUM  ERRMSG   SENSOR_TEMP_MAX  SENSOR_TEMP_INT
	 *   y = 9: POS2	SF_POS1  ERRMSG   SENSOR_TEMP_MAX  SENSOR_TEMP_INT
	 *
	 */
	char data1[1][5] = {{ENDAT3_FID_BGRSP,  ENDAT3_FID_POS2,  ENDAT3_FID_POS1, ENDAT3_FID_ERRMSG,  ENDAT3_FID_SF_POS1}};
	char data2[10][5] = {{ENDAT3_FID_BGRSP, ENDAT3_FID_EVALNUM, ENDAT3_FID_SENSOR_TEMP_MAX, ENDAT3_FID_ERRMSG, ENDAT3_FID_SENSOR_TEMP_INT},
					{ENDAT3_FID_BGRSP, ENDAT3_FID_EVALNUM, ENDAT3_FID_SENSOR_TEMP_MAX, ENDAT3_FID_ERRMSG, ENDAT3_FID_SENSOR_TEMP_INT},
					{ENDAT3_FID_BGRSP, ENDAT3_FID_EVALNUM, ENDAT3_FID_SENSOR_TEMP_MAX, ENDAT3_FID_ERRMSG, ENDAT3_FID_SENSOR_TEMP_INT},
					{ENDAT3_FID_BGRSP, ENDAT3_FID_EVALNUM, ENDAT3_FID_SENSOR_TEMP_MAX, ENDAT3_FID_ERRMSG, ENDAT3_FID_SENSOR_TEMP_INT},
					{ENDAT3_FID_BGRSP, ENDAT3_FID_EVALNUM, ENDAT3_FID_SENSOR_TEMP_MAX, ENDAT3_FID_ERRMSG, ENDAT3_FID_SENSOR_TEMP_INT},
					{ENDAT3_FID_BGRSP, ENDAT3_FID_EVALNUM, ENDAT3_FID_SENSOR_TEMP_MAX, ENDAT3_FID_ERRMSG, ENDAT3_FID_SENSOR_TEMP_INT},
					{ENDAT3_FID_BGRSP, ENDAT3_FID_EVALNUM, ENDAT3_FID_SENSOR_TEMP_MAX, ENDAT3_FID_ERRMSG, ENDAT3_FID_SENSOR_TEMP_INT},
					{ENDAT3_FID_BGRSP, ENDAT3_FID_EVALNUM, ENDAT3_FID_SENSOR_TEMP_MAX, ENDAT3_FID_ERRMSG, ENDAT3_FID_SENSOR_TEMP_INT},
					{ENDAT3_FID_BGRSP, ENDAT3_FID_EVALNUM, ENDAT3_FID_SENSOR_TEMP_MAX, ENDAT3_FID_ERRMSG, ENDAT3_FID_SENSOR_TEMP_INT},
					{ENDAT3_FID_BGRSP, ENDAT3_FID_EVALNUM, ENDAT3_FID_SENSOR_TEMP_MAX, ENDAT3_FID_ERRMSG, ENDAT3_FID_SENSOR_TEMP_INT}};

	memset(cache->cacheMem, 0, cache->cacheMemSize);

	/* Configurate Data1 LPF */
	pointer = 0;
	ENDAT3_lpfCacheListSetSendlist(cache, 1, 5,  1,  pointer, (uint8_t *)data1);

	/* Configurate Data2 LPF */
	pointer += 5 * 1;
	ENDAT3_lpfCacheListSetSendlist(cache, 2, 5,  10,  pointer, (uint8_t *)data2);

	/* Configurate other DataZ for no LPF output */
	pointer = 0;
	for ( z = 3; z < 8; z++) {
		ENDAT3_lpfCacheListSetSendlist(cache, z, 0,  0, pointer, NULL);
	}

	PRINTF("Flush cache to Encoder\r\n");
	/* Flush cache to Encoder */
	if (ENDAT3_lpfCacheFlushToEncoder(enc_dev.base, enc_dev.bus_addr, cache, 1)) {
		PRINTF("\tFlush cache to FID Memory failed\r\n");
		return 1;
	}

	PRINTF("Active the new LPF configuration\r\n");
	/* Active the new LPF configuration */
	if (ENDAT3_BG_Reconfigure(enc_dev.base, enc_dev.bus_addr, 1) != kStatus_Success) {
		PRINTF("\tReconfigure failed\r\n");
		return 1;
	}

	PRINTF("Dump ENDAT3_MEM_BASE_LPFLIVE again.\r\n");
	/* Dump ENDAT3_MEM_BASE_LPFLIVE again. */
	  if (ENDAT3_memCacheInit(enc_dev.base, enc_dev.bus_addr, ENDAT3_MEM_BASE_LPFLIVE, cache, memory_cache_buf, 200, 1) != kStatus_Success) {
		PRINTF("\tMemory cache initialization failed\r\n");
		return 1;
	}

	if (ENDAT3_memCacheFetch(enc_dev.base, enc_dev.bus_addr, cache, 1) != kStatus_Success) {
		PRINTF("\tMemory cache updating failed\r\n");
		return 1;
	}

	if (ENDAT3_memCacheCheckCS(cache) != kStatus_Success) {
		PRINTF("\tLPFLIVE cache CS check error\r\n");
		return 1;
	}

	/*Dump current LPF configuration*/
	for (z = 0; z < 7; z++) {
		ENDAT3_lpfCacheGetXdimYdim(z + 1, cache, &x_dim, &y_dim);
		pointer = ENDAT3_lpfCacheGetPointer(z + 1, cache);
		if (x_dim == 0) {
			PRINTF("\tList%d: no LPFS will be output\r\n");
		} else {
			PRINTF("\tList%d: pointer = 0x%x xdim = %d y_dim = %d\r\n", z + 1, pointer, x_dim, y_dim);
			for (y = 0; y < y_dim; y++) {
				for (x = 0; x < x_dim; x++) {
					fid = ENDAT3_lpfCacheListGetFid(cache, x_dim, x, y, pointer);
					PRINTF("\t\tFID(x = %d; y = %d) : %s\r\n", x, y, ENDAT3_FID2str(fid));
				}
			}
		}
	}

	PRINTF("Test Data1 request\r\n");
	/*Test Data1 request*/
	ENDAT3_FG_Req_Rsp(enc_dev.base, ENDAT3_FG_REQ_DATA1, 0, &rsp);
	ENDAT3_RspDump(&rsp);

	PRINTF("Test Data2 request and Dump FID frame from FID MEM\r\n");
	/*Test Data2 request*/
	struct FID fid_mem;
	for (int i = 0; i < 10; i++) {
		ENDAT3_FG_Req_Rsp(enc_dev.base, ENDAT3_FG_REQ_DATA2, 0, &rsp);
		ENDAT3_RspDump(&rsp);

		ENDAT3_FIDMEM_getLpf(enc_dev.base, 0, ENDAT3_FID_BGRSP, &fid_mem);
		ENDAT3_FIDMEM_Dump(&fid_mem);

		ENDAT3_FIDMEM_getLpf(enc_dev.base, 0, ENDAT3_FID_EVALNUM, &fid_mem);
		ENDAT3_FIDMEM_Dump(&fid_mem);

		ENDAT3_FIDMEM_getLpf(enc_dev.base, 0, ENDAT3_FID_SENSOR_TEMP_MAX, &fid_mem);
		ENDAT3_FIDMEM_Dump(&fid_mem);

		ENDAT3_FIDMEM_getLpf(enc_dev.base, 0, ENDAT3_FID_ERRMSG, &fid_mem);
		ENDAT3_FIDMEM_Dump(&fid_mem);

		ENDAT3_FIDMEM_getLpf(enc_dev.base, 0, ENDAT3_FID_SENSOR_TEMP_INT, &fid_mem);
		ENDAT3_FIDMEM_Dump(&fid_mem);
	}


	PRINTF("Endat3 Safety Collector 0 verification\r\n");

	ENDAT3_Safety_Packet0_SF_Cfg(enc_dev.base, ENDAT3_FID_SF_POS1);
	ENDAT3_Safety_Packet0_SD2_Cfg(enc_dev.base, ENDAT3_FID_POS2);
	ENDAT3_Safety_Packet0_SD1_Cfg(enc_dev.base, ENDAT3_FID_POS1, 1);

	ENDAT3_FG_Req_Rsp(enc_dev.base, ENDAT3_FG_REQ_DATA1, 0, &rsp);
	ENDAT3_RspDump(&rsp);
	PRINTF("SC_STATUS_0: %d\r\n",ENDAT3_Safety_Packet_status(enc_dev.base, 0, 0));
	ENDAT3_SAFETY_MEM_Dump(enc_dev.base, 0, 0, 1);


	PRINTF("Endat3 Safety Collector 1 verification\r\n");
	ENDAT3_Safety_Packet1_SF_Cfg(enc_dev.base, ENDAT3_FID_SF_POS1);
	ENDAT3_Safety_Packet1_SD2_Cfg(enc_dev.base, ENDAT3_FID_POS2);
	ENDAT3_Safety_Packet1_SD1_Cfg(enc_dev.base, ENDAT3_FID_POS1, 1);

	ENDAT3_FG_Req_Rsp(enc_dev.base, ENDAT3_FG_REQ_DATA1, 0, &rsp);
	ENDAT3_RspDump(&rsp);
	PRINTF("SC_STATUS_1: %d\r\n",ENDAT3_Safety_Packet_status(enc_dev.base, 1, 0));
	ENDAT3_SAFETY_MEM_Dump(enc_dev.base, 0, 0, 1);

	PRINTF("BG Interrupt test:\r\n");
	ENDAT3_BG_IRQ_Enable(enc_dev.base, ENDAT3_BG_IRQ_MASK_BIM_BG_ERR_EXEC_MASK |
									   ENDAT3_BG_IRQ_MASK_BIM_BG_RSP_DATA_UPDATED_MASK |
									   ENDAT3_BG_IRQ_MASK_BIM_BG_REQ_EMPTY_MASK |
									   ENDAT3_BG_IRQ_MASK_BIM_BG_HANDLER_IDLE_MASK |
									   ENDAT3_BG_IRQ_MASK_BIM_BG_HANDLER_REQ_MASK |
									   ENDAT3_BG_IRQ_MASK_BIM_BG_HANDLER_RSP_MASK |
									   ENDAT3_BG_IRQ_MASK_BIM_BG_HANDLER_ERROR_MASK);
	EnableIRQ(DEMO_ENDAT3_BG_IRQn);
	ENDAT3_BG_Nop(enc_dev.base, 0, 0x123567, &nop_rsp, fg_strobes);
	if (nop_rsp == ((0x01ULL << 40) | 0x123567)) {
		PRINTF("\tSucceed to NOP test.\r\n");
	} else {
		PRINTF("\tFail to NOP test. Recv: 0x%x\r\n", nop_rsp);
	}

	DisableIRQ(DEMO_ENDAT3_BG_IRQn);
	ENDAT3_BG_IRQ_Disable(enc_dev.base, ENDAT3_BG_IRQ_MASK_BIM_BG_ERR_EXEC_MASK |
									   ENDAT3_BG_IRQ_MASK_BIM_BG_RSP_DATA_UPDATED_MASK |
									   ENDAT3_BG_IRQ_MASK_BIM_BG_REQ_EMPTY_MASK |
									   ENDAT3_BG_IRQ_MASK_BIM_BG_HANDLER_IDLE_MASK |
									   ENDAT3_BG_IRQ_MASK_BIM_BG_HANDLER_REQ_MASK |
									   ENDAT3_BG_IRQ_MASK_BIM_BG_HANDLER_RSP_MASK |
									   ENDAT3_BG_IRQ_MASK_BIM_BG_HANDLER_ERROR_MASK);

	PRINTF(" Register accessing performance test\r\n");
	BOARD_InitSysTick();

	/* Enable the FG_IRQ0 when HPF received */
	uint32_t *pos = (uint32_t *)(((uint8_t *)enc_dev.base) + 0x1000);
	uint32_t p;
	SYSTICK_START_COUNT();
	for (int i = 0; i < 1000; i++) {
		p = *pos;
	}


	PRINTF("Register reading: %d\r\n", SYSTICK_GET_COUNT());
	pos = (uint32_t *)(&enc_dev.base->BG_REQ_1);
	p = *pos;
	SYSTICK_START_COUNT();
	for (int i = 0; i < 1000; i++) {
		*pos = p;
	}

	PRINTF("Register writing: %d\r\n", SYSTICK_GET_COUNT());


	EnableIRQ(DEMO_ENDAT3_FG_IRQn);
	/* Enable the FG_IRQ0 when HPF received */
	ENDAT3_FG_IRQ_Enable_With_FIxM_Frame_Count(enc_dev.base, 0, 1);
	for (int i = 0; i < 10; i++) {
		SYSTICK_START_COUNT();
		ENDAT3_FG_Req(enc_dev.base, ENDAT3_FG_REQ_DATA0, 0);
		SDK_DelayAtLeastUs(2000000, SystemCoreClock);
	}
	ENDAT3_FG_IRQ_Disable(enc_dev.base, 0, FIxM_FRAME_CNT_EN);
	DisableIRQ(DEMO_ENDAT3_FG_IRQn);

	PRINTF("Get the position data on sync mode\r\n");
 	/* Get the position data on trigger mode */

 	ENDAT3_FG_Req_without_strobe(enc_dev.base, ENDAT3_FG_REQ_DATA0, 0);

	/* Initialize FlexPWM to generate the trigger signalis. */
	PWM_Trigger_Init(BOARD_PWM_BASEADDR);
	// Enable Interrupt
	EnableIRQ(DEMO_XBARA_IRQn);
	ENDAT3_HW_Strobe_Enable(enc_dev.base);
	while (1){
	}
}
