/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_hiperface.h"

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
dsl_encoder_t enc;
uint8_t event_mask_h = 0, event_mask_l = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
uint32_t SYSTICK_GET_COUNT()
{
	uint32_t val  = SysTick->VAL;
	uint32_t load = SysTick->LOAD;
	return load - val;
}

void DEMO_XBARA_IRQHandler(void)
{
	if (!DSL_GetEventEstimatorThresholdErr(BOARD_HIPERFACE_BASEADDR)) {
		PRINTF("Pos: %ld\r\n", DSL_GetFastPosition(BOARD_HIPERFACE_BASEADDR, &enc));
	} else {
		PRINTF("Estimator Deviation Threshold Error\r\n");
	}
}

void DEMO_HIPERFACE_POS_RCVD_IRQHandler(void)
{
	if (!DSL_GetEventEstimatorThresholdErr(BOARD_HIPERFACE_BASEADDR)) {
		PRINTF("Pos: %ld\r\n", DSL_GetFastPosition(BOARD_HIPERFACE_BASEADDR, &enc));
	} else {
		PRINTF("Estimator Deviation Threshold Error\r\n");
	}
}

void DEMO_HIPERFACE_S_IRQHandler(void)
{

}

void DEMO_HIPERFACE_IRQHandler(void)
{
	HIPERFACE_Type *base = BOARD_HIPERFACE_BASEADDR;
	if (DSL_GetEventSlaveEventSum(base) && DSL_GetEventMaskMSUM(event_mask_h)) {
			PRINTF("The DSL Slave has signaled an event and the summary mask is set accordingly\r\n");
			DSL_ClrEventSlaveEventSum(base);
	} 

	if (DSL_GetEventeEstimatorOn(base) && DSL_GetEventMaskMPOS(event_mask_h)) {
			PRINTF("Fast position data consistency error. The fast position read through drive interface is supplied by the estimator\r\n");
			DSL_ClrEventeEstimatorOn(base);
	}

	if (DSL_GetEventEstimatorThresholdErr(base) && DSL_GetEventMaskMDTE(event_mask_h)) {
			PRINTF("Current value of deviation greater than the specified maximum\r\n");
			DSL_ClrEventEstimatorThresholdErr(base);
	}

	if (DSL_GetEventProtocolRstWarning(base) && DSL_GetEventMaskMPRST(event_mask_h)) {
			PRINTF("The forced protocol reset was triggered\r\n");
			DSL_ClrEventProtocolRstWarning(base);
	}

	if (DSl_GetEventMsgInitStatus(base) && DSL_GetEventMaskMMIN(event_mask_l)) {
			PRINTF("An acknowledgment was received from the Slave for the initialization of a message\r\n");
			DSl_ClrEventMsgInitStatus(base);
	}

	if (DSl_GetEventLongMsgAnswerErr(base) && DSL_GetEventMaskMANS(event_mask_l)) {
			PRINTF("An error occurred during the answer to a long message. The effectiveness of the previous transaction is not known\r\n");
			DSl_ClrEventLongMsgAnswerErr(base);
	}

	if (DSl_GetEventQMLowValueWarning(base) && DSL_GetEventMaskMQMLW(event_mask_l)) {
			PRINTF("Quality monitoring value below \"14\"\r\n");
			DSl_ClrEventQMLowValueWarning(base);
	}

	if (DSl_GetEventLongMsgChannelfree(base) && DSL_GetEventMaskMFREL(event_mask_l)) {
			PRINTF("A \"long message\" can be sent on the Parameters Channel\r\n");
			DSl_ClrEventLongMsgChannelfree(base);
	}
}

/*!
 * @brief Main function
 */

int main(void)
{

	dsl_encoder_version_info_t info;
    PRINTF("Encoder Hiperface example\n");

	/* Enable DSL Master*/
	hiperface_config_t config;
	DSL_GetDefaultConfig(&config);
	DSL_MasterInit(BOARD_HIPERFACE_BASEADDR, &config);
	DSL_EncoderInit(BOARD_HIPERFACE_BASEADDR, &enc);
	if (DSL_CheckLinkStatus(BOARD_HIPERFACE_BASEADDR, 5) != kStatus_Success) {
		PRINTF("No connection present or connection error due to a communications error\r\n");
		return -1;
	}

	DSL_GetMasterReleaseInfo(BOARD_HIPERFACE_BASEADDR, &info);
	PRINTF("Type of IP Core: %d\r\n", info.coding);
	PRINTF("IP Core Major release number: %d\r\n", info.majorNumber);
	PRINTF("IP Core Minor release number: %d\r\n", info.minorNumber);
	PRINTF("IP Core Release date: %d-%d-%d\r\n", info.year, info.month, info.day);

	uint64_t pos;
	int32_t speed;
	DSL_GetFastPositionAndSpeed(BOARD_HIPERFACE_BASEADDR, &pos, &speed);
	PRINTF("%lld, %d\r\n", pos, speed);
	
	/* Cache all RDB infomation */
	DSL_RDB_ReadAllNodeDefiningValue(BOARD_HIPERFACE_BASEADDR, &enc);
	DSL_RDB_DumpAllNodeDefiningValue(&enc);
	dsl_rdb_node_t *node;
	node = DSL_RDB_FindNodeFromCache(&enc.rootNode, DSL_RID_TypeOfEncoder);
	if (node)
		DSL_RDB_DumpNodeDefiningValue(node, 0);
	DSL_RDB_FreeAllNodeDefiningValue(BOARD_HIPERFACE_BASEADDR, &enc);

	uint16_t enc_type;
	DSL_RDB_GetTypeOfEncoder(BOARD_HIPERFACE_BASEADDR, &enc_type);
	PRINTF("Encoder Type: %s\r\n", DSL_RDB_TypeOfEncoderTostr(enc_type));

	uint32_t enc_resolution;
	DSL_RDB_GetResolution(BOARD_HIPERFACE_BASEADDR, &enc_resolution);
	PRINTF("Encoder Resolution: %d\r\n", enc_resolution);

	DSL_SetEventMaskMSUM(BOARD_HIPERFACE_BASEADDR, event_mask_h, 1);
	DSL_SetEventMaskMPOS(BOARD_HIPERFACE_BASEADDR, event_mask_h, 1);
	DSL_SetEventMaskMDTE(BOARD_HIPERFACE_BASEADDR, event_mask_h, 1);
	DSL_SetEventMaskMPRST(BOARD_HIPERFACE_BASEADDR,event_mask_h, 1);
	DSL_SetEventMaskMFREL(BOARD_HIPERFACE_BASEADDR, event_mask_l, 1);
	DSL_SetEventMaskMQMLW(BOARD_HIPERFACE_BASEADDR, event_mask_l, 1);
	DSL_SetEventMaskMANS(BOARD_HIPERFACE_BASEADDR, event_mask_l, 1);
	DSL_SetEventMaskMMIN(BOARD_HIPERFACE_BASEADDR, event_mask_h, 1);

	/* Test HIPERFACE_IRQn  */
	EnableIRQ(DEMO_HIPERFACE_IRQn);
	// Slave reset will generate some events.
	DSL_RDB_SetReset(BOARD_HIPERFACE_BASEADDR);
	// wait for the interruper to complete
	SDK_DelayAtLeastUs(2000000, SystemCoreClock);
	DisableIRQ(DEMO_HIPERFACE_IRQn);

	/*Test HIPERFACE_POS_RCVD_IRQn*/
	EnableIRQ(DEMO_HIPERFACE_POS_RCVD_IRQn);
	

	DSL_SetEventMaskMSUM(BOARD_HIPERFACE_BASEADDR, event_mask_h, 0);
	DSL_SetEventMaskMPOS(BOARD_HIPERFACE_BASEADDR, event_mask_h, 0);
	DSL_SetEventMaskMDTE(BOARD_HIPERFACE_BASEADDR, event_mask_h, 0);
	DSL_SetEventMaskMPRST(BOARD_HIPERFACE_BASEADDR, event_mask_h, 0);
	DSL_SetEventMaskMFREL(BOARD_HIPERFACE_BASEADDR, event_mask_l, 0);
	DSL_SetEventMaskMQMLW(BOARD_HIPERFACE_BASEADDR, event_mask_l, 0);
	DSL_SetEventMaskMANS(BOARD_HIPERFACE_BASEADDR, event_mask_l, 0);
	DSL_SetEventMaskMMIN(BOARD_HIPERFACE_BASEADDR, event_mask_l, 0);

	uint8_t enc_st0,enc_st1,enc_st2,enc_st3,enc_st4,enc_st5,enc_st6,enc_st7;

	enc_st0 = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x40); PRINTF("enc_st0=0x%x mir_st0=0x%x\r\n",enc_st0, BOARD_HIPERFACE_BASEADDR->MIR_ST[0]); SDK_DelayAtLeastUs(2, SystemCoreClock);
	enc_st1 = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x41); PRINTF("enc_st1=0x%x mir_st1=0x%x\r\n",enc_st1, BOARD_HIPERFACE_BASEADDR->MIR_ST[1]); SDK_DelayAtLeastUs(2, SystemCoreClock);
	enc_st2 = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x42); PRINTF("enc_st2=0x%x mir_st2=0x%x\r\n",enc_st2, BOARD_HIPERFACE_BASEADDR->MIR_ST[2]); SDK_DelayAtLeastUs(2, SystemCoreClock);
	enc_st3 = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x43); PRINTF("enc_st3=0x%x mir_st3=0x%x\r\n",enc_st3, BOARD_HIPERFACE_BASEADDR->MIR_ST[3]); SDK_DelayAtLeastUs(2, SystemCoreClock);
	enc_st4 = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x44); PRINTF("enc_st4=0x%x mir_st4=0x%x\r\n",enc_st4, BOARD_HIPERFACE_BASEADDR->MIR_ST[4]); SDK_DelayAtLeastUs(2, SystemCoreClock);
	enc_st5 = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x45); PRINTF("enc_st5=0x%x mir_st5=0x%x\r\n",enc_st5, BOARD_HIPERFACE_BASEADDR->MIR_ST[5]); SDK_DelayAtLeastUs(2, SystemCoreClock);
	enc_st6 = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x46); PRINTF("enc_st6=0x%x mir_st6=0x%x\r\n",enc_st6, BOARD_HIPERFACE_BASEADDR->MIR_ST[6]); SDK_DelayAtLeastUs(2, SystemCoreClock);
	enc_st7 = ENC_ST_register_reading(BOARD_HIPERFACE_BASEADDR, 0x47); PRINTF("enc_st7=0x%x mir_st7=0x%x\r\n",enc_st7, BOARD_HIPERFACE_BASEADDR->MIR_ST[7]); SDK_DelayAtLeastUs(2, SystemCoreClock);

	uint8_t srssi = Slave_SRSSI_register_reading(BOARD_HIPERFACE_BASEADDR); PRINTF("srssi = 0x%x\r\n",srssi);

	uint8_t ping_v;
	ping_v = Slave_Ping_register_reading(BOARD_HIPERFACE_BASEADDR);
	PRINTF("Default ping value: 0x%x\r\n", ping_v);

	// send ping message: 0x57
	Slave_Ping_register_writing(BOARD_HIPERFACE_BASEADDR, 0x57);
	SDK_DelayAtLeastUs(20, SystemCoreClock);
	ping_v = Slave_Ping_register_reading(BOARD_HIPERFACE_BASEADDR);
	PRINTF("ping meaage test: %s\r\n", ping_v == 0x57 ? "Ok" : "Fault");

	if (DSL_SyncModeEnable(BOARD_HIPERFACE_BASEADDR, APP_DEFAULT_PWM_FREQUENCE, 5) != kStatus_Success) {
		PRINTF("Invalid ES\r\n");
		return -1;
	}

	/* Initialize FlexPWM to generate the trigger signal to trigge transmitting */
	PWM_Trigger_Init(BOARD_PWM_BASEADDR);

    // Enable Interrupt
  	EnableIRQ(XBAR1_CH0_CH1_IRQn);
	while (1);
}
