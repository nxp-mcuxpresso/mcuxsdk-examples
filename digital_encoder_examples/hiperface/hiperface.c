/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_hiperface.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define MAX_INPUT_LEN     17

/* Main menu choice definitions */
#define MENU_CHOICE_MASTER_INFO              1
#define MENU_CHOICE_REG_ACCESS_TEST          2
#define MENU_CHOICE_INQUIRY_RDB_INFO         3
#define MENU_CHOICE_ACCESS_RESOURCE          4
#define MENU_CHOICE_REMOTE_SLAVE_REG_ACCESS  5
#define MENU_CHOICE_SYNC_MODE_TEST           6

/* RDB resource access command index definitions */
#define RDB_CMD_GET_ENCODER_TYPE        1
#define RDB_CMD_GET_RESOLUTION          2
#define RDB_CMD_GET_MEASUREMENT_RANGE   3
#define RDB_CMD_GET_ENCODER_TYPE_NAME   4
#define RDB_CMD_GET_SERIAL_NUMBER       5
#define RDB_CMD_GET_DEVICE_VERSION      6
#define RDB_CMD_GET_FIRMWARE_DATE       7
#define RDB_CMD_GET_EEPROM_SIZE         8
#define RDB_CMD_GET_SAFE_CH2_RESOLUTION 9
#define RDB_CMD_GET_TEMPERATURE_RANGE   11
#define RDB_CMD_GET_TEMPERATURE         12
#define RDB_CMD_GET_SUPPLY_VOLTAGE_RANGE 13
#define RDB_CMD_GET_SUPPLY_VOLTAGE      14
#define RDB_CMD_GET_ROTATION_SPEED_RANGE 15
#define RDB_CMD_GET_ROTATION_SPEED      16
#define RDB_CMD_GET_ACCELERATION_RANGE  17
#define RDB_CMD_GET_LIFETIME            19
#define RDB_CMD_GET_LIFETIME_REMAINING  20
#define RDB_CMD_GET_ERROR_LOG_NUMBER    21
#define RDB_CMD_GET_ERROR_LOG           22
#define RDB_CMD_GET_ERROR_LOG_FILTER    23
#define RDB_CMD_SET_ERROR_LOG_FILTER    24
#define RDB_CMD_SET_RESET               25
#define RDB_CMD_SET_SHUT_DOWN           26
#define RDB_CMD_GET_SET_POSITION        27
#define RDB_CMD_SET_SET_POSITION        28
#define RDB_CMD_GET_CURRENT_ACCESS_LVL  29
#define RDB_CMD_SET_ACCESS_LEVEL        30
#define RDB_CMD_CHANGE_ACCESS_KEY       31
#define RDB_CMD_READ_COUNTER            32
#define RDB_CMD_SET_INCREMENT_COUNTER   33
#define RDB_CMD_RESET_COUNTER           34
#define RDB_CMD_FILE_OPERATION          35
#define RDB_CMD_SIMPLE_IO_OPERATION     37

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void DSL_RDB_DumpNodeDefiningValue(dsl_rdb_node_t *node, int level, int iteration);
 int getValueAndEcho();

/*******************************************************************************
 * Variables
 ******************************************************************************/
dsl_encoder_t enc;
uint8_t event_mask_h = 0, event_mask_l = 0;
int syncPosRecvNum;

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

static char getChar()
{
	int ret = GETCHAR();
	return (char)(ret & 0xFF);
}

int64_t getV64ValueAndEcho()
{
	char str[MAX_INPUT_LEN] = {0};
	int index = 0;
	int64_t v64 = 0;
	char ch;
	int ret = 0;

	while (index < MAX_INPUT_LEN - 2) {
		ch = getChar();
		PRINTF("%c", ch);
		if(ch == '\r' || ch == '\n')
			break;
		str[index++] = ch;
	};

	if (index > 0) {
		str[MAX_INPUT_LEN - 1] = '\0';
		if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
			ret = sscanf(str, "%llx", &v64);
		} else if (str[0] > '0' && str[0] <= '9') {
			ret = sscanf(str, "%lld", &v64);
		} else {
			ret = sscanf(str, "%c", &ch);
			v64 = (int)ch;
		}
	}
	PRINTF("\r\n");
	if (ret > 0)
		return v64;
	return ret;
}

int getValueAndEcho()
{
	char str[MAX_INPUT_LEN] = {0};
	int index = 0;
	int v32 = 0;
	char ch;
	int ret = 0;

	while (index < MAX_INPUT_LEN - 2) {
		ch = getChar();
		PRINTF("%c", ch);
		if(ch == '\r' || ch == '\n')
			break;
		str[index++] = ch;
	};

	if (index > 0) {
		str[16] = '\0';
		if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
			ret = sscanf(str, "%x", &v32);
		} else if (str[0] >= '0' && str[0] <= '9') {
			ret = sscanf(str, "%d", &v32);
		} else {
			ret = sscanf(str, "%c", &ch);
			v32 = (int)ch;
		}
	}
	PRINTF("\r\n");
	if (ret > 0)
		return v32;
	return ret;
}

void DEMO_XBARA_SYNC_POS_RCVD_IRQHandler(void)
{
	uint64_t pos = DSL_GetFastPosition(BOARD_HIPERFACE_BASEADDR, &enc);
	if (!DSL_GetEventEstimatorThresholdErr(BOARD_HIPERFACE_BASEADDR)) {
		/*Only output low 32bits position data due to PRINTF.*/
		PRINTF("Pos_sync: %c%ld\r\n", (pos >> 63) == 1 ? '-' : ' ',  (uint32_t)(pos & 0xFFFFFFFF));
	} else {
		PRINTF("Estimator Deviation Threshold Error\r\n");
		DSL_ClrEventEstimatorThresholdErr(BOARD_HIPERFACE_BASEADDR);
	}
	hiperface_clear_xbara_sync_pos_recv_irq_status();
	syncPosRecvNum--;
	if (syncPosRecvNum <= 0) {
		DisableIRQ(DEMO_XBARA_SYNC_POS_RCVD_IRQn);
	}

}

void DEMO_HIPERFACE_POS_RCVD_IRQHandler(void)
{
	uint64_t pos = DSL_GetFastPosition(BOARD_HIPERFACE_BASEADDR, &enc);
	if (!DSL_GetEventEstimatorThresholdErr(BOARD_HIPERFACE_BASEADDR)) {
		/*Only output low 32bits position data due to PRINTF.*/
		PRINTF("Pos_irq: %c%ld\r\n", (pos >> 63) == 1 ? '-' : ' ',  (uint32_t)(pos & 0xFFFFFFFF));
	} else {
		PRINTF("Estimator Deviation Threshold Error\r\n");
		DSL_ClrEventEstimatorThresholdErr(BOARD_HIPERFACE_BASEADDR);
	}
	hiperface_clear_fast_pos_irq_status();
}

void DEMO_HIPERFACE_S_IRQHandler(void)
{
		PRINTF("HIPERFACE_S_IRQHandler: 0x%x\r\n", BOARD_HIPERFACE_BASEADDR->EVENT_S);
		BOARD_HIPERFACE_BASEADDR->MASK_S = 0x0;
		BOARD_HIPERFACE_BASEADDR->EVENT_S = 0;
}

void DEMO_HIPERFACE_IRQHandler(void)
{
	HIPERFACE_Type *base = BOARD_HIPERFACE_BASEADDR;
	if (DSL_GetEventSlaveEventSum(base) && DSL_GetEventMaskMSUM(event_mask_h)) {
			PRINTF("The DSL Slave has signaled an event and the summary mask is set accordingly.\r\n");
			DSL_ClrEventSlaveEventSum(base);
	}

	if (DSL_GetEventeEstimatorOn(base) && DSL_GetEventMaskMPOS(event_mask_h)) {
			PRINTF("Fast position data consistency error. The fast position read through drive interface is supplied by the estimator.\r\n");
			DSL_ClrEventeEstimatorOn(base);
	}

	if (DSL_GetEventEstimatorThresholdErr(base) && DSL_GetEventMaskMDTE(event_mask_h)) {
			PRINTF("Current value of deviation greater than the specified maximum.\r\n");
			DSL_ClrEventEstimatorThresholdErr(base);
	}

	if (DSL_GetEventProtocolRstWarning(base) && DSL_GetEventMaskMPRST(event_mask_h)) {
			PRINTF("The forced protocol reset was triggered.\r\n");
			DSL_ClrEventProtocolRstWarning(base);
	}

	if (DSl_GetEventMsgInitStatus(base) && DSL_GetEventMaskMMIN(event_mask_l)) {
			PRINTF("An acknowledgment was received from the Slave for the initialization of a message.\r\n");
			DSl_ClrEventMsgInitStatus(base);
	}

	if (DSl_GetEventLongMsgAnswerErr(base) && DSL_GetEventMaskMANS(event_mask_l)) {
			PRINTF("An error occurred during the answer to a long message. The effectiveness of the previous transaction is not known.\r\n");
			DSl_ClrEventLongMsgAnswerErr(base);
	}

	if (DSl_GetEventQMLowValueWarning(base) && DSL_GetEventMaskMQMLW(event_mask_l)) {
			PRINTF("Quality monitoring value below \"14\".\r\n");
			DSl_ClrEventQMLowValueWarning(base);
	}

	if (DSl_GetEventLongMsgChannelfree(base) && DSL_GetEventMaskMFREL(event_mask_l)) {
			PRINTF("\"long message\" can be sent on the Parameters Channel.\r\n");
	}
}

void DSL_RDB_DumpNodes(dsl_rdb_node_t *node, int level, int iteration)
{
	int i;
	for (i = 0; i < level * 4; i++)
		PRINTF(" ");
	PRINTF("RID: 0x%03x, Resource Name: %10s, Data Type: %s\r\n", node->rid,
		   node->resourceName, DSL_RDB_DataTypeToStr(node->dataType));

	if (!iteration)
		return;
	iteration--;
	if (node->dataType == RDB_DATA_TYPE_NODE_INDICATOR) {
		level++;
		for (i = 0; i < node->childrenNum; i++) {
			DSL_RDB_DumpNodes(&node->nodes[i], level, iteration);
		}
	}
}

void DSL_RDB_DumpNodeViaTree(dsl_rdb_node_t *node, int level, int iteration)
{
	int i;
	for (i = 0; i < level * 4; i++)
		PRINTF(" ");
	PRINTF("RID: 0x%x\r\n", node->rid);

	for (i = 0; i < level * 4; i++)
		PRINTF(" ");
	PRINTF(" |_ Resource Name: %s\r\n", node->resourceName);

	for (i = 0; i < level * 4; i++)
		PRINTF(" ");
	PRINTF(" |_ Access:\r\n");

	for (i = 0; i < level * 4; i++)
		PRINTF(" ");
	PRINTF(" |   |_ Read: %s\r\n", DSL_RDB_AccessLevelToStr(node->readAccessLevel));

	for (i = 0; i < level * 4; i++)
		PRINTF(" ");
	PRINTF(" |   |_ Write: %s\r\n", DSL_RDB_AccessLevelToStr(node->writeAccessLevel));

	for (i = 0; i < level * 4; i++)
		PRINTF(" ");
	PRINTF(" |_ Time overrun: %d\r\n", node->timeOverrun > 254 ? 255 : node->timeOverrun);

	for (i = 0; i < level * 4; i++)
		PRINTF(" ");
	PRINTF(" |_ Data type: %s\r\n", DSL_RDB_DataTypeToStr(node->dataType));
	if (!iteration)
		return;
	iteration--;
	if (node->dataType == RDB_DATA_TYPE_NODE_INDICATOR) {
		level++;
		for (i = 0; i < node->childrenNum; i++) {
			DSL_RDB_DumpNodeViaTree(&node->nodes[i], level, iteration);
		}
	}
}
static int PrintMainMenu(void)
{
	PRINTF("|--------------------------------------------------------------------------|\r\n");
	PRINTF("|   Link Status: %s, Quality Monitoring: %d                                |\r\n",
		   DSL_GetQualityMonitoringLink(BOARD_HIPERFACE_BASEADDR) ? "Yes" : "No",
           DSL_GetQualityMonitoringValue(BOARD_HIPERFACE_BASEADDR));
	PRINTF("|--------------------------------------------------------------------------|\r\n");
	PRINTF("|   1: Display the release version of DSL Master                           |\r\n");
	PRINTF("|   2: DSL Master Register access test                                     |\r\n");
	PRINTF("|   3: Inqury Motor feedback system resource infomation                    |\r\n");
	PRINTF("|   4: Access Motor feedback system resources                              |\r\n");
	PRINTF("|   5: Access Remote Slave Registers                                       |\r\n");
	PRINTF("|   6: Run synchronous mode test (100 cycles)                              |\r\n");
	PRINTF("|--------------------------------------------------------------------------|\r\n");
	PRINTF("Please input: ");
	return getValueAndEcho();
}

static void DumpMasterInfo(HIPERFACE_Type *base)
{
	dsl_encoder_version_info_t info;
	DSL_GetMasterReleaseInfo(base, &info);
	PRINTF("Type of IP Core: %d\r\n", info.coding);
	PRINTF("IP Core Major release number: %d\r\n", info.majorNumber);
	PRINTF("IP Core Minor release number: %d\r\n", info.minorNumber);
	PRINTF("IP Core Release date: %d-%d-%d\r\n", info.year, info.month, info.day);
}

static void RegisterAccessPerformanceTest(HIPERFACE_Type *base)
{
	uint8_t value;
	uint64_t time;
	uint32_t count;
	PRINTF("Register access performance test with 32-bits:\r\n");
	BOARD_InitSysTick();
	volatile uint32_t *reg = (uint32_t *)&base->PRIM[0];
	value = *reg;
	SYSTICK_START_COUNT();
	for (int i =0 ; i< 1000; i++) {
		*reg = value;
	}
	count = SYSTICK_GET_COUNT();
	time = (uint64_t) count * 1000000000/1000 / SystemCoreClock; /* ns */
	PRINTF("    Writing time:%dns\r\n", (uint32_t)time);
	SYSTICK_START_COUNT();
	for (int i =0 ; i< 1000; i++) {
		value = *reg;
	}
	count = SYSTICK_GET_COUNT();
	time = (uint64_t) count * 1000000000/1000 / SystemCoreClock; /* ns */
	PRINTF("    Reading time:%dns\r\n", (uint32_t)time);
}

static void DumpRDB_Infomation(HIPERFACE_Type *base, dsl_encoder_t *enc)
{
	dsl_rdb_node_t *node;
	int choice;
	/* Cache all RDB infomation */
	DSL_RDB_ReadAllNodeDefiningValue(base, enc);
	PRINTF("All available resources on the Encoder:\r\n");
	DSL_RDB_DumpNodes(&enc->rootNode, 0, 0xFF);
	while (1) {
		PRINTF("Please input RID: ('b' to back to main menu'): ");
		choice = getValueAndEcho();
		if (choice == 'b' || choice == 'B') {
			break;
		}
		node = DSL_RDB_FindNodeFromCache(&enc->rootNode, choice);
		if (node) {
			DSL_RDB_DumpNodeViaTree(node, 0, 1);
		} else {
			PRINTF("Invail RID: 0x%x\r\n", choice);
		}
	}
	DSL_RDB_FreeAllNodeDefiningValue(BOARD_HIPERFACE_BASEADDR, enc);
}

static void PrintNodeRequestMenu(void)
{
	PRINTF("|-------------------------------------------------------------------------------|\r\n");
	PRINTF("|   1: Get Encoder Type                          2: Get Encoder Resolution      |\r\n");
	PRINTF("|   3: Get Measurement Range                     4: Get Encoder Type Name       |\r\n");
	PRINTF("|   5: Get Serial Number                         6: Get Device version          |\r\n");
	PRINTF("|   7: Get Firmware date                         8: Get EEPROM Size             |\r\n");
	PRINTF("|   9: Get Sate Channel2 Resolution                                             |\r\n");
	PRINTF("|  11: Get Temperature Range                    12: Get Temperature             |\r\n");
	PRINTF("|  13: Get Supply Voltage Range                 14: Get Supply Voltage          |\r\n");
	PRINTF("|  15: Get Rotation Speed Range                 16: Get Rotation Speed          |\r\n");
	PRINTF("|  17: Get Acceleration Range                                                   |\r\n");
	PRINTF("|  19: Get Lifetime and Shaft Rotations Number  20: Get Lifetime Remaining      |\r\n");
	PRINTF("|  21: Get Error Log Number                     22: Get Error Log               |\r\n");
	PRINTF("|  23: Get Error Log Filter                     24: Set Error Log Filter        |\r\n");
	PRINTF("|  25: Set Reset                                26: Set Shut Down               |\r\n");
	PRINTF("|  27: Get SetPosition                          28: Set SetPosition             |\r\n");
	PRINTF("|  29: Get Current Access Level                 30: Set Access Level            |\r\n");
	PRINTF("|  31: Change Access Key                        32: Read Counter                |\r\n");
	PRINTF("|  33: Set Increment Counter                    34: Reset Counter               |\r\n");
	PRINTF("|  35: File Operation                                                           |\r\n");
	PRINTF("|  37: Simple IO  Operation                                                     |\r\n");
	PRINTF("|-------------------------------------------------------------------------------|\r\n");
}

static void PrintFileOperationMenu(void)
{
	PRINTF("|-------------------------------------------------------------------|\r\n");
	PRINTF("|  1: Load File                       2: Read File                  |\r\n");
	PRINTF("|  3: Write File                      4: Get File Status            |\r\n");
	PRINTF("|  5: Create File                     6: Change File                |\r\n");
	PRINTF("|  7: Delete File                                                   |\r\n");
	PRINTF("|  9: Get Directory Basic Data       10: Get Directory File Name    |\r\n");
	PRINTF("|-------------------------------------------------------------------|\r\n");
}

#define RDB_ACCESS_BUFF_LEN 64
static void RDB_ResourceAccess(HIPERFACE_Type *base, dsl_encoder_t *enc)
{
	status_t status;
	int choice, choice_1;
	uint32_t v32u_0, v32u_1;
	uint16_t v16u_0, v16u_1;
	uint8_t v8u_0;
	uint64_t v64u_0;
	float vf0, vf1;
	dsl_rdb_error_protocol_t errlog;
	uint8_t buff[RDB_ACCESS_BUFF_LEN];
	
	while (1) {
		PrintNodeRequestMenu();
		PRINTF("Please input command index: ('b' to back to main menu'): ");
		choice = getValueAndEcho();
		if (choice == 'b' || choice == 'B') {
			break;
		}
		status = kStatus_Fail;
		switch (choice) {
			case RDB_CMD_GET_ENCODER_TYPE:
				if ((status = DSL_RDB_GetTypeOfEncoder(BOARD_HIPERFACE_BASEADDR, &v16u_0)) == kStatus_Success)
					PRINTF("    Encoder Type: %s\r\n", DSL_RDB_TypeOfEncoderTostr(v16u_0));
				break;
			case RDB_CMD_GET_RESOLUTION:
				if ((status = DSL_RDB_GetResolution(BOARD_HIPERFACE_BASEADDR, &v32u_0)) == kStatus_Success)
					PRINTF("    Encoder Resolution: %u\r\n", v32u_0);
				break;
			case RDB_CMD_GET_MEASUREMENT_RANGE:
				if ((status = DSL_RDB_GetMeasurementRange(BOARD_HIPERFACE_BASEADDR, &v32u_0)) == kStatus_Success)
					PRINTF("    Measurement Range: %u\r\n", v32u_0);
				break;
			case RDB_CMD_GET_ENCODER_TYPE_NAME:
				if ((status = DSL_RDB_GetTypeNameOfEncoder(BOARD_HIPERFACE_BASEADDR, buff, RDB_ACCESS_BUFF_LEN)) == kStatus_Success)
					PRINTF("    Encoder Type Name: %s\r\n", buff);
				break;
			case RDB_CMD_GET_SERIAL_NUMBER:
				if ((status = DSL_RDB_GetSerialNumber(BOARD_HIPERFACE_BASEADDR, buff, RDB_ACCESS_BUFF_LEN)) == kStatus_Success)
					PRINTF("    Serial Number : %s\r\n", buff);
				break;
			case RDB_CMD_GET_DEVICE_VERSION:
				if ((status = DSL_RDB_GetBaseiceVersion(BOARD_HIPERFACE_BASEADDR, buff, 17, &buff[17], 5)) == kStatus_Success)
					PRINTF("    Firmware version : %s\r\n    Hardware version : %s\r\n", buff, &buff[17]);
				break;
			case RDB_CMD_GET_FIRMWARE_DATE:
				if ((status = DSL_RDB_GetFirmwareDate(BOARD_HIPERFACE_BASEADDR,  buff, RDB_ACCESS_BUFF_LEN)) == kStatus_Success)
					PRINTF("    Firmware Date : %s\r\n", &buff[0]);
				break;
			case RDB_CMD_GET_EEPROM_SIZE:
				if ((status = DSL_RDB_GetEEPROMSize(BOARD_HIPERFACE_BASEADDR, &v16u_0)) == kStatus_Success)
					PRINTF("    EEPROM Size: %u\r\n", v16u_0);
				break;
			case RDB_CMD_GET_SAFE_CH2_RESOLUTION:
				if ((status = DSL_RDB_GetSafeChannel2Resolution(BOARD_HIPERFACE_BASEADDR, &v32u_0)) == kStatus_Success)
					PRINTF("    Safe Channel2 Resolution: %u\r\n", v32u_0);
				break;
			case RDB_CMD_GET_TEMPERATURE_RANGE:
				if ((status = DSL_RDB_GetTemperatureRange(BOARD_HIPERFACE_BASEADDR, &vf0, &vf1)) == kStatus_Success)
					PRINTF("    Temperature Range: %d.%d  - %d.%d\r\n", (int)vf1, vf1 < 0 ? ((int)vf1) * 10 - (int)(vf1 * 10) : (int)(vf1 * 10) -  ((int)vf1) * 10,
						  (int)vf0, (int)(vf0 * 10) -  ((int)vf0) * 10);
				break;
			case RDB_CMD_GET_TEMPERATURE:
				if ((status = DSL_RDB_GetTemperature(BOARD_HIPERFACE_BASEADDR, &vf0)) == kStatus_Success)
					PRINTF("    Current Temperature: %d.%d \r\n", (int)vf0, vf0 < 0 ? ((int)vf0) * 10 - (int)(vf0 * 10) : (int)(vf0 * 10) -  ((int)vf0) * 10);
				break;
			case RDB_CMD_GET_SUPPLY_VOLTAGE_RANGE:
				if ((status = DSL_RDB_GetSupplyVoltageRange(BOARD_HIPERFACE_BASEADDR, &v16u_0, &v16u_1)) == kStatus_Success)
					PRINTF("    Supply Voltag Range: %umv  - %umv\r\n", v16u_1, v16u_0);
				break;
			case RDB_CMD_GET_SUPPLY_VOLTAGE:
				if ((status = DSL_RDB_GetSupplyVoltage(BOARD_HIPERFACE_BASEADDR, &v16u_0)) == kStatus_Success)
					PRINTF("    Current Supply Voltage: %umv\r\n", v16u_0);
				break;
			case RDB_CMD_GET_ROTATION_SPEED_RANGE:
				if ((status = DSL_RDB_GetRotationSpeedRange(BOARD_HIPERFACE_BASEADDR, &v16u_0)) == kStatus_Success)
					PRINTF("    Rotation Speed Range: %uRPM (Rotation per minute)\r\n", v16u_0);
				break;
			case RDB_CMD_GET_ROTATION_SPEED:
				if ((status = DSL_RDB_GetRotationSpeed(BOARD_HIPERFACE_BASEADDR, &v16u_0)) == kStatus_Success)
					PRINTF("   Rotation Speed: %uRPM (Rotation per minute)\r\n", v16u_0);
				break;
			case RDB_CMD_GET_ACCELERATION_RANGE:
				if ((status = DSL_RDB_GetAccelerationRange(BOARD_HIPERFACE_BASEADDR, &v16u_0)) == kStatus_Success)
					PRINTF("    Acceleration Range: %urad/s^2\r\n", v16u_0);
				break;
			case RDB_CMD_GET_LIFETIME:
				if ((status = DSL_RDB_GetLifetime(BOARD_HIPERFACE_BASEADDR, &v32u_0, &v32u_1)) == kStatus_Success)
					PRINTF("    Operating Time: %umin, Shaft Rotations Number: %u\r\n", v32u_0, v32u_1);
				break;
			case RDB_CMD_GET_LIFETIME_REMAINING:
				if ((status = DSL_RDB_GetLifetimeRemaining(BOARD_HIPERFACE_BASEADDR, &v32u_0)) == kStatus_Success)
					PRINTF("    Remaining task lifetime in minutes: %umin\r\n", v32u_0);
				break;
			case RDB_CMD_GET_ERROR_LOG_NUMBER:
				if ((status = DSL_RDB_GetErrorLogNumber(BOARD_HIPERFACE_BASEADDR, &v32u_0)) == kStatus_Success)
					PRINTF("    Error Log Number: %u\r\n", v32u_0);
				break;
			case RDB_CMD_GET_ERROR_LOG:
				PRINTF("Please input log index: ");
				choice_1 = getValueAndEcho();
				if ((status = DSL_RDB_GetErrorLog(BOARD_HIPERFACE_BASEADDR, choice_1, &errlog)) == kStatus_Success) {
					PRINTF("    TimeStamp: %u\r\n", errlog.timeStamp);
					PRINTF("    Temperature(0.1 °C): %u\r\n", errlog.temperature);
					PRINTF("    Technology Specific: %u\r\n", errlog.technologySpecific);
					PRINTF("    Internal Supply Voltage: %umV\r\n", errlog.internalSupplyVoltage);
					PRINTF("    Rotation Speed: %u\r\n", errlog.rotationSpeed);
					PRINTF("    Additional Error Code: %u\r\n", errlog.additionalErrorCode);
					PRINTF("    Error Code: %u\r\n", errlog.errorCode);
				}
				break;
			case RDB_CMD_GET_ERROR_LOG_FILTER:
				if ((status = DSL_RDB_GetErrorLogFilter(BOARD_HIPERFACE_BASEADDR, &v8u_0)) == kStatus_Success)
					PRINTF("    Error Log Filter: %s\r\n", v8u_0 == ERROR_LOG_FILTER_ON ? "On" : "Off");
				break;
			case RDB_CMD_SET_ERROR_LOG_FILTER:
				PRINTF("Please input 1 to ON or 2 to OFF: ");
				choice_1 = getValueAndEcho();
				if (choice_1 == 1) {
					v8u_0 = ERROR_LOG_FILTER_ON;
				} else {
					v8u_0 = ERROR_LOG_FILTER_OFF;
				}
				if ((status = DSL_RDB_SetErrorLogFilter(BOARD_HIPERFACE_BASEADDR, v8u_0)) == kStatus_Success)
					PRINTF("    Success.\r\n");
				break;
			case RDB_CMD_SET_RESET:
				if ((status = DSL_RDB_SetReset(BOARD_HIPERFACE_BASEADDR)) == kStatus_Success)
					PRINTF("    Success.\r\n");
				break;
			case RDB_CMD_SET_SHUT_DOWN:
				if ((status = DSL_RDB_SetShutDown(BOARD_HIPERFACE_BASEADDR)) == kStatus_Success)
					PRINTF("    Success.\r\n");
				break;
			case RDB_CMD_GET_SET_POSITION:
				if ((status = DSL_RDB_GetSetPosition(BOARD_HIPERFACE_BASEADDR, &v64u_0)) == kStatus_Success)
					PRINTF("    Current SetPosition: %lld.\r\n", v64u_0);
				break;
			case RDB_CMD_SET_SET_POSITION:
				PRINTF("Please input the position: ");
				v64u_0 = getV64ValueAndEcho();
				if ((status = DSL_RDB_SetSetPosition(BOARD_HIPERFACE_BASEADDR, v64u_0)) == kStatus_Success)
					PRINTF("    Success.\r\n");
				break;
			case RDB_CMD_GET_CURRENT_ACCESS_LVL:
				if ((status = DSL_RDB_GetCurrentAccessLevel(BOARD_HIPERFACE_BASEADDR, &v8u_0)) == kStatus_Success)
					PRINTF("    Current Access Level: %s.\r\n", DSL_RDB_AccessLevelToStr((uint16_t)v8u_0));
				break;
			case RDB_CMD_SET_ACCESS_LEVEL:
				PRINTF("    0 Public\r\n");
				PRINTF("    1 Operator\r\n");
				PRINTF("    2 Maintenance\r\n");
				PRINTF("    3 Authorized\r\n");
				PRINTF("    4 Service\r\n");
				PRINTF("Please select Access Leval:");
				choice_1 = getValueAndEcho();
				PRINTF("Standard access key:\r\n");
				PRINTF("    0 Public:      No access key necessary. Press Enter\r\n");
				PRINTF("    1 Operator:    0x31313131\r\n");
				PRINTF("    2 Maintenance: 0x32323232\r\n");
				PRINTF("    3 Authorized:  0x33333333\r\n");
				PRINTF("    4 Service:     0x34343434\r\n");
				PRINTF("Please input access key(uint32_t): ");
				v32u_0 = getValueAndEcho();
				if ((status = DSL_RDB_SetAccessLevel(BOARD_HIPERFACE_BASEADDR, choice_1, v32u_0)) == kStatus_Success)
					PRINTF("    Success.\r\n");
				break;
			case RDB_CMD_CHANGE_ACCESS_KEY:
				PRINTF("    0 Public\r\n");
				PRINTF("    1 Operator\r\n");
				PRINTF("    2 Maintenance\r\n");
				PRINTF("    3 Authorized\r\n");
				PRINTF("    4 Service\r\n");
				PRINTF("Please select Access Leval:");
				choice_1 = getValueAndEcho();
				PRINTF("Standard access key:\r\n");
				PRINTF("    0 Public:      No access key necessary. Press Enter\r\n");
				PRINTF("    1 Operator:    0x31313131\r\n");
				PRINTF("    2 Maintenance: 0x32323232\r\n");
				PRINTF("    3 Authorized:  0x33333333\r\n");
				PRINTF("    4 Service:     0x34343434\r\n");
				PRINTF("Please input access old key(uint32_t): ");
				v32u_0 = getValueAndEcho();
				PRINTF("Please input access new key(uint32_t): ");
				v32u_1 = getValueAndEcho();
				if ((status = DSL_RDB_ChangeAccessKey(BOARD_HIPERFACE_BASEADDR, choice_1, v32u_1, v32u_0)) == kStatus_Success)
					PRINTF("    Success.\r\n");
				break;
			case RDB_CMD_READ_COUNTER:
				if ((status = DSL_RDB_GetReadCounter(BOARD_HIPERFACE_BASEADDR, &v32u_0)) == kStatus_Success)
					PRINTF("    Counter: %d.\r\n", v32u_0);
				break;
			case RDB_CMD_SET_INCREMENT_COUNTER:
				if ((status = DSL_RDB_SetIncrementCounter(BOARD_HIPERFACE_BASEADDR)) == kStatus_Success)
					PRINTF("    Success.\r\n");
				break;
			case RDB_CMD_RESET_COUNTER:
				if ((status = DSL_RDB_SetResetcounter(BOARD_HIPERFACE_BASEADDR)) == kStatus_Success)
					PRINTF("    Success.\r\n");
				break;
			case RDB_CMD_FILE_OPERATION:
			    PrintFileOperationMenu();
				PRINTF("Please input file operation command: ");
				choice_1 = getValueAndEcho();
				PRINTF("    File operations are not supported in this version.\r\n");
				break;
			case RDB_CMD_SIMPLE_IO_OPERATION:
				PRINTF("    Simple I/Os operations are not supported in this version.\r\n");
				break;
			default:
				PRINTF("    Unknow cmd.\r\n");
				break;
		}
		if (status != kStatus_Success) {
		    PRINTF("    Failed with status: 0x%x\r\n", status);
		}
	}
	DSL_RDB_FreeAllNodeDefiningValue(BOARD_HIPERFACE_BASEADDR, enc);
}

void DumpEncoderStatus(void)
{
	uint8_t enc_st;
	PRINTF("    Status Summary Bit: 0x%x\r\n", BOARD_HIPERFACE_BASEADDR->SAFE_SUM);
	enc_st = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x40);
	PRINTF("        enc_st0=0x%x\r\n",enc_st);
	enc_st = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x41);
	PRINTF("        enc_st1=0x%x\r\n",enc_st);
	enc_st = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x42);
	PRINTF("        enc_st2=0x%x\r\n",enc_st);
	enc_st = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x43);
	PRINTF("        enc_st3=0x%x\r\n",enc_st);
	enc_st = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x44);
	PRINTF("        enc_st4=0x%x\r\n",enc_st);
	enc_st = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x45);
	PRINTF("        enc_st5=0x%x\r\n",enc_st);
	enc_st = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x46);
	PRINTF("        enc_st6=0x%x\r\n",enc_st);
	enc_st = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x47);
	PRINTF("        enc_st7=0x%x\r\n",enc_st);
}

void EncoderStatusClear(void)
{
	ENC_ST_register_writing(BOARD_HIPERFACE_BASEADDR, 0x40, 0);
	ENC_ST_register_writing(BOARD_HIPERFACE_BASEADDR, 0x41, 0);
	ENC_ST_register_writing(BOARD_HIPERFACE_BASEADDR, 0x42, 0);
	ENC_ST_register_writing(BOARD_HIPERFACE_BASEADDR, 0x43, 0);
	ENC_ST_register_writing(BOARD_HIPERFACE_BASEADDR, 0x44, 0);
	ENC_ST_register_writing(BOARD_HIPERFACE_BASEADDR, 0x45, 0);
	ENC_ST_register_writing(BOARD_HIPERFACE_BASEADDR, 0x46, 0);
	ENC_ST_register_writing(BOARD_HIPERFACE_BASEADDR, 0x47, 0);
}

void DumpSlave_SRSSI(void)
{
	uint8_t srssi = Slave_SRSSI_register_reading(BOARD_HIPERFACE_BASEADDR);
	PRINTF("    SRSSI : 0x%x\r\n", srssi);
}

void SlaveMail_Send(uint8_t mail_value)
{
	Slave_Mail_register_writing(BOARD_HIPERFACE_BASEADDR, mail_value);
}

void SlavePing_Test(void)
{
	uint8_t ping;
	uint32_t i;
	for(i = 0; i < 3; i++) {
		Slave_Ping_register_writing(BOARD_HIPERFACE_BASEADDR, 0x57);
		ping = Slave_Ping_register_reading(BOARD_HIPERFACE_BASEADDR);
		PRINTF("    ping meaage test %d: %s\r\n", i, ping == 0x57 ? "Ok" : "Fault");
		SDK_DelayAtLeastUs(500000, SystemCoreClock);
	}
}

void RemoteSlaveRegisterAccess(void)
{
	int mail_value;
	while (1) {
		PRINTF("|-----------------------------------------|\r\n");
		PRINTF("|  1, Read the Encoder status.            |\r\n");
		PRINTF("|  2, Clear the Encoder status.           |\r\n");
		PRINTF("|  3, Dump Slave SRSSI.                   |\r\n");
		PRINTF("|  4, Send Slave Mail.                    |\r\n");
		PRINTF("|  5, Slave Ping Test.                    |\r\n");
		PRINTF("|-----------------------------------------|\r\n");
		PRINTF("Please select command: ('b' to back to main menu'):");

		int choice = getValueAndEcho();
		if (choice == 'b' || choice == 'B') {
			break;
		}

		switch(choice) {
			case 1:
				DumpEncoderStatus(); break;
			case 2:
				EncoderStatusClear(); break;
			case 3:
				DumpSlave_SRSSI(); break;
			case 4:
				PRINTF("Please input mail value(uint8_t): ");
				mail_value = getValueAndEcho();
				SlaveMail_Send((uint8_t)mail_value);
				break;
			case 5:
				SlavePing_Test(); break;
			default:
				PRINTF("Unknown command.\r\n"); break;
		}
	}
}

/*Sync mode test*/
void SyncModeTest(void)
{
	hiperface_config_t config;
	config.es = DSL_getMaxES(APP_DEFAULT_PWM_FREQUENCE);
	config.pos_ready_mode = POS_READY_MODE_SHOWS_TIME_SYNC_TRANSMISSIONS;
	if (DSL_SyncModeEnable(BOARD_HIPERFACE_BASEADDR, APP_DEFAULT_PWM_FREQUENCE, &config) != kStatus_Success) {
		PRINTF("Invalid ES\r\n");
		return;
	}
	PRINTF("    ES: %d\r\n", config.es);
	/* Initialize FlexPWM to generate the trigger signal to trigge transmitting */
	PWM_Trigger_Init(BOARD_PWM_BASEADDR);
	PRINTF("    Sync mode test started.\r\n");
	syncPosRecvNum = 100;

	EnableIRQ(DEMO_XBARA_SYNC_POS_RCVD_IRQn);
	SDK_DelayAtLeastUs(1000000, SystemCoreClock);
	DSL_SyncModeDisable(BOARD_HIPERFACE_BASEADDR);
	PRINTF("    Sync mode test completed.\r\n");
}

/*!
 * @brief Main function
 */
int main(void)
{
	dsl_encoder_t enc;
	int choice;

	BOARD_InitHardware();
	PRINTF("Encoder Hiperface example:\r\n");

	/* Enable DSL Master*/
	hiperface_config_t config;
	DSL_GetDefaultConfig(&config);
	DSL_MasterInit(BOARD_HIPERFACE_BASEADDR, &config);
	DSL_EncoderInit(BOARD_HIPERFACE_BASEADDR, &enc);
	PRINTF("Check the Encoder link...\r\n");
	if (DSL_CheckLinkStatus(BOARD_HIPERFACE_BASEADDR, 5) != kStatus_Success) {
		PRINTF("No connection present or connection error due to a communications error\r\n");
	}

	/* Enable event listening */
	DSL_SetEventMaskMSUM(BOARD_HIPERFACE_BASEADDR, event_mask_h, 1);
	DSL_SetEventMaskMPOS(BOARD_HIPERFACE_BASEADDR, event_mask_h, 1);
	DSL_SetEventMaskMDTE(BOARD_HIPERFACE_BASEADDR, event_mask_h, 1);
	DSL_SetEventMaskMPRST(BOARD_HIPERFACE_BASEADDR,event_mask_h, 1);
	DSL_SetEventMaskMFREL(BOARD_HIPERFACE_BASEADDR, event_mask_l, 0);
	DSL_SetEventMaskMQMLW(BOARD_HIPERFACE_BASEADDR, event_mask_l, 1);
	DSL_SetEventMaskMANS(BOARD_HIPERFACE_BASEADDR, event_mask_l, 1);
	DSL_SetEventMaskMMIN(BOARD_HIPERFACE_BASEADDR, event_mask_h, 1);
	EnableIRQ(DEMO_HIPERFACE_IRQn);
	BOARD_HIPERFACE_BASEADDR->MASK_S = 0xFF;
	EnableIRQ(DEMO_HIPERFACE_S_IRQn);

	while (1) {
		choice = PrintMainMenu();
		switch (choice) {
			case MENU_CHOICE_MASTER_INFO:
				DumpMasterInfo(BOARD_HIPERFACE_BASEADDR); break;
			case MENU_CHOICE_REG_ACCESS_TEST:
				RegisterAccessPerformanceTest(BOARD_HIPERFACE_BASEADDR); break;
			case MENU_CHOICE_INQUIRY_RDB_INFO:
			    DumpRDB_Infomation(BOARD_HIPERFACE_BASEADDR, &enc); break;
			case MENU_CHOICE_ACCESS_RESOURCE:
				RDB_ResourceAccess(BOARD_HIPERFACE_BASEADDR, &enc); break;
			case MENU_CHOICE_REMOTE_SLAVE_REG_ACCESS:
				RemoteSlaveRegisterAccess(); break;
			case MENU_CHOICE_SYNC_MODE_TEST:
				SyncModeTest(); break;
			default:
				PRINTF("Invalid choice\r\n"); break;
		}
	}
}
