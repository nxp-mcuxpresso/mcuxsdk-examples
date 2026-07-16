/** @file ncp_ble.c
 *
 *  @brief ncp_ble file
 *
 *  Copyright 2020 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */
#if CONFIG_NCP_BLE
///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
#include "fsl_os_abstraction.h"
#include "fsl_component_log_config.h"
#include "psa/crypto.h"

#include "ncp_glue_ble.h"
#include "ncp_ble.h"
#include "ncp_pm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BLE_NCP_COMMAND_QUEUE_NUM 16

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
typedef struct ncp_cmd_t ble_ncp_command_t;

extern int ble_ncp_L2capInit(void);
#if CONFIG_NCP_USE_ENCRYPT
extern void ncp_set_mbedtls_set_time();
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/
extern uint32_t current_cmd;
extern uint16_t g_cmd_seqno;
extern uint8_t cmd_buf[NCP_INBUF_SIZE];

bool is_create_conn_cmd = false;

/* Indicate the state of ncp ble*/
atomic_t ncp_ble_state[1];

//os_thread_t ble_ncp_thread;                         /* ncp task */
//static os_thread_stack_define(ble_ncp_stack, 4096); /* ncp task stack*/
#define BLE_TASK_PRIO        (PRIORITY_RTOS_TO_OSA(((configMAX_PRIORITIES - CONFIG_NCP_PRIORITY_OFFSET)-3)))
#define BLE_NCP_STACK_SIZE   4096

static OSA_TASK_HANDLE_DEFINE(ble_ncp_handle);

static void ble_ncp_task(void *pvParameters);
OSA_TASK_DEFINE(ble_ncp_task, BLE_TASK_PRIO, 1, BLE_NCP_STACK_SIZE, 0);

OSA_SEMAPHORE_HANDLE_DEFINE(ble_ncp_lock);
OSA_SEMAPHORE_HANDLE_DEFINE(ncp_ble_resp_buf_lock);

/*BLE NCP COMMAND QUEUE*/
static osa_msgq_handle_t ble_ncp_command_queue; /* ncp adapter TX msgq */
OSA_MSGQ_HANDLE_DEFINE(ble_ncp_command_queue_buff, BLE_NCP_COMMAND_QUEUE_NUM,  sizeof(ble_ncp_command_t));

static const ncp_pm_ops_t *s_pm_ops = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* ble_ncp_send_response() handles the response .
 * This involves
 * 1) sending cmd response out to interface
 * 2) computation of the crc of the cmd resp
 * 3) reset cmd_buf & ble_res_buf
 * 4) release  lock
 */
int ble_ncp_send_response(uint8_t *pbuf)
{
    int ret                = NCP_CMD_RESULT_OK;
    uint16_t transfer_len = 0;
    NCP_COMMAND *res = (NCP_COMMAND *)pbuf;

    /* set cmd response seqno */
    res->seqnum = (GET_MSG_TYPE(res->cmd) == NCP_MSG_TYPE_RESP) ? g_cmd_seqno : 0;
    transfer_len        = res->size;
    if (transfer_len >= NCP_CMD_HEADER_LEN)
    {
        /* write response to host */
        ret = ncp_tlv_send(pbuf, transfer_len);
        if (ret != NCP_CMD_RESULT_OK)
        {
            ncp_e("failed to write response");
            ret = -NCP_CMD_RESULT_ERROR;
        }
    }
    else
    {
        ncp_e("command length is less than 12, cmd_len = %d", transfer_len);
        ret = -NCP_CMD_RESULT_ERROR;
    }

    if (GET_MSG_TYPE(res->cmd) != NCP_MSG_TYPE_EVENT)
    {
        /* Reset cmd_buf */
        memset(cmd_buf, 0, sizeof(cmd_buf));
        /* Reset ble_res_buf */
        // memset(ble_res_buf, 0, sizeof(ble_res_buf));
        OSA_SemaphorePost(ble_ncp_lock);
        ncp_d("put  lock");
    }

    ncp_put_ble_resp_buf_lock();

    return ret;
}


int ble_ncp_command_handle_input(uint8_t *cmd)
{
    NCP_COMMAND *input_cmd = (NCP_COMMAND *)cmd;
    struct cmd_t *command         = NULL;
    int ret                       = NCP_CMD_RESULT_OK;

    uint32_t cmd_class    = GET_CMD_CLASS(input_cmd->cmd);
    uint32_t cmd_subclass = GET_CMD_SUBCLASS(input_cmd->cmd);
    uint32_t cmd_id       = GET_CMD_ID(input_cmd->cmd);
    void *cmd_tlv         = GET_CMD_TLV(input_cmd);
    current_cmd           = input_cmd->cmd;
    g_cmd_seqno           = input_cmd->seqnum;

    command = lookup_class(cmd_class, cmd_subclass, cmd_id);
    if (NULL == command)
    {
        ncp_d("ncp ble lookup cmd failed\r\n");
        return -NCP_CMD_RESULT_ERROR;
    }

    ncp_d("ncp ble got command: <%s>", command->help);

    is_create_conn_cmd = false;
    ret = command->handler(cmd_tlv);

    return ret;
}

static void ble_ncp_task(void *pvParameters)
{
    int ret = 0;
    ble_ncp_command_t cmd_item;
    uint8_t *cmd_buf = NULL;
    while (1)
    {
        ret = OSA_MsgQGet(ble_ncp_command_queue, &cmd_item, osaWaitForever_c);
        if (ret != NCP_CMD_RESULT_OK)
        {
            ncp_e("ble ncp command queue receive failed");
            continue;
        }
        else
        {
            cmd_buf = cmd_item.cmd_buff;
            OSA_SemaphoreWait(ble_ncp_lock, osaWaitForever_c);
            ble_ncp_command_handle_input(cmd_buf);
            OSA_MemoryFree(cmd_buf);
            cmd_buf = NULL;
        }
    }
}

static void ble_ncp_callback(void *tlv, size_t tlv_sz, int status)
{
    int ret = 0;
    ble_ncp_command_t cmd_item;
    cmd_item.block_type = 0;
    cmd_item.command_sz = tlv_sz;
    cmd_item.cmd_buff = (ncp_tlv_data_qelem_t *)OSA_MemoryAllocate(tlv_sz);
    if (!cmd_item.cmd_buff)
    {
        ncp_adap_e("failed to allocate memory for tlv queue element");
        return ;
    }
    memcpy(cmd_item.cmd_buff, tlv, tlv_sz);

    ret = OSA_MsgQPut(ble_ncp_command_queue, &cmd_item);
    if (ret != kStatus_Success)
        ncp_e("send to ble ncp cmd queue failed");
    else
        ncp_d("success to send ncp command on queue");
}

int ble_ncp_init(void)
{
    int ret;

    s_pm_ops = ncp_pm_get_ops();

    ble_ncp_command_queue = (osa_msgq_handle_t)ble_ncp_command_queue_buff;
    ret = OSA_MsgQCreate(ble_ncp_command_queue, BLE_NCP_COMMAND_QUEUE_NUM,  sizeof(ble_ncp_command_t));
    if (ret != NCP_CMD_RESULT_OK)
    {
        ncp_e("failed to create ble ncp command queue: %d", ret);
        return NCP_CMD_RESULT_ERROR;
    }

    ret = OSA_SemaphoreCreateBinary(ble_ncp_lock);
    if (ret != kStatus_Success)
    {
        ncp_e("failed to create ble_ncp_lock: %d", ret);
        return ret;
    }
    else
    {
        OSA_SemaphorePost(ble_ncp_lock);
    }

    ret = OSA_SemaphoreCreateBinary(ncp_ble_resp_buf_lock);
    if (ret != kStatus_Success)
    {
        ncp_e("failed to create system_ncp_lock: %d", ret);
        return ret;
    }
    else
    {
        OSA_SemaphorePost(ncp_ble_resp_buf_lock);
    }

    ret = ble_ncp_L2capInit();
    if (ret != NCP_CMD_RESULT_OK)
    {
        ncp_e("failed to init the ncp l2cap: %d", ret);
        return NCP_CMD_RESULT_ERROR;
    }
    ncp_tlv_install_handler(GET_CMD_CLASS(NCP_CMD_BLE), (void *)ble_ncp_callback);

    ret = OSA_TaskCreate((osa_task_handle_t) ble_ncp_handle, OSA_TASK(ble_ncp_task), NULL);
    if (ret != NCP_CMD_RESULT_OK)
    {
        ncp_e("failed to create ncp ble task: %d", ret);
        return NCP_CMD_RESULT_ERROR;
    }
    if (bt_init() != NCP_CMD_RESULT_OK)
    {
         ncp_e("Failed to initialize BLE\n");
         return ret;
    }

    psa_crypto_init();
#if CONFIG_NCP_USE_ENCRYPT
    ncp_set_mbedtls_set_time();
#endif
    //printSeparator();
    PRINTF("BLE initialized\r\n");

    return NCP_CMD_RESULT_OK;
}

void ncp_get_ble_resp_buf_lock()
{
    OSA_SemaphoreWait(ncp_ble_resp_buf_lock, osaWaitForever_c);
}

void ncp_put_ble_resp_buf_lock()
{
    OSA_SemaphorePost(ncp_ble_resp_buf_lock);
}


void ncp_ble_state_set(int state)
{
    if (atomic_test_and_set_bit(ncp_ble_state, state))
        return;

    if (state == NCP_BLE_SCANNING)
    {
        if (s_pm_ops && s_pm_ops->enter_critical)
        {
            s_pm_ops->enter_critical();
        }
    }
}

void ncp_ble_state_clear(int state)
{
    if (!atomic_test_and_clear_bit(ncp_ble_state, state))
        return;

    if (state == NCP_BLE_SCANNING)
    {
        if (s_pm_ops && s_pm_ops->exit_critical)
        {
            s_pm_ops->exit_critical();
        }
    }
}

bool ncp_ble_state_check(int state)
{
    return atomic_test_bit(ncp_ble_state, state);
}

bool ncp_ble_is_running(void)
{
    bool ret = false;

    if (ncp_ble_state_check(NCP_BLE_SCANNING) == true) //BLE is scanning
        ret = true;

    return ret;
}
#endif /* CONFIG_NCP_BLE */
