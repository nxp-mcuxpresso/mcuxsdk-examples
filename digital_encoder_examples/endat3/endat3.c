/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_endat3.h"
#include "fsl_xbar.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)

#define getHexAndEcho() getValueAndEcho(1)
#define getIntAndEcho() getValueAndEcho(0)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
 int getValueAndEcho(int isHex);
 void ENDAT3_RspDump(endat3_rsp_t *rsp);

/*******************************************************************************
 * Variables
 ******************************************************************************/
 ENDAT3_Type *endat3_base;
 int nodes_num = 2;
 int clk_rxtx = ENDAT3_RXTX_RATE_12_5MBPS;
/*******************************************************************************
 * Code
 ******************************************************************************/

static char getChar()
{
	int ret = GETCHAR();
	return (char)(ret & 0xFF);
}

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

static uint32_t SYSTICK_GET_COUNT()
{
	uint32_t val  = SysTick->VAL;
	uint32_t load = SysTick->LOAD;
	return load - val;
}

int getValueAndEcho(int isHex)
{
	char str[17] = {0};
	int index = 0;
	int v32 = 0;
	char ch;
	int ret = 0;

	while (index < 15) {
		ch = getChar();
		PRINTF("%c", ch);
		if(ch == '\r' || ch == '\n')
			break;
		str[index++] = ch;
	};

	if (index > 0) {
		str[16] = '\0';
		if (isHex) {
			ret = sscanf(str, "%x", &v32);
		} else {
			ret = sscanf(str, "%d", &v32);
		}
	}
	PRINTF("\r\n");
	if (ret > 0)
		return v32;
	return ret;
}

static int PrintMainMenu(void)
{
	PRINTF("|-------------------------------------------------------------------------\r\n");
	PRINTF("|	 Operation mode selection: Data Rate: %sMbps\r\n", clk_rxtx == 0 ? "12.5" : "25");
	PRINTF("|--------------------------------------------------------------------------\r\n");
	PRINTF("|\t 1: Point to Point                                                      \r\n");
	PRINTF("|\t 2: Bus operation                                                       \r\n");
	PRINTF("|\t 3: Set the master clk_rxtx to 25Mbps (default: 12.5Mbps)               \r\n");
	PRINTF("|\t 4: Set the master clk_rxtx to 12.5Mbps                                 \r\n");
	PRINTF("|--------------------------------------------------------------------------\r\n");
	PRINTF("Please input: ");
	return getIntAndEcho();
}

static int PrintMenuForBus(void)
{
	PRINTF("|-------------------------------------------------------------------------\r\n");
	PRINTF("|	 Bus Operation: Participants number: %d, Data Rate: %sMbps\r\n", nodes_num, clk_rxtx == 0 ? "12.5" : "25");
	PRINTF("|-------------------------------------------------------------------------\r\n");
	PRINTF("|\t 1: Set the number of participants (defaule: 2)                        \r\n");
	PRINTF("|\t 2: Clear the address for all the participants                         \r\n");
	PRINTF("|\t 3: Assign the address for all the participants                        \r\n");
	PRINTF("|\t 4: Scan the address for all the participants                          \r\n");
	PRINTF("|\t 5: Switch the data transfer rate to  25Mbps                           \r\n");
	PRINTF("|\t 6: Switch the data transfer rate to  12.5Mbps                         \r\n");
	PRINTF("|\t 7: Dump the basic information of the participant                      \r\n");
	PRINTF("|\t 8: Access the memory of the participant                               \r\n");
	PRINTF("|\t 9: FG requests                                                        \r\n");
	PRINTF("|\t10: BG requests                                                        \r\n");
	PRINTF("|\t11: LPF configuration                                                  \r\n");
	PRINTF("|\t12: FID-based response memory dump                                     \r\n");
	PRINTF("|\t13: Safety response memory dump                                        \r\n");
	PRINTF("|\t16: Get Position in irq mode (10 times)                                \r\n");
	PRINTF("|\t17: Get Position in sync mode (Entry any key to stop)                  \r\n");
	PRINTF("|\t18: Exit to Main Menu                                                  \r\n");
	PRINTF("|--------------------------------------------------------------------------\r\n");
	PRINTF("Please input: ");
	return getIntAndEcho();
}

static int PrintMenuForP2P(void)
{
	PRINTF("|-------------------------------------------------------------------------\r\n");
	PRINTF("|	 P2P Operation:  Data Rate: %sMbps\r\n", clk_rxtx == 0 ? "12.5" : "25");
	PRINTF("|-------------------------------------------------------------------------\r\n");
	PRINTF("|\t 1: Active encoder                                                     \r\n");
	PRINTF("|\t 2: Switch the data transfer rate to  25Mbps                           \r\n");
	PRINTF("|\t 3: Switch the data transfer rate to  12.5Mbps                         \r\n");
	PRINTF("|\t 4: Dump the basic information of the participant                      \r\n");
	PRINTF("|\t 5: Access the memory of the participant                               \r\n");
	PRINTF("|\t 6: FG requests                                                        \r\n");
	PRINTF("|\t 7: BG requests                                                        \r\n");
	PRINTF("|\t 8: LPF configuration                                                  \r\n");
	PRINTF("|\t 9: FID-based response memory dump                                     \r\n");
	PRINTF("|\t10: Safety response memory dump                                        \r\n");
	PRINTF("|\t11: Get Position in irq mode (10 times)                                \r\n");
	PRINTF("|\t12: Get Position in sync mode (Entry any key to stop)                  \r\n");
	PRINTF("|\t13: Exit to Main Menu                                                  \r\n");
	PRINTF("|--------------------------------------------------------------------------\r\n");
	PRINTF("Please input: ");
	return getIntAndEcho();
}

static int PrintMemorySel(void)
{
	PRINTF("\tPlease select the encoder memory:\r\n");
	PRINTF("\t1: XL\r\n\t2: XEL\r\n\t3: SET\r\n\t4: XSET\r\n\t5: LPFSET\r\n");
	PRINTF("\t6: OEM1\r\n\t7: OEM2\r\n\t8: OEM3\r\n\t9: SENSORS\r\n\t10: LPFLIVE\r\n");
	PRINTF("\t11: FEATURE\r\n\t12: FEATURE1\r\n\t13: FEATURE2\r\n");
	PRINTF("Please input: ");
	int v32 = getIntAndEcho();
	if (v32 > 0 && v32 < 14) {
		return v32 - 1;
	} else {
		PRINTF("invalid input: %d\r\n", v32);
		return -1;
	}
}

static int PrintLPFMemorySel(void)
{
	PRINTF("\tPlease select the LPF memory:\r\n");
	PRINTF("\t1: LPFSET\r\n");
	PRINTF("\t2: LPFLIVE\r\n");
	PRINTF("Please input: ");
	int v32 = getIntAndEcho();
	if (v32 > 0 && v32 < 3) {
		return v32 - 1;
	} else {
		PRINTF("invalid input: %d\r\n", v32);
		return -1;
	}
}

static int PrintParticipantSel(void)
{
	PRINTF("\tPlease intput the address of the participants: ");
	int ch = getIntAndEcho();
	if (ch > nodes_num) {
		PRINTF("invalid input: %d\r\n", ch);
		return -1;
	}
	return ch;
}

static int PrintFidSel(uint8_t *fid)
{
	uint8_t fidstr[20][18] = {"NOP              ", "POS1             ", "POS2             ", "TOUCHPROBE       ", "POS_ABS          ",
							  "ZERODATA         ", "ERRMSG           ", "EVALNUM          ", "MOUNT0           ", "MOUNT1           ",
							  "COMMU            ", "SENSOR_TEMP_MAX  ", "SENSOR_TEMP_INT  ", "SENSOR_TEMP_M1   ", "SENSOR_TEMP_M2   ",
							  "SENSOR_TEMP_M3   ", "SF_POS1          ", "BGRSP            ", "BGREQ            ", "BUS_ADDR         "};
	uint8_t fid_array[20] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x0A, 0x11, 0x12, 0x13, 0x1A, 0x20, 0x21, 0x22, 0x23, 0x24, 0x50, 0x60, 0x68, 0x70};
	while (1) {
		int i;
		for (i = 0; i < 20; i++) {
			if (i % 5 == 0) {
				PRINTF("\r\n\t");
			}
			PRINTF("%2d: %s", i, fidstr[i]);
		}
		PRINTF("\r\n\t21: Exit..\r\n");
		PRINTF("\r\n\t Please input: ");
		int index = getIntAndEcho();
		if (index > 22) {
			PRINTF("\r\n\tinvalid input\r\n");
			continue;
		}
		if (index == 21) {
			return -1;
		} else {
			*fid = fid_array[index];
			return 0;
		}
	}
}

static uint8_t PrintSendlistSel(void)
{
	uint8_t	 fgReqCodestr[10][22] = {"DATA0     ", "DATA1     ", "DATA2     ", "DATA3      ", "DATA4        ",
									 "DATA5     ", "DATA6     ", "DATA7     ", "DATA       ", "DATANOP      "};
	uint8_t fgReqCode[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

	while (1) {
		PRINTF("\tPlease select the send list :\r\n");
		int i;
		for (i = 0; i < 10; i++) {
			if (i % 5 == 0) {
				PRINTF("\r\n\t");
			}
			PRINTF("%2d: %s", i, fgReqCodestr[i]);
		}
		PRINTF("\r\n\t Please input: ");
		int index = getIntAndEcho();
		if (index > 9) {
			PRINTF("\r\n\tinvalid input\r\n");
			continue;
		}

		return fgReqCode[index];
	}
}

static int PrintBgReqSel(uint8_t *req_code)
{
	uint8_t bgReqCodestr[8][16] = {"NOP       ", "READ     ", "WRITE      ", "RECONFIGURE ", "AUTH     ",
									"PROTECT   ", "SETPASS  ", "LOCATE     "};
	uint8_t bgReqCode[8] = {0x01, 0x02, 0x03, 0x04, 0x80, 0x81, 0x83, 0x84};
	int index;
	while (1) {
		int i;
		for (i = 0; i < 8; i++) {
			if (i % 5 == 0) {
				PRINTF("\r\n\t");
			}

			PRINTF("%2d: %s", i, bgReqCodestr[i]);
		}
		PRINTF("\r\n\t%2d: Back to previous Menu", 9);
		PRINTF("\r\n\tPlease input: ");
		index = getIntAndEcho();
		if (index > 9) {
			PRINTF("\r\n\tinvalid input\r\n");
			continue;
		} else {
			break;
		}
	}
	if (index == 9)
		return -1;

	*req_code = bgReqCode[index];
	return 0;
}

void ENDAT3_ClearAddressForAllParticipants(ENDAT3_Type *base, int nodes_num)
{
	ENDAT3_Bus_Init_ForAllParticipants(base, nodes_num);
	for (int i = 0; i < nodes_num; i++) {
		ENDAT3_FG_Hello(base);
		if (ENDAT3_Bus_Assign_Address(base, 0) != kStatus_Success) {
			PRINTF("Address clearing is failed for participent %d\r\n", i);
		} else {
			PRINTF("Address clearing is successful for participent %d\r\n", i);
		}
	}
}

void ENDAT3_AssignAddressForAllParticipants(ENDAT3_Type *base, int nodes_num)
{
	ENDAT3_Bus_Init_ForAllParticipants(base, nodes_num);
	for (int i = nodes_num; i > 0; i--) {
		ENDAT3_FG_Hello(base);
		if (ENDAT3_Bus_Assign_Address(base, i) != kStatus_Success) {
			PRINTF("Address assignment is failed for participent %d\r\n", i);
			return;
		} else {
			PRINTF("Address assignment is successful for participent %d\r\n", i);
		}
	}
}

void ENDAT3_ScanAddressForAllParticipants(ENDAT3_Type *base, int nodes_num)
{
	for (int i = nodes_num; i >= 0;  i--) {
		ENDAT3_FG_Hello(base);
		if (ENDAT3_FG_Bus_P2P_Hello(base, i) == kStatus_Success) {
			PRINTF("\tAddress %d: active\r\n", i);
		} else {
			PRINTF("\tAddress %d: inactive\r\n", i);
		}
	}

}

void ENDAT3_FIDMEM_Dump(struct FID *fid_res)
{
	uint64_t v64 = fid_res->fid64;
    PRINTF("\tFID_MEM: fid64 = 0x%x%x\r\n", (uint32_t)(v64 >> 32), (uint32_t)(v64 & 0xFFFFFFFF));
    PRINTF("\tFID_MEM: timestamp = 0x%x\r\n", ENDAT3_FIDMEM_GetTimestamp(fid_res));
    PRINTF("\tFID_MEM: lpfv = 0x%x\r\n", ENDAT3_FIDMEM_GetLPFV(fid_res));
	v64 = ENDAT3_FIDMEM_GetData(fid_res);
    PRINTF("\tFID_MEM: LPF_DATA = 0x%x%x\r\n", (uint32_t)(v64 >> 32), (uint32_t)(v64 & 0xFFFFFFFF));
}


void Endat3_DeviceInfoDump(endat3_mem_cache_t *mem_cache)
{
	uint8_t devName[11] = {0};
	uint8_t serial[6];
	uint32_t sn;
	uint64_t ident;
	uint16_t value_u16, phyDatarate;

	value_u16 = ENDAT3_MEM_CACHE_READ_EL_MANUFACTURER(mem_cache);
	PRINTF("Encoder :\r\n\tEL.manufacturer : ");
	switch (value_u16) {
		case 0x0000: PRINTF("Vacant\r\n"); 			break;
		case 0x0001: PRINTF("HEIDENHAIN\r\n"); 		break;
		case 0x0002: PRINTF("AMO\r\n"); 			break;
		case 0x0003: PRINTF("LEINE LINDE\r\n"); 	break;
		case 0x0004: PRINTF("NUMERIK JENA\r\n"); 	break;
		case 0x0005: PRINTF("RSF\r\n"); 			break;
		default:
			PRINTF("Reserved for ID 0x%x\r\n", value_u16);
			break;
	}

	devName[0] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_0(mem_cache);
	devName[1] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_1(mem_cache);
	devName[2] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_2(mem_cache);
	devName[3] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_3(mem_cache);
	devName[4] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_4(mem_cache);
	devName[5] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_5(mem_cache);
	devName[6] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_6(mem_cache);
	devName[7] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_7(mem_cache);
	devName[8] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_8(mem_cache);
	devName[9] = ENDAT3_MEM_CACHE_READ_EL_DEVICENAME_CHAR_9(mem_cache);
	PRINTF("\tEL.deviceName : %s\r\n", devName);

	ident = ENDAT3_MEM_CACHE_READ_EL_DEVICEIDENT(mem_cache);
	PRINTF("\tEL.deviceIdent : %6u-%c%c\r\n", (uint32_t)(ident>>16), (uint8_t)(ident >> 8) & 0xFF, (uint8_t)(ident & 0xFF));

	memcpy(serial, (uint8_t *)&(mem_cache->cacheMem[ENDAT3_MEM_EL_DEVICESERIAL_OFFSET]), 6);
	sn = (serial[4] << 24) | (serial[3] << 16) | (serial[2] << 8) | serial[1];
	PRINTF("\tEL.deviceSerial : %c%u%c\r\n",serial[0], sn, serial[5]);

	value_u16 = mem_cache->cacheMem[ENDAT3_MEM_EL_PROTOCOLFEATURES_OFFSET];
	PRINTF("\tEL.protocolFeatures :\r\n\t\tSupport of bus in daisy-chain mode: %s\r\n", value_u16 & 0x01 ? "Yes": "No");

	phyDatarate = mem_cache->cacheMem[ENDAT3_MEM_EL_PHYDATARATE_OFFSET];
	PRINTF("\tEL.phyDatarate :\r\n\t\tSupport of 12.5 Mbps: %s\r\n", phyDatarate & 0x01 ? "Yes": "No");
	PRINTF("\t\tSupport of 25 Mbps: %s\r\n", phyDatarate & 0x02 ? "Yes": "No");

	value_u16 = mem_cache->cacheMem[ENDAT3_MEM_EL_SUPPORTHPF_0_OFFSET];
	PRINTF("\tEL.supportHpf :\r\n\t\tFirst supported HPF-FID : 0x%x: %s\r\n", value_u16 & 0xFF, ENDAT3_FID2str(value_u16 & 0xFF));
	PRINTF("\t\tSecond supported HPF-FID : 0x%x: %s\r\n", value_u16 >> 8,ENDAT3_FID2str(value_u16 >> 8));
	value_u16 = mem_cache->cacheMem[ENDAT3_MEM_EL_SUPPORTHPF_1_OFFSET];
	PRINTF("\t\tThird supported HPF-FID : 0x%x: %s\r\n", value_u16 & 0xFF, ENDAT3_FID2str(value_u16 & 0xFF));
	PRINTF("\t\tFourth supported HPF-FID : 0x%x: %s\r\n", value_u16 >> 8, ENDAT3_FID2str(value_u16 >> 8));
}

void Endat3_DeviceFeatureDump(endat3_mem_cache_t *mem_cache, uint16_t phyDatarate)
{
	uint16_t value_u16, encoder_type;
	if (phyDatarate & 0x01) {
		value_u16 = mem_cache->cacheMem[ENDAT3_MEM_XEL_TIMEHPFOUT_RATE_0_OFFSET];
		if (value_u16 != 0)
			PRINTF("\r\n\tXEL.timeHPFout[0] : %dns for 12.5Mhz\r\n", value_u16 * 10);
	}

	if (phyDatarate & 0x02) {
		value_u16 = mem_cache->cacheMem[ENDAT3_MEM_XEL_TIMEHPFOUT_RATE_1_OFFSET];
		if (value_u16 != 0x0000)
			PRINTF("\tXEL.timeHPFout[1] : %dns for 25Mhz\r\n", value_u16 * 10);
	}

	encoder_type = ENDAT3_MEM_CACHE_READ_XEL_ENCODERTYPE(mem_cache);
	switch (encoder_type) {
		case 0x0000: PRINTF("\tXEL.encoderType: Not available\r\n"); break;
		case 0x0001: PRINTF("\tXEL.encoderType: Rotary absolute\r\n"); break;
		case 0x0002: PRINTF("\tXEL.encoderType: Rotary incremental\r\n"); break;
		case 0x0003: PRINTF("\tXEL.encoderType: Linear absolute\r\n"); break;
		case 0x0004: PRINTF("\tXEL.encoderType: Linear incremental\r\n"); break;
		default:     PRINTF("\tXEL.encoderType: Reserved\r\n"); break;
	}

	PRINTF("\tSupport of 1 Vpp analog signals: ");
	if (ENDAT3_MEM_CACHE_READ_XEL_DEVICEFEATURE_SUPP_1VPP(mem_cache)) {
		PRINTF("y\r\n");
	} else {
		PRINTF("n\r\n");
	}

	PRINTF("\tSupport of 1 TTL/HTL signals: ");
	if (ENDAT3_MEM_CACHE_READ_XEL_DEVICEFEATURE_SUPP_TTL(mem_cache)) {
		PRINTF("y\r\n");
	} else {
		PRINTF("n\r\n");
	}

	PRINTF("\tSupport of XSET.offsetPos: ");
	if (ENDAT3_MEM_CACHE_READ_XEL_DEVICEFEATURE_SUPP_OFFSETPOS(mem_cache)) {
		PRINTF("y\r\n");
	} else {
		PRINTF("n\r\n");
	}

	PRINTF("\tSupport of XSET.tempWarnLevel: ");
	if (ENDAT3_MEM_CACHE_READ_XEL_DEVICEFEATURE_SUPP_TEMPWARN(mem_cache)) {
		PRINTF("y\r\n");
	} else {
		PRINTF("n\r\n");
	}

	PRINTF("\tSupport of a battery-buffered multiturn (BBMT): ");
	if (ENDAT3_MEM_CACHE_READ_XEL_DEVICEFEATURE_SUPP_BBMT(mem_cache)) {
		PRINTF("y\r\n");
	} else {
		PRINTF("n\r\n");
	}

	PRINTF("\tFunctional safety is supported: ");
	if (ENDAT3_MEM_CACHE_READ_XEL_SAFETYBITS(mem_cache)) {
		PRINTF("y\r\n");
	} else {
		PRINTF("n\r\n");
	}

	value_u16 = mem_cache->cacheMem[ENDAT3_MEM_XEL_FORMATPOS1ABS_OFFSET];
	PRINTF("");
	if (encoder_type == 0x0001 || encoder_type == 0x0002) {
		/* Format for angles */
		PRINTF("\tXEL.formatPos1Abs : MT %dBits, ST:%dBits\r\n" , value_u16 >> 8, 32 - (value_u16 & 0xFF));
	} else if ( encoder_type == 0x0003 || encoder_type == 0x0004) {
		/* Format for Linear */
		PRINTF("\tXEL.formatPos1Abs : resolution:%d\r\n" , 32 - (value_u16 & 0xFF));
	}
}

void ENDAT3_DumpParticipantInfomation(ENDAT3_Type *base, uint8_t bus_addr)
{
	status_t status;
	uint16_t cache_buf[0x100] = {0};
	endat3_mem_cache_t mem_cache;
	uint16_t phyDatarate;

	if (bus_addr > 0) {
		if ((status = ENDAT3_Bus_P2P_memCacheInit(base, bus_addr, ENDAT3_MEM_BASE_EL, &mem_cache, cache_buf, 0x100, 1)) != kStatus_Success) {
			PRINTF("%s failed: status=%d\r\n", __func__, status);
			return;
		}
	
		if ((status = ENDAT3_Bus_P2P_memCacheFetch(base, bus_addr, &mem_cache, 1)) != kStatus_Success) {
			PRINTF("%s failed: status=%d\r\n", __func__, status);
			return;
		}
	} else {
		if ((status = ENDAT3_memCacheInit(base, ENDAT3_MEM_BASE_EL, &mem_cache, cache_buf, 0x100, 1)) != kStatus_Success) {
			PRINTF("%s failed: status=%d\r\n", __func__, status);
			return;
		}

		if ((status = ENDAT3_memCacheFetch(base, &mem_cache, 1)) != kStatus_Success) {
			PRINTF("%s failed: status=%d\r\n", __func__, status);
			return;
		}
	}

	if (ENDAT3_memCacheCheckCS(&mem_cache) != kStatus_Success) {
		PRINTF("\tAddress : EL cache CS check error\t\n");
		return;
	}

	Endat3_DeviceInfoDump(&mem_cache);

	if (bus_addr > 0) {
		if ((status = ENDAT3_Bus_P2P_memCacheInit(base, bus_addr, ENDAT3_MEM_BASE_XEL, &mem_cache, cache_buf, 0x100, 1)) != kStatus_Success) {
			PRINTF("%s failed: status=%d\r\n", __func__, status);
			return;
		}

		if ((status = ENDAT3_Bus_P2P_memCacheFetch(base, bus_addr, &mem_cache, 1)) != kStatus_Success) {
			PRINTF("%s failed: status=%d\r\n", __func__, status);
			return;
		}
	} else {
		if ((status = ENDAT3_memCacheInit(base, ENDAT3_MEM_BASE_XEL, &mem_cache, cache_buf, 0x100, 1)) != kStatus_Success) {
			PRINTF("%s failed: status=%d\r\n", __func__, status);
			return;
		}

		if ((status = ENDAT3_memCacheFetch(base, &mem_cache, 1)) != kStatus_Success) {
			PRINTF("%s failed: status=%d\r\n", __func__, status);
			return;
		}
	}

	if (ENDAT3_memCacheCheckCS(&mem_cache) != kStatus_Success) {
		PRINTF("\tCS check error\r\n", bus_addr);
		return;
	}

	if (bus_addr > 0) {
		status = ENDAT3_Bus_P2P_memRead(base, bus_addr, ENDAT3_MEM_BASE_EL + ENDAT3_MEM_EL_PHYDATARATE_OFFSET, 1, &phyDatarate, 1);
	} else {
		status = ENDAT3_memRead(base, ENDAT3_MEM_BASE_EL + ENDAT3_MEM_EL_PHYDATARATE_OFFSET,  1, &phyDatarate, 1);
	}
	if (status != kStatus_Success) {
		PRINTF("\tMemory Read Error\r\n");
		return;
	}
	Endat3_DeviceFeatureDump(&mem_cache, phyDatarate);
}

void ENDAT3_ModifyMemory_Help()
{
	PRINTF("help: \r\n");
	PRINTF("\tmd address <len> ; Read memory\r\n");
	PRINTF("\tmw address value ; write memory\r\n");
	PRINTF("\tdump ; dump the memory\r\n");
	PRINTF("\tsave ; Save the modification\r\n");
	PRINTF("\texit ; Exit without saving\r\n");
}

void ENDAT3_ModifyMemory(ENDAT3_Type *base, uint8_t bus_addr, uint32_t mem_index)
{
	uint32_t mem_base_array[14] = {ENDAT3_MEM_BASE_EL, ENDAT3_MEM_BASE_XEL, ENDAT3_MEM_BASE_SET,
		ENDAT3_MEM_BASE_XSET, ENDAT3_MEM_BASE_LPFSET, ENDAT3_MEM_BASE_OEM1,
		ENDAT3_MEM_BASE_OEM2, ENDAT3_MEM_BASE_OEM3,ENDAT3_MEM_BASE_SENSORS,
		ENDAT3_MEM_BASE_LPFLIVE, ENDAT3_MEM_BASE_FEATURE, ENDAT3_MEM_BASE_FEATURE1, ENDAT3_MEM_BASE_FEATURE2};
	status_t status;
	uint16_t cache_buf[0x100] = {0};
	endat3_mem_cache_t mem_cache;
	(void)status;

	if (bus_addr > 0) {
		if ((status = ENDAT3_Bus_P2P_memCacheInit(base, bus_addr, mem_base_array[mem_index], &mem_cache, cache_buf, 0x100, 1)) != kStatus_Success) {
			PRINTF("\tAddress%d : Failed to initialize memory cache.\r\n", bus_addr);
			return;
		}

		if ((status = ENDAT3_Bus_P2P_memCacheFetch(base, bus_addr, &mem_cache, 1)) != kStatus_Success) {
			PRINTF("\tAddress%d : Failed to fetch memory cache.\r\n", bus_addr);
			return;
		}
	} else {
		if ((status = ENDAT3_memCacheInit(base, mem_base_array[mem_index], &mem_cache, cache_buf, 0x100, 1)) != kStatus_Success) {
			PRINTF("\tEncoder : Failed to initialize memory cache.\r\n");
			return;
		}

		if ((status = ENDAT3_memCacheFetch(base, &mem_cache, 1)) != kStatus_Success) {
			PRINTF("\tEncoder : Failed to fetch memory cache.\r\n");
			return;
		}
	}

	if (ENDAT3_memCacheCheckCS(&mem_cache) != kStatus_Success) {
		PRINTF("\tAddress%d : CS check error\r\n", bus_addr);
		return;
	}

	PRINTF("Memory:\r\n");
	for (int i = 0; i < mem_cache.memSize; i += 8) {
		PRINTF("\t 0x%x : ", mem_cache.memBase + i);
		for (int j = i; j < mem_cache.memSize && j < i + 8; j++ ) {
			PRINTF("0x%x  ", mem_cache.cacheMem[j]);
		}
		PRINTF("\r\n");
	}

	while (1) {
		PRINTF("=> ");
		char str[65] = {0};
		char cmd_buf[16] = {0};
		char *cmd = NULL;
		int index = 0;
		int address, value, addr;
		char ch;
		index = 0;
		while (index < 63) {
			ch = getChar();
			PRINTF("%c", ch);
			if(ch == '\r' || ch == '\n')
				break;
			str[index++] = ch;
		};

		if (index > 0) {
			str[64] = '\0';
		}
		PRINTF("\r\n");

		int num = sscanf(str, "%s %x %x", cmd_buf, &address, &value);

		PRINTF("%s %d %d\r\n", cmd_buf, address, value);
		for (int i = 0; i < 16; i++) {
			if (cmd_buf[i] != ' ') {
				cmd = &cmd_buf[i];
				break;
			}
		}

		if (num > 1) {
			if (address < mem_cache.memBase || address > mem_cache.memBase + mem_cache.memSize) {
				PRINTF("Invalid input\r\n");
				continue;
			}

			addr = address - mem_cache.memBase;
			if (strncmp(cmd, "md", 16) == 0) {
				if (num == 2)
					value = 8;

				for (int i = 0; i < value; i += 8) {
					PRINTF("0x%x : ", address + i);
					for (int j = i; j < value && j < i + 8; j++ ) {
						PRINTF("0x%x  ", mem_cache.cacheMem[addr + j]);
					}
					PRINTF("\r\n");
				}
			} else if (strncmp(cmd, "mw", 16) == 0) {
				if (num < 3) {
					PRINTF("Invalid input\r\n");
					continue;
				} else {
					mem_cache.cacheMem[addr] = value;
					ENDAT3_memCacheSetDirty(&mem_cache, addr, 1);
					PRINTF("\r\n");
					for (int i = 0; i < MAX_MEMORY_AREA_SIZE/32; i++)
						PRINTF("0x%x ", mem_cache.dirtyWordMap[i]);
					PRINTF("\r\n");
				}
			}
		} else if (num == 1) {
			if (strncmp(cmd, "save", 16) == 0) {
				ENDAT3_memCacheUpdataCS(&mem_cache);
				if (bus_addr > 0) {
					ENDAT3_Bus_P2P_memCacheFlush(base, bus_addr, &mem_cache, 1);
					if ((status = ENDAT3_Bus_P2P_memCacheFetch(base, bus_addr, &mem_cache, 1)) != kStatus_Success) {
						PRINTF("\tAddress%d : Failed to fetch memory cache.\r\n", bus_addr);
					}
				} else {
					ENDAT3_memCacheFlush(base, &mem_cache, 1);
					if ((status = ENDAT3_memCacheFetch(base, &mem_cache, 1)) != kStatus_Success) {
						PRINTF("\tEncoder : Failed to fetch memory cache.\r\n");
					}
				}

				if (ENDAT3_memCacheCheckCS(&mem_cache) != kStatus_Success) {
					PRINTF("\tAddress%d : CS check error\r\n", bus_addr);
				}

				continue;
			} else if (strncmp(cmd, "exit", 16) == 0) {
				return;
			} else if (strncmp(cmd, "dump", 16) == 0) {
				PRINTF("Memory:\r\n");
				for (int i = 0; i < mem_cache.memSize; i += 8) {
					PRINTF("\t 0x%x : ", mem_cache.memBase + i);
					for (int j = i; j < mem_cache.memSize && j < i + 8; j++ ) {
						PRINTF("0x%x  ", mem_cache.cacheMem[j]);
					}
					PRINTF("\r\n");
				}
			} else {
				ENDAT3_ModifyMemory_Help();
			}
		} else {
			ENDAT3_ModifyMemory_Help();
		}
	}
}

void ENDAT3_PosDump(endat3_rsp_t *rsp)
{
	uint64_t position;
	position = ENDAT3_READ_HPF_DATA(rsp->hpf.hpf64);

	if ((rsp->hpf.hpf.status | 0x1F) != 0x0C) {
		if (ENDAT3_READ_HPF_STATUS_F(rsp->hpf.hpf64)) {
			PRINTF("The position data might be wrong and should therefore no longer be used. The cause for f being set can be read through the LPF ERRMSG\r\n");
			return;
		}

		if (ENDAT3_READ_HPF_STATUS_W(rsp->hpf.hpf64)) {
			PRINTF("The position data is correct, but errors might occur soon. The cause for W being set can be read through the LPF ERRMSG.\r\n");
			return;
		}

		if (ENDAT3_READ_HPF_STATUS_HPFV(rsp->hpf.hpf64) == 0x0) {
			PRINTF("The field HPF.DATA does not contain the configured contents. One of the error codes is being transmitted\r\n");
			return;
		}

		if (ENDAT3_READ_HPF_STATUS_RM(rsp->hpf.hpf64) == 0x0) {
			PRINTF("Absolute value cannot be made available\r\n");
			return;
		}

		if (ENDAT3_READ_HPF_STATUS_ERR_REQ(rsp->hpf.hpf64)) {
			PRINTF("The request code does not support or that is not defined\r\n");
			return;
		}
	}

	uint16_t multiturn  = position >> 32;
	uint32_t singleturn = position & 0xFFFFFFFF;
	PRINTF("\tMT: %5u ST: %10u\r\n", multiturn, singleturn);
}

void dump_postion(void)
{
	endat3_rsp_t rsp;

	PRINTF("Enc %d: ", 0);
	ENDAT3_getRsp(endat3_base, 0, &rsp);
	ENDAT3_PosDump(&rsp);
}

void dump_bus_postion(void)
{
	endat3_rsp_t rsp;
	for (int i = nodes_num; i > 0; i--) {
		 PRINTF("Enc %d: ", i);
		ENDAT3_getRsp(endat3_base, i, &rsp);
		ENDAT3_PosDump(&rsp);
	}
}

void ENDAT3_RspDump(endat3_rsp_t *rsp)
{
	uint16_t errCode;
	uint64_t v64 = ENDAT3_READ_HPF_DATA(rsp->hpf.hpf64);
	PRINTF("\tHPF.STATUS: 0x%x(F: %d, W: %d, HPFV: %d, RM: %d, ERR_REQ: %d)\r\n", ENDAT3_READ_HPF_STATUS(rsp->hpf.hpf64), ENDAT3_READ_HPF_STATUS_F(rsp->hpf.hpf64),
			ENDAT3_READ_HPF_STATUS_W(rsp->hpf.hpf64), ENDAT3_READ_HPF_STATUS_HPFV(rsp->hpf.hpf64),
			ENDAT3_READ_HPF_STATUS_RM(rsp->hpf.hpf64), ENDAT3_READ_HPF_STATUS_ERR_REQ(rsp->hpf.hpf64));
	PRINTF("\tHPF.DATA: 0x%x%x\r\n", (uint32_t)(v64 >> 32), (uint32_t)(v64 & 0xFFFFFFFF));
	PRINTF("\tHPF.CS: 0x%x\r\n", ENDAT3_READ_HPF_CS(rsp->hpf.hpf64));

	if (ENDAT3_READ_HPF_STATUS_HPFV(rsp->hpf.hpf64) == 0) {
		errCode = ENDAT3_READ_HPF_ERRORCODE(rsp->hpf.hpf64);
		PRINTF("\tHPF invalid [0x%x: %s]\r\n", errCode, ENDAT3_Err2str(errCode));
	} else {
		PRINTF("\tLPH.STATUS: 0x%x (BG_STATUS: %d  BG_ERR_EXEC: %d  ZACT: %d)\r\n", ENDAT3_READ_LPH_STATUS(rsp->lph.lph32),
			ENDAT3_READ_LPH_STATUS_BG_STATUS(rsp->lph.lph32), ENDAT3_READ_LPH_STATUS_BG_ERR_EXEC(rsp->lph.lph32),
			ENDAT3_READ_LPH_STATUS_ZACT(rsp->lph.lph32));
		PRINTF("\tLPH.NLPF: 0x%x (YACT: %d  XDIM: %d)\r\n", ENDAT3_READ_LPH_NLPF(rsp->lph.lph32),
			ENDAT3_READ_LPH_NLPF_YACT(rsp->lph.lph32), ENDAT3_READ_LPH_NLPF_XDIM(rsp->lph.lph32));
		PRINTF("\tLPH.CS: 0x%x\r\n", ENDAT3_READ_LPH_CS(rsp->lph.lph32));

		uint8_t i;
		char *str;
		for (i = 0; i < rsp->xdim; i++) {
			uint8_t fid = ENDAT3_READ_LPF_STATUS_FID(rsp->lpf[i].lpf64);
			uint8_t lpfv = ENDAT3_READ_LPF_STATUS_LPFV(rsp->lpf[i].lpf64);
			str = ENDAT3_FID2str(fid);
			uint8_t status = ENDAT3_READ_LPF_STATUS(rsp->lpf[i].lpf64);
			PRINTF("\tLPF[%d].STATUS: 0x%x (FID: 0x%x [%s] LPFV: %d)\r\n", i,status , fid, str, lpfv);
			if (lpfv == 0) {
				errCode = ENDAT3_READ_LPF_ERRORCODE(rsp->lpf[i].lpf64);
				PRINTF("\t\tLPF invalid [0x%x: %s]\r\n", errCode, ENDAT3_Err2str(errCode));
				continue;
			}
			PRINTF("\tLPF[%d].DATA: 0x%x\r\n", i, ENDAT3_READ_LPF_DATA(rsp->lpf[i].lpf64));
			PRINTF("\tLPF[%d].CS: 0x%x\r\n", i, ENDAT3_READ_LPF_CS(rsp->lpf[i].lpf64));
		}
	}
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
		PRINTF("\tHPF.DATA:     0x%012\" PRIx64 \"\r\n", ENDAT3_READ_HPF_DATA(hpf->hpf64));
		PRINTF("\tHPF.DATA:     0x%012llx\r\n", ENDAT3_READ_HPF_DATA(hpf->hpf64));
		PRINTF("\tHPF.CS:       0x%02x\r\n", ENDAT3_READ_HPF_CS(hpf->hpf64));
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
		PRINTF("\tLPF[%2d].CS:          0x%02x\r\n", ENDAT3_READ_LPF_CS(lpf->lpf64));
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
	PRINTF("\tLPF: singleturn = 0x%x. multiturn = 0x%x\r\n", ENDAT3_READ_POS_SINGLETURN_LPF(lpf->lpf64), ENDAT3_READ_POS_MULTITURN_LPF(lpf->lpf64));
	PRINTF("\tLPF[%2d].CS: 0x%02x\r\n", ENDAT3_READ_LPF_CS(lpf->lpf64));

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
	PRINTF("\tLPF[%2d].CS:          0x%02x\r\n", ENDAT3_READ_LPF_CS(lpf->lpf64));
}

/* Triggered by the second PWM trigger signal via XBAR */
void DEMO_XBARA_IRQHandler(void)
{
	if (nodes_num > 0) {
		dump_bus_postion();
	} else {
		dump_postion();
	}
	XBAR_ClearOutputStatusFlag(kXBAR1_OutputEdma4IpdReq76);
	SDK_ISR_EXIT_BARRIER;
}

/* FG_IRQ0_IRQHandler */
void FG_IRQ0_IRQHandler(void)
{
	uint32_t time = SYSTICK_GET_COUNT();
	PRINTF("The minimal communication cycle: %d\r\n", time);
	if (nodes_num > 0) {
		dump_bus_postion(); 
	} else {
		dump_postion();
	}
	ENDAT3_IRQ_Clear(endat3_base, CLEAR_FG_IRQ0);
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
	if (endat3_base->FG_STATUS | ENDAT3_FG_STATUS_BG_IRQ_STATE_MASK) {
		PRINTF("BG_RSP_1: 0x%x\r\n", endat3_base->BG_RSP_1);
	}
	ENDAT3_IRQ_Clear(endat3_base, CLEAR_BG_IRQ);
	SDK_ISR_EXIT_BARRIER;
}

void DEMO_FG_IRQ_IRQHandler(void)
{
	if (endat3_base->FG_STATUS | ENDAT3_FG_STATUS_FG_IRQ0_STATE_MASK) {
		FG_IRQ0_IRQHandler();
	}

	if (endat3_base->FG_STATUS | ENDAT3_FG_STATUS_FG_IRQ1_STATE_MASK) {
		FG_IRQ1_IRQHandler();
	}

	if (endat3_base->FG_STATUS | ENDAT3_FG_STATUS_FG_IRQ2_STATE_MASK) {
		FG_IRQ2_IRQHandler();
	}

	SDK_ISR_EXIT_BARRIER;
}

void ENDAT3_DumpPostionInSync(ENDAT3_Type *base, uint8_t data_req)
{
	ENDAT3_FG_Req(base, data_req, 0);
	/* Initialize FlexPWM to generate the trigger signalis. */
	PWM_Trigger_Init(BOARD_PWM_BASEADDR);
	// Enable Interrupt
	EnableIRQ(DEMO_XBARA_IRQn);
	ENDAT3_HW_Strobe_Enable(base);

	getChar();
	DisableIRQ(DEMO_XBARA_IRQn);
	ENDAT3_HW_Strobe_Disable(base);
}

void ENDAT3_Bus_DumpPostionInSync(ENDAT3_Type *base, uint8_t data_req)
{
	for (int i = nodes_num; i > 0; i--) {
		ENDAT3_FG_Bus_BC_with_FG_Req_Rsp(base, i, data_req, 0);
	}

	ENDAT3_FG_Bus_BC_Req(base);
	/* Initialize FlexPWM to generate the trigger signalis. */
	PWM_Trigger_Init(BOARD_PWM_BASEADDR);
	// Enable Interrupt
	EnableIRQ(DEMO_XBARA_IRQn);
	ENDAT3_HW_Strobe_Enable(base);

	getChar();
	DisableIRQ(DEMO_XBARA_IRQn);
	ENDAT3_HW_Strobe_Disable(base);
}

void ENDAT3_DumpPostionInIrq(ENDAT3_Type *base, uint8_t data_req)
{
	BOARD_InitSysTick();
	EnableIRQ(DEMO_ENDAT3_FG_IRQn);
	/* Enable the FG_IRQ0 when HPF received */
	ENDAT3_FG_IRQ_Enable_With_FIxM_Frame_Count(base, 0, 1);

	for (int i = 0; i < 10; i++) {
		PRINTF("Test %d\r\n", i);
		SYSTICK_START_COUNT();
		ENDAT3_FG_Req(base, data_req, 0);
		SDK_DelayAtLeastUs(2000000, SystemCoreClock);
		PRINTF("\r\n");
	}

	ENDAT3_FG_IRQ_Disable(base, 0, FIxM_FRAME_CNT_EN);
	DisableIRQ(DEMO_ENDAT3_FG_IRQn);
}

void ENDAT3_Bus_DumpPostionInIrq(ENDAT3_Type *base, int32_t nodes_num, uint8_t data_req)
{
	BOARD_InitSysTick();
	EnableIRQ(DEMO_ENDAT3_FG_IRQn);
	/* Enable the FG_IRQ0 when HPF received */
	ENDAT3_FG_IRQ_Enable_With_FIxM_Bus_Address_Count(base, 0, nodes_num);

	/* Active the send list data_req for all participants. */
	for (int i = nodes_num; i > 0; i--) {
		ENDAT3_FG_Bus_BC_with_FG_Req_Rsp(base, i, data_req, 0);
	}

	for (int i = 0; i < 10; i++) {
		PRINTF("Test %d\r\n", i);
		SYSTICK_START_COUNT();
		ENDAT3_FG_Bus_BC_Req(base);
		SDK_DelayAtLeastUs(2000000, SystemCoreClock);
		PRINTF("\r\n");
	}
	ENDAT3_FG_IRQ_Disable(base, 0, FIxM_ADDR_CNT_EN);
	DisableIRQ(DEMO_ENDAT3_FG_IRQn);
}

uint16_t PrintFgReqDataClear(void)
{
	uint16_t v16 = 0;
	char ch;
	PRINTF("\tReset errors? please input <y/n>: ");
	ch = getChar();
	PRINTF("%c\r\n", ch);
	if (ch == 'y' || ch == 'Y')
		v16 = 0x01;

	PRINTF("\tReset the warning (W)? please input <y/n>: ");
	ch = getChar();
	PRINTF("%c\r\n", ch);
	if (ch == 'y' || ch == 'Y')
		v16 |= 0x02;

	PRINTF("\tClear the absolute value? please input <y/n>: ");
	ch = getChar();
	PRINTF("%c\r\n", ch);
	if (ch == 'y' || ch == 'Y')
		v16 |= 0x04;

	return v16;
}

uint16_t PrintFgReqDataEcho(void)
{
	PRINTF("\tplease input an arbitrary 2Bytes data for ECHO: 0x");

	int v32 = getHexAndEcho();
	PRINTF("\r\n");
	return v32 & 0xFFFF;
}

uint16_t PrintFgReqDataRate(void)
{
	PRINTF("\t1: Switch to 12.5 Mbps, 2 : Switch to 25 Mbps\r\n");

	int v32 = getIntAndEcho();
	PRINTF("\r\n");
	if (v32 == 1) {
		return ENDAT3_FG_DATA_RATE_12_5MBPS;
	} else {
		return ENDAT3_FG_DATA_RATE_25_MBPS;
	}
}

int PrintFgReqSel(uint8_t *req_code, uint16_t *req_data)
{
	uint8_t	 fgReqCodestr[17][22] = {"DATA0     ", "DATA1     ", "DATA2     ", "DATA3      ", "DATA4        ",
									"DATA5     ", "DATA6     ", "DATA7     ", "DATA       ", "DATANOP      ",
									"RESET     ", "CLEAR     ", "ECHO      ", "RATE       ", "HELLO        ",
									"BUSINIT   ", "FORCE     "};
	uint8_t fgReqCode[17] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0B, 0x0C, 0x0E, 0x10, 0x22, 0x82, 0x90};
	int index;
	while (1) {
		int i;
		for (i = 0; i < 17; i++) {
			if (i % 5 == 0) {
				PRINTF("\r\n\t");
		}

		PRINTF("%2d: %s", i, fgReqCodestr[i]);
		}
		PRINTF("\r\n\t%2d: Back to previous Menu", 17);
		PRINTF("\r\n\tPlease input: ");
		index = getIntAndEcho();
		if (index > 18) {
			PRINTF("\r\n\tinvalid input\r\n");
			continue;
		} else {
			break;
		}
	}
	if (index == 17)
		return -1;

	*req_code = fgReqCode[index];
	switch(*req_code) {
		case ENDAT3_FG_REQ_CLEAR: *req_data = PrintFgReqDataClear(); break;
		case ENDAT3_FG_REQ_ECHO: *req_data = PrintFgReqDataEcho(); break;
		case ENDAT3_FG_REQ_RATE: *req_data = PrintFgReqDataRate(); break;
		case ENDAT3_FG_REQ_RESET: *req_data =ENDAT3_FG_DATA_RESET; break;
		default: *req_data = 0x0000;
	}
	return 0;
}

void Endat3_FG_REQ(ENDAT3_Type *base)
{
	uint8_t req_code;
	uint16_t req_data;

	endat3_rsp_t rsp;
	while (1) {
		if (PrintFgReqSel(&req_code, &req_data) < 0) {
			return;
		}

		switch (req_code) {
			case ENDAT3_FG_REQ_ECHO:
				if (ENDAT3_FG_Echo(base, req_data) == kStatus_Success) {
					PRINTF("\tECHO test successful \r\n");
				} else {
					PRINTF("\tECHO test failed \r\n");
				}
				break;
			default:
				if (ENDAT3_FG_Req_Rsp(base, req_code, req_data, &rsp) == kStatus_Success) {
					ENDAT3_RspDump(&rsp);
				}
				break;

		}
	}
}

void Endat3_BUS_FG_REQ(ENDAT3_Type *base, int8_t bus_addr)
{
	uint8_t req_code;
	uint16_t req_data;

	endat3_rsp_t rsp;
	while (1) {
		if (PrintFgReqSel(&req_code, &req_data) < 0) {
			return;
		}

		switch (req_code) {
			case ENDAT3_FG_REQ_ECHO:
				if (ENDAT3_FG_Bus_P2P_Echo(base, bus_addr, req_data) == kStatus_Success) {
					PRINTF("\tECHO test successful \r\n");
				} else {
					PRINTF("\tECHO test failed \r\n");
				}
				break;
			default:
				if (ENDAT3_FG_Bus_P2P_Req_Rsp(base, bus_addr, req_code, req_data, &rsp) == kStatus_Success) {
					ENDAT3_RspDump(&rsp);
				}
				break;

		}
	}
}

void getBGREQ_Nop(uint64_t *arbitrary)
{
	PRINTF("\tPlease input an arbitrary data : 0x");
	*arbitrary = getHexAndEcho();
}

void getBGREQ_Read(uint32_t *address, uint8_t *num)
{
	PRINTF("\tPlease input the address : 0x");
	*address = getHexAndEcho();
	PRINTF("\tPlease input the num_words : 0x");
	*num = getHexAndEcho();
}

void getBGREQ_Write(uint32_t *address, uint16_t *word)
{
	PRINTF("\tPlease input the address : 0x");
	*address = getHexAndEcho();
	PRINTF("\tPlease input the value : 0x");
	*word = (uint16_t)getHexAndEcho();
}

void getBGREQ_Auth_SetPass(uint16_t *usrlevel, uint16_t *pass)
{
	uint32_t v32;
	PRINTF("\tPlease select the usrlevel:\r\n\t 1, User; 2, OEM2; 3, OEM1; 4, MANUFACTURER\r\n");
	while (1) {
		PRINTF("\tPlease input: ");
		v32 = getIntAndEcho();
		if (v32 > 4 || v32 < 1) {
			continue;
		}
		*usrlevel = v32 - 1;
		break;
	}
	PRINTF("\tPlease input the pass : 0x");
	*pass = getHexAndEcho();
}

void getBGREQ_Protect(uint32_t *address, uint8_t *mode, uint16_t *acclevel)
{
	uint32_t v32;
	PRINTF("\tPlease input the address : 0x");
	*address = getHexAndEcho();

	PRINTF("\tPlease select the mode:\r\n");
	PRINTF("\t1, QUERY queries the current access levels\r\n");
	PRINTF("\t2, SET_READ sets the access level for read-accesses\r\n");
	PRINTF("\t3, SET_WRITE sets the access level for write-accesses\r\n");
	while (1) {
		PRINTF("\tPlease input: ");
		v32 = getIntAndEcho();
		if (v32 > 3 || v32 < 1) {
			continue;
		}
		*mode = v32;
		break;
	}

	if (*mode != ENDAT3_BG_PROTECT_MODE_QUERY) {
		PRINTF("\tPlease select the acclevel:\r\n\t 1, User; 2, OEM2; 3, OEM1; 4, MANUFACTURER\r\n");
		while (1) {
			PRINTF("\tPlease input: ");
			v32 = getIntAndEcho();
			if (v32 > 4 || v32 < 1) {
				continue;
			}
			*acclevel = v32 - 1;
			break;
		}
	}
}

void getBGREQ_Locate(uint8_t *ctrl)
{
	PRINTF("\tPlease input the ctrl value : 0x");
	*ctrl = getHexAndEcho();
}


void Endat3_BG_REQ(ENDAT3_Type *base)
{
	uint8_t req_code;

	while (1) {
		if (PrintBgReqSel(&req_code) < 0) {
			return;
		}

		switch (req_code) {
			case ENDAT3_BG_OPCODE_NOP: {
				uint64_t arbitrary_req = 0;
				uint64_t arbitrary_res = 0;
				getBGREQ_Nop(&arbitrary_req);
				ENDAT3_BG_Nop(base, arbitrary_req, &arbitrary_res, 1);
				if (arbitrary_res == ((0x01ULL << 40) | arbitrary_req)) {
					PRINTF("\tNOP request successful.\r\n");
				} else {
					PRINTF("\tNOP request failed.\r\n");
				}
				break;
			}
			case ENDAT3_BG_OPCODE_RECONFIGURE: {
				if (ENDAT3_BG_Reconfigure(base, 1) == kStatus_Success) {
					PRINTF("\tBG Reconfigure request successful \r\n");
				} else {
					PRINTF("\tBG Reconfigure request failed \r\n");
				}
				break;
			}
			case ENDAT3_BG_OPCODE_READ: {
				uint32_t address_read;
				uint8_t num_read;
				uint16_t words_read[3];
				getBGREQ_Read(&address_read, &num_read);
				if (ENDAT3_BG_Read(base, address_read, num_read, words_read, 1) == kStatus_Success) {
					for (int i = 0; i < num_read; i++) {
						PRINTF("\t word[%d]: 0x%x", i, words_read[i]);
					}
				} else {
					PRINTF("\tBG Read request failed\r\n");
				}
				break;
			}
			case ENDAT3_BG_OPCODE_WRITE: {
				uint32_t address_write;
				uint16_t word_write;
				getBGREQ_Write(&address_write, &word_write);
				if (ENDAT3_BG_Write(base, address_write, word_write, 1) == kStatus_Success) {
					PRINTF("\tBG write request successful \r\n");
				} else {
					PRINTF("\tBG write request failed \r\n");
				}
				break;
			}
			case ENDAT3_BG_OPCODE_AUTH: {
				uint16_t usrlevel_auth;
				uint16_t pass_auth;
				getBGREQ_Auth_SetPass(&usrlevel_auth, &pass_auth);
				if (ENDAT3_BG_Auth(base, usrlevel_auth, pass_auth, 1) == kStatus_Success) {
					PRINTF("\tBG Auth request successful \r\n");
				} else {
					PRINTF("\tBG Auth request failed \r\n");
				}
				break;
			}
			case ENDAT3_BG_OPCODE_SETPASS: {
				uint16_t usrlevel_set;
				uint16_t pass_set;
				getBGREQ_Auth_SetPass(&usrlevel_set, &pass_set);
				if (ENDAT3_BG_Setpass(base, usrlevel_set, pass_set, 1) == kStatus_Success) {
					PRINTF("\tBG Auth request successful \r\n");
				} else {
					PRINTF("\tBG Auth request failed \r\n");
				}
				break;
			}
			case ENDAT3_BG_OPCODE_PROTECT: {
				uint32_t address_pro;
				uint8_t mode;
				uint16_t acclevel;
				uint8_t al_write, al_read;
				char acclevelStr[4][16] = {"USER", "OEM2","OEM1", "MANUFACTURER"};
				getBGREQ_Protect(&address_pro, &mode, &acclevel);
				if (ENDAT3_BG_Protect(base, address_pro, mode, acclevel, &al_write, &al_read, 1) == kStatus_Success) {
					PRINTF("\tBG Protect request successful. Effective access level: al_write: %s al_read: %s.\r\n", acclevelStr[al_write], acclevelStr[al_read]);
				} else {
					PRINTF("\tBG Auth request failed \r\n");
				}
				break;
			}
			case ENDAT3_BG_OPCODE_LOCATE: {
				uint8_t ctrl;
				getBGREQ_Locate(&ctrl);
				if (ENDAT3_BG_Locate(base, ctrl, 1) == kStatus_Success) {
					PRINTF("\tBG Auth request successful \r\n");
				} else {
					PRINTF("\tBG Auth request failed \r\n");
				}
				break;
			}
			default:
				break;

		}
	}
}

void Endat3_BUS_BG_REQ(ENDAT3_Type *base, int8_t bus_addr)
{
	uint8_t req_code;

	while (1) {
		if (PrintBgReqSel(&req_code) < 0) {
			return;
		}

		switch (req_code) {
			case ENDAT3_BG_OPCODE_NOP: {
				uint64_t arbitrary_req = 0;
				uint64_t arbitrary_res = 0;
				getBGREQ_Nop(&arbitrary_req);
				ENDAT3_BG_Bus_P2P_Nop(base, bus_addr, arbitrary_req, &arbitrary_res, 1);
				if (arbitrary_res == ((0x01ULL << 40) | arbitrary_req)) {
					PRINTF("\tNOP request successful.\r\n");
				} else {
					PRINTF("\tNOP request failed.\r\n");
				}
				break;
			}
			case ENDAT3_BG_OPCODE_RECONFIGURE: {
				if (ENDAT3_BG_Bus_P2P_Reconfigure(base, bus_addr, 1) == kStatus_Success) {
					PRINTF("\tBG Reconfigure request successful \r\n");
				} else {
					PRINTF("\tBG Reconfigure request failed \r\n");
				}
				break;
			}
			case ENDAT3_BG_OPCODE_READ: {
				uint32_t address_read;
				uint8_t num_read;
				uint16_t words_read[3];
				getBGREQ_Read(&address_read, &num_read);
				if (ENDAT3_BG_Bus_P2P_Read(base, bus_addr, address_read, num_read, words_read, 1) == kStatus_Success) {
					for (int i = 0; i < num_read; i++) {
						PRINTF("\t word[%d]: 0x%x", i, words_read[i]);
					}
				} else {
					PRINTF("\tBG Read request failed\r\n");
				}
				break;
			}
			case ENDAT3_BG_OPCODE_WRITE: {
				uint32_t address_write;
				uint16_t word_write;
				getBGREQ_Write(&address_write, &word_write);
				if (ENDAT3_BG_Bus_P2P_Write(base, bus_addr, address_write, word_write, 1) == kStatus_Success) {
					PRINTF("\tBG write request successful \r\n");
				} else {
					PRINTF("\tBG write request failed \r\n");
				}
				break;
			}
			case ENDAT3_BG_OPCODE_AUTH: {
				uint16_t usrlevel_auth;
				uint16_t pass_auth;
				getBGREQ_Auth_SetPass(&usrlevel_auth, &pass_auth);
				if (ENDAT3_BG_Bus_P2P_Auth(base, bus_addr, usrlevel_auth, pass_auth, 1) == kStatus_Success) {
					PRINTF("\tBG Auth request successful \r\n");
				} else {
					PRINTF("\tBG Auth request failed \r\n");
				}
				break;
			}
			case ENDAT3_BG_OPCODE_SETPASS: {
				uint16_t usrlevel_set;
				uint16_t pass_set;
				getBGREQ_Auth_SetPass(&usrlevel_set, &pass_set);
				if (ENDAT3_BG_Bus_P2P_Setpass(base, bus_addr, usrlevel_set, pass_set, 1) == kStatus_Success) {
					PRINTF("\tBG Auth request successful \r\n");
				} else {
					PRINTF("\tBG Auth request failed \r\n");
				}
				break;
			}
			case ENDAT3_BG_OPCODE_PROTECT: {
				uint32_t address_pro;
				uint8_t mode;
				uint16_t acclevel;
				uint8_t al_write, al_read;
				char acclevelStr[4][16] = {"USER", "OEM2","OEM1", "MANUFACTURER"};
				getBGREQ_Protect(&address_pro, &mode, &acclevel);
				if (ENDAT3_BG_Bus_P2P_Protect(base, bus_addr, address_pro, mode, acclevel, &al_write, &al_read, 1) == kStatus_Success) {
					PRINTF("\tBG Protect request successful. Effective access level: al_write: %s al_read: %s.\r\n", acclevelStr[al_write], acclevelStr[al_read]);
				} else {
					PRINTF("\tBG Auth request failed \r\n");
				}
				break;
			}
			case ENDAT3_BG_OPCODE_LOCATE: {
				uint8_t ctrl;
				getBGREQ_Locate(&ctrl);
				if (ENDAT3_BG_Bus_P2P_Locate(base, bus_addr, ctrl, 1) == kStatus_Success) {
					PRINTF("\tBG Auth request successful \r\n");
				} else {
					PRINTF("\tBG Auth request failed \r\n");
				}
				break;
			}
			default:
				break;

		}
	}
}

void Endat3_LPF_Configuration(ENDAT3_Type *base, int8_t bus_addr, uint32_t mem_index)
{
	uint32_t mem_base_array[2] = {ENDAT3_MEM_BASE_LPFSET, ENDAT3_MEM_BASE_LPFLIVE};
	status_t status;
	uint16_t cache_buf[0x100] = {0};
	endat3_mem_cache_t mem_cache;

	if (bus_addr > 0) {
		if ((status = ENDAT3_Bus_P2P_memCacheInit(base, bus_addr, mem_base_array[mem_index], &mem_cache, cache_buf, 0x100, 1)) != kStatus_Success) {
			PRINTF("\tAddress%d : Failed to initialize memory cache.\r\n", bus_addr);
			return;
		}

		if ((status = ENDAT3_Bus_P2P_memCacheFetch(base, bus_addr, &mem_cache, 1)) != kStatus_Success) {
			PRINTF("\tAddress%d : Failed to fetch memory cache.\r\n", bus_addr);
			return;
		}
	} else {
		if ((status = ENDAT3_memCacheInit(base, mem_base_array[mem_index], &mem_cache, cache_buf, 0x100, 1)) != kStatus_Success) {
			PRINTF("\tEncoder : Failed to initialize memory cache.\r\n");
			return;
		}

		if ((status = ENDAT3_memCacheFetch(base, &mem_cache, 1)) != kStatus_Success) {
			PRINTF("\tEncoder : Failed to fetch memory cache.\r\n");
			return;
		}
	}

	if (ENDAT3_memCacheCheckCS(&mem_cache) != kStatus_Success) {
		PRINTF("\tAddress%d : CS check error\r\n", bus_addr);
		return;
	}

	uint8_t x_dim, y_dim, z_dim, x, y, z;
	uint16_t pointer;
	uint8_t fid;
	/*Dump current LPF configuration*/
	for (z = 0; z < 7; z++) {
		ENDAT3_lpfCacheGetXdimYdim(z + 1, &mem_cache, &x_dim, &y_dim);
		pointer = ENDAT3_lpfCacheGetPointer(z + 1, &mem_cache);
		if (x_dim == 0) {
			PRINTF("\tList%d: no LPFS will be output\r\n", z + 1);
		} else {
			PRINTF("\tList%d: pointer = 0x%x xdim = %d y_dim = %d\r\n", z + 1, pointer, x_dim, y_dim);
			for (y = 0; y < y_dim; y++) {
				PRINTF("\t\tFID( y = %d) : ", y);
				for (x = 0; x < x_dim; x++) {
					fid = ENDAT3_lpfCacheListGetFid(&mem_cache, x_dim, x, y, pointer);
					PRINTF("%s(x = %d),  ", ENDAT3_FID2str(fid), x);
				}
				PRINTF("\r\n");
			}
		}
	}

	z = 0;
	y = 0;
	x = 0;
	pointer = 0;
	PRINTF("\tStart to LPF configuration ? please input <y/n>: ");
	char ch;
	uint8_t lpf[1024];
	ch = getChar();
	if (ch == 'n' || ch == 'N')
		return;
	PRINTF("\r\n");
	while (1) {
		PRINTF("\tPlease input Z dimension: ");
		z_dim = getIntAndEcho();
		if (z_dim > 7) {
			PRINTF("\tDon't greater than 7 for Z dimension\r\n");
			continue;
		} else {
			break;
		}
	}
	PRINTF("\r\n");

	for (z =1; z <= z_dim; z++) {
		PRINTF("\tReconfiguration for z = %d : \r\n", z);

		while (1) {
			PRINTF("\t\tPlease input Y dimension: ");
			y_dim = getIntAndEcho();
			if (y_dim > 16) {
				PRINTF("\t\tDon't greater than 16 for Y dimension\r\n");
				continue;
			} else {
				break;
			}
		}
		PRINTF("\r\n");

		if (y_dim == 0) {
			x_dim = 0;
		} else {
			while (1) {
				PRINTF("\t\tPlease input X dimension: ");
				x_dim = getIntAndEcho();
				if (x_dim > 64) {
					PRINTF("\t\tDon't greater than 64 for X dimension\r\n");
					continue;
				} else {
					break;
				}
			}
			PRINTF("\r\n");
		}

		for (y = 0; y < y_dim; y++) {
			PRINTF("\t\tReconfiguration for y = %d config: \r\n", y);
			for (x = 0; x < x_dim; x++) {
				PRINTF("\t\tPlease select FID for x = %d: ", x);
				uint8_t fid;
				if (PrintFidSel(&fid) < 0) {
					return;
				}
				lpf[y * x_dim + x] = fid;
			}
		}
		PRINTF("z, x_dim,  y_dim,  pointer: %d %d %d %d\r\n", z, x_dim,  y_dim,  pointer);
		ENDAT3_lpfCacheListSetSendlist(&mem_cache, z, x_dim,  y_dim,  pointer, (uint8_t *)lpf);
		pointer += x_dim * y_dim;
	}

	PRINTF("\tDump unsaved LPF configuration\r\n");
	for (z = 0; z < 7; z++) {
		ENDAT3_lpfCacheGetXdimYdim(z + 1, &mem_cache, &x_dim, &y_dim);
		pointer = ENDAT3_lpfCacheGetPointer(z + 1, &mem_cache);
		if (x_dim == 0) {
			PRINTF("\tList%d: no LPFS will be output\r\n", z + 1);
		} else {
			PRINTF("\tList%d: pointer = 0x%x xdim = %d y_dim = %d\r\n", z + 1, pointer, x_dim, y_dim);
			for (y = 0; y < y_dim; y++) {
				PRINTF("\t\tFID( y = %d) : ", y);
				for (x = 0; x < x_dim; x++) {
					fid = ENDAT3_lpfCacheListGetFid(&mem_cache, x_dim, x, y, pointer);
					PRINTF("%s(x = %d),  ", ENDAT3_FID2str(fid), x);
				}
				PRINTF("\r\n");
			}
		}
	}

	PRINTF("\tSave and active modified LPF configuration? please input <y/n>: ");
	ch = getChar();
	if (ch == 'n' || ch == 'N')
		return;

	if (bus_addr > 0) {
		status = ENDAT3_Bus_P2P_lpfCacheFlushToEncoder(base, bus_addr, &mem_cache, 1);
	} else {
		status = ENDAT3_lpfCacheFlushToEncoder(base, &mem_cache, 1);
	}

	if (status != kStatus_Success) {
		PRINTF("\tFlush cache to FID Memory failed\r\n");
		return;
	}

	if (mem_index == 0) { /* LPFSET is activated by a RECONFIGURE command.*/
		/* Active the new LPF configuration */
		if (bus_addr > 0) {
			status = ENDAT3_BG_Bus_P2P_Reconfigure(base, bus_addr, 1);
		} else {
			status = ENDAT3_BG_Reconfigure(base, 1);
		}

		if (status != kStatus_Success) {
			PRINTF("\tReconfigure failed\r\n");
			return;
		}
	}

	PRINTF("\r\n\tActive the new LPF configuration\r\n");
	return;
}

void ENDAT3_dump_FID(ENDAT3_Type *base, int8_t bus_addr)
{
	uint8_t fid;
	struct FID fid_mem;

	while(1) {
		if (PrintFidSel(&fid) < 0)
			return;
		ENDAT3_FIDMEM_getLpf(base, bus_addr, fid, &fid_mem);
		ENDAT3_FIDMEM_Dump(&fid_mem);
	}
}

void ENDAT3_Safety_Packet(ENDAT3_Type *base, int8_t bus_addr)
{
	uint8_t collector, fid_sf, fid_sd2, fid_sd1, isHPF;
	char ch;
	status_t status;
	endat3_rsp_t rsp;
	while (1) {
		PRINTF("\tPlease select safety collector : 1, safety collector 1; 2, safety collector 2;\r\n");
		PRINTF("\tPlease input: ");
		collector = getIntAndEcho();
		if (collector < 1 || collector > 2) {
			PRINTF("\t\tInvalid input\r\n");
			continue;
		} else {
			collector--;
			break;
		}
	}

	PRINTF("\tPlease select FID_SF :\r\n");
	if (PrintFidSel(&fid_sf) < 0)
		return;
	
	PRINTF("\tPlease select FID_SD2 :\r\n");
	if (PrintFidSel(&fid_sd2) < 0)
		return;

	PRINTF("\tPlease select FID_SD1 :\r\n");
	if (PrintFidSel(&fid_sd1) < 0)
		return;

	PRINTF("\tIs FID_SD1 in the HPF? y/<n>: ");
	ch = getChar();
	isHPF = 1;
	if (ch == 'n' || ch == 'N') {
		isHPF = 0;
	}

	PRINTF("Endat3 Safety Collector %d verification\r\n", collector);
	if (collector == 0) {
		ENDAT3_Safety_Packet0_SF_Cfg(base, fid_sf);
		ENDAT3_Safety_Packet0_SD2_Cfg(base, fid_sd2);
		ENDAT3_Safety_Packet0_SD1_Cfg(base, fid_sd1, isHPF);
	} else {
		ENDAT3_Safety_Packet1_SF_Cfg(base, ENDAT3_FID_SF_POS1);
		ENDAT3_Safety_Packet1_SD2_Cfg(base, ENDAT3_FID_POS2);
		ENDAT3_Safety_Packet1_SD1_Cfg(base, ENDAT3_FID_POS1, isHPF);
	}

	PRINTF("\tPlease input send list: ");
	uint8_t fgReqCodestr[8][22] = {"DATA0     ", "DATA1     ", "DATA2     ", "DATA3      ", "DATA4        ",
									"DATA5     ", "DATA6     ", "DATA7     "};
	uint8_t fgReqCode[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
	int index;
	while (1) {
		int i;
		for (i = 0; i < 8; i++) {
			if (i % 5 == 0) {
				PRINTF("\r\n\t");
			}
			PRINTF("%2d: %s", i, fgReqCodestr[i]);
		}

		PRINTF("\r\n\tPlease input: ");
		index = getIntAndEcho();
		if (index > 8) {
			PRINTF("\r\n\tinvalid input\r\n");
			continue;
		} else {
			break;
		}
	}

	if (bus_addr > 0) {
		status = ENDAT3_FG_Bus_P2P_Req_Rsp(base, bus_addr, fgReqCode[index], 0, &rsp);
	} else {
		status = ENDAT3_FG_Req_Rsp(base, fgReqCode[index], 0, &rsp);
	}

	if (status == kStatus_Success) {
		ENDAT3_RspDump(&rsp);
		PRINTF("SC_STATUS_0: 0x%x %d\r\n", base->SC_STATUS[collector], ENDAT3_Safety_Packet_status(base, collector, bus_addr));
		ENDAT3_SAFETY_MEM_Dump(base, bus_addr, collector, isHPF);
	} else {
		PRINTF("Send list active failed\r\n");
	}
}

/*
 * @brief Main function
 */
int main(void)
{
	int ch;
	int memory_index;
	int running;
	endat3_base = BOARD_ENDAT3_BASEADDR;
	BOARD_InitHardware();
    PRINTF("Start Endat3 test tool\r\n\r\n");
	// Master Features
	PRINTF("Instantiation information of the EnDat 3 Master:\r\n");
	PRINTF("\tNumber of supported bus nodes: %d\r\n", (endat3_base->INSTANTIATION_INFO)
										& ENDAT3_INSTANTIATION_INFO_NUM_BUS_NODES_SUPP_MASK);
	PRINTF("\tFID based memory enabled: %d\r\n", (endat3_base->INSTANTIATION_INFO
										& ENDAT3_INSTANTIATION_INFO_FID_BASED_MEMORY_ENABLE_MASK)
										>> ENDAT3_INSTANTIATION_INFO_FID_BASED_MEMORY_ENABLE_SHIFT);
	PRINTF("\tOnly 12.5 MBit/s supported: %d\r\n", (endat3_base->INSTANTIATION_INFO
										& ENDAT3_INSTANTIATION_INFO_USE_12_5_MBIT_ONLY_MASK)
										>> ENDAT3_INSTANTIATION_INFO_USE_12_5_MBIT_ONLY_SHIFT);
	PRINTF("\tIRQ auto clear enabled: %d\r\n", (endat3_base->INSTANTIATION_INFO
										& ENDAT3_INSTANTIATION_INFO_IRQ_AUTO_CLEAR_ENABLE_MASK)
										>> ENDAT3_INSTANTIATION_INFO_IRQ_AUTO_CLEAR_ENABLE_SHIFT);
	PRINTF("\tDebug functionality enabled: %d\r\n", (endat3_base->INSTANTIATION_INFO
										& ENDAT3_INSTANTIATION_INFO_DEBUG_ENABLE_MASK)
										>> ENDAT3_INSTANTIATION_INFO_DEBUG_ENABLE_SHIFT);
	PRINTF("\tNumber of supported safety packets: %d\r\n", (endat3_base->INSTANTIATION_INFO
										& ENDAT3_INSTANTIATION_INFO_NUM_SAFETY_PACKETS_MASK)
										>> ENDAT3_INSTANTIATION_INFO_NUM_SAFETY_PACKETS_SHIFT);

	ENDAT3_RxTxClkConfig(endat3_base, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_12_5MBPS, 0);
	while(1) {
		running = 1;
		ch = PrintMainMenu();

		if (ch == 1) {
			ENDAT3_FG_Hello(endat3_base);
			nodes_num = 0;
			while (running) {
				ch = PrintMenuForP2P();
				switch (ch) {
					case 1:
						if (ENDAT3_FG_Hello(endat3_base) == kStatus_Success) {
							PRINTF("\tEncoder: active\r\n");
						} else {
							PRINTF("\tEncoder: inactive\r\n");
						}
						break;
					case 2:
						if (ENDAT3_FG_Rate(endat3_base, ENDAT3_RXTX_RATE_25MBPS) != kStatus_Success) {
							PRINTF("Failed to switch to 25Mbps\r\n");
							break;
						}
						ENDAT3_RxTxClkConfig(endat3_base, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_25MBPS,  0);
						SDK_DelayAtLeastUs(ENDAT3_RATE_TIMEOUT * 1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

						if (ENDAT3_FG_Hello(endat3_base) == kStatus_Success) {
							PRINTF("Switch to 25Mbps\r\n");
							clk_rxtx = ENDAT3_RXTX_RATE_25MBPS;
						} else {
							ENDAT3_RxTxClkConfig(endat3_base, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_12_5MBPS, 0);
							PRINTF("Failed to switch to 25Mbps\r\n");
						}
						break;
					case 3:
						if (ENDAT3_FG_Rate(endat3_base, ENDAT3_RXTX_RATE_12_5MBPS) != kStatus_Success) {
							PRINTF("Failed to switch to 12.5Mbps\r\n");
							break;
						}
						ENDAT3_RxTxClkConfig(endat3_base, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_12_5MBPS,  0);
						SDK_DelayAtLeastUs(ENDAT3_RATE_TIMEOUT * 1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

						if (ENDAT3_FG_Hello(endat3_base) == kStatus_Success) {
							PRINTF("Switch to 12.5Mbps\r\n");
							clk_rxtx = ENDAT3_RXTX_RATE_12_5MBPS;
						} else {
							ENDAT3_RxTxClkConfig(endat3_base, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_25MBPS, 0);
							PRINTF("Failed to switch to 12.5Mbps\r\n");
						}
						break;
					case 4:
						ENDAT3_DumpParticipantInfomation(endat3_base, 0);
						break;
					case 5:
						memory_index = PrintMemorySel();
						if (memory_index < 0) {
							PRINTF("invalid input: %d\r\n", memory_index);
						} else {
							ENDAT3_ModifyMemory(endat3_base, 0, memory_index);
						}
						break;
					case 6:
						Endat3_FG_REQ(endat3_base);
						break;
					case 7:
						Endat3_BG_REQ(endat3_base);
						break;
					case 8:
						memory_index = PrintLPFMemorySel();
						if (memory_index < 0) {
							PRINTF("\tinvalid input: %d\r\n", memory_index);
						} else {
							Endat3_LPF_Configuration(endat3_base, 0, memory_index);
						}
						break;
					case 9:
						ENDAT3_dump_FID(endat3_base, 0);
						break;
					case 10:
						ENDAT3_Safety_Packet(endat3_base, 0);
						break;
					case 11:
						ch = PrintSendlistSel();
						ENDAT3_DumpPostionInIrq(endat3_base, (uint8_t)ch);
						break;
					case 12:
						ch = PrintSendlistSel();
						ENDAT3_DumpPostionInSync(endat3_base, (uint8_t)ch);
							break;
					case 13:
						running = 0;
						break;
				}
			}
		}  else if (ch == 3) {
			ENDAT3_RxTxClkConfig(endat3_base, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_25MBPS, 0);
			clk_rxtx = ENDAT3_RXTX_RATE_25MBPS;
		}  else if (ch == 4) {
			ENDAT3_RxTxClkConfig(endat3_base, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_12_5MBPS, 0);
			clk_rxtx = ENDAT3_RXTX_RATE_12_5MBPS;
		} else if (ch == 2) {
			nodes_num = 2;
			ENDAT3_Set_Bus_Participants_Num(endat3_base, nodes_num);
			ENDAT3_Bus_Hello_ForAllParticipants(endat3_base, nodes_num);
			while (running) {
				ch = PrintMenuForBus();
				switch (ch) {
					case 1:
						PRINTF("\tPlease intput the number of the participants:");
						ch = getIntAndEcho();
						nodes_num = ch;
						ENDAT3_Set_Bus_Participants_Num(endat3_base, nodes_num);
						ENDAT3_Bus_Hello_ForAllParticipants(endat3_base, nodes_num);
						break;
					case 2:
						ENDAT3_ClearAddressForAllParticipants(endat3_base, nodes_num);
						break;
					case 3:
						ENDAT3_AssignAddressForAllParticipants(endat3_base, nodes_num);
						break;
					case 4:
						ENDAT3_ScanAddressForAllParticipants(endat3_base, nodes_num);
						break;
					case 5:
						ENDAT3_FG_Bus_Rate_Switch(endat3_base, nodes_num, ENDAT3_RXTX_RATE_25MBPS);
						ENDAT3_RxTxClkConfig(endat3_base, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_25MBPS,  0);
						SDK_DelayAtLeastUs(ENDAT3_RATE_TIMEOUT * 1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

						if (ENDAT3_Bus_Hello_ForAllParticipants(endat3_base, nodes_num) == kStatus_Success) {
							PRINTF("Switch to 25Mbps\r\n");
							clk_rxtx = ENDAT3_RXTX_RATE_25MBPS;
						} else {
							ENDAT3_RxTxClkConfig(endat3_base, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_12_5MBPS, 0);
							PRINTF("Failed to switch to 25Mbps\r\n");
						}
						break;
					case 6:
						ENDAT3_FG_Bus_Rate_Switch(endat3_base, nodes_num, ENDAT3_RXTX_RATE_12_5MBPS);
						SDK_DelayAtLeastUs(ENDAT3_RATE_TIMEOUT * 1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
						ENDAT3_RxTxClkConfig(endat3_base, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_12_5MBPS,  0);

						if (ENDAT3_Bus_Hello_ForAllParticipants(endat3_base, nodes_num) == kStatus_Success) {
							PRINTF("Switch to 12.5Mbps\r\n");
							clk_rxtx = ENDAT3_RXTX_RATE_12_5MBPS;
						} else {
							ENDAT3_RxTxClkConfig(endat3_base, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_25MBPS,  0);
							PRINTF("Failed to switch to 12.5Mbps\r\n");
						}
						break;
					case 7:
						ch = PrintParticipantSel();
						if (ch > 0) {
							ENDAT3_DumpParticipantInfomation(endat3_base, (uint8_t)ch);
						}
						break;
					case 8:
						ch = PrintParticipantSel();
						if (ch < 0) {
							break;
						}
						memory_index = PrintMemorySel();
						if (memory_index < 0) {
							PRINTF("invalid input: %d\r\n", memory_index);
						} else {
							ENDAT3_ModifyMemory(endat3_base, (uint8_t)ch, memory_index);
						}
						break;
					case 9:
						ch = PrintParticipantSel();
						if (ch < 0) {
							break;
						}
						Endat3_BUS_FG_REQ(endat3_base, (uint8_t)ch);
						break;
					case 10:
						ch = PrintParticipantSel();
						if (ch < 0) {
							break;
						}
						Endat3_BUS_BG_REQ(endat3_base, (uint8_t)ch);
						break;
					case 11:
						ch = PrintParticipantSel();
						if (ch < 0) {
							break;
						}
						memory_index = PrintLPFMemorySel();
						if (memory_index < 0) {
							PRINTF("\tinvalid input: %d\r\n", memory_index);
						} else {
							Endat3_LPF_Configuration(endat3_base, (uint8_t)ch, memory_index);
						}
						break;
					case 12:
						ch = PrintParticipantSel();
						if (ch < 0) {
							break;
						}
						ENDAT3_dump_FID(endat3_base, (uint8_t)ch);
						break;
					case 13:
						ch = PrintParticipantSel();
						if (ch < 0) {
							break;
						}
						ENDAT3_Safety_Packet(endat3_base, (uint8_t)ch);
						break;
					case 16:
						ch = PrintSendlistSel();
						ENDAT3_Bus_DumpPostionInIrq(endat3_base, nodes_num, (uint8_t)ch);
						break;
					case 17:
						ch = PrintSendlistSel();
						ENDAT3_Bus_DumpPostionInSync(endat3_base, (uint8_t)ch);
						break;
					case 18:
						running = 0;
						break;
				}
			}
		}
	}
}
