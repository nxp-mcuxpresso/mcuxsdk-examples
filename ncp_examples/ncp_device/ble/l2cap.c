/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if CONFIG_NCP_BLE

#include <porting.h>
#include <bluetooth/l2cap.h>
#include <net/buf.h>
#include <sys/util.h>

#include "ncp_glue_ble.h"
#include "fsl_component_log_config.h"
#define LOG_MODULE_NAME bt_l2cap
#include "fsl_component_log.h"

LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAX_L2CAP_CHANNEL 1

#define CREDITS			10
#define DATA_MTU		(23 * CREDITS)

#define L2CAP_POLICY_NONE		0x00
#define L2CAP_POLICY_ALLOWLIST		0x01
#define L2CAP_POLICY_16BYTE_KEY		0x02
NET_BUF_POOL_FIXED_DEFINE(data_tx_pool, 1, BT_L2CAP_SDU_BUF_SIZE(DATA_MTU), CONFIG_BT_CONN_TX_USER_DATA_SIZE, NULL);
NET_BUF_POOL_FIXED_DEFINE(data_rx_pool, 1, BT_L2CAP_SDU_BUF_SIZE(DATA_MTU), CONFIG_NET_BUF_USER_DATA_SIZE, NULL);


#if 0
#define CONTROLLER_INDEX        0
#define DATA_MTU_INITIAL        128
#define DATA_MTU                256
#define DATA_BUF_SIZE           BT_L2CAP_SDU_BUF_SIZE(DATA_MTU)
#define CHANNELS                2
#define SERVERS                 1

typedef struct channel_tag
{
    uint8_t chan_id; /* Internal number that identifies L2CAP channel. */
    struct bt_l2cap_le_chan le;
    bool in_use;
    bool hold_credit;
    struct net_buf *pending_credit;
} channel_t;
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if 0
/* Event callbacks */
static struct net_buf *alloc_buf_cb(struct bt_l2cap_chan *chan);
static void connected_cb(struct bt_l2cap_chan *l2cap_chan);
static void disconnected_cb(struct bt_l2cap_chan *l2cap_chan);
static int recv_cb(struct bt_l2cap_chan *l2cap_chan, struct net_buf *buf);

#if (defined(CONFIG_BT_L2CAP_ECRED) && (CONFIG_BT_L2CAP_ECRED == 1))
static void reconfigured_cb(struct bt_l2cap_chan *l2cap_chan);
#endif /* CONFIG_BT_L2CAP_ECRED */

/* Command handlers */
static void supported_commands(uint8_t *data, uint16_t len);
static void ble_l2cap_connect(uint8_t *data, uint16_t len);
static void disconnect(uint8_t *data, uint16_t len);
static void send_data(uint8_t *data, uint16_t len);
static void ble_l2cap_listen(uint8_t *data, uint16_t len);
static void credits(uint8_t *data, uint16_t len);

#if (defined(CONFIG_BT_L2CAP_ECRED) && (CONFIG_BT_L2CAP_ECRED == 1))
static void reconfigure(uint8_t *data, uint16_t len);
#endif /* CONFIG_BT_L2CAP_ECRED */

#if (defined(CONFIG_BT_EATT) && (CONFIG_BT_EATT == 1))
void disconnect_eatt_chans(uint8_t *data, uint16_t len);
#endif /* CONFIG_BT_EATT */

/* Helpers */
static channel_t *get_free_channel(void);
static int ble_l2cap_accept(struct bt_conn *conn, struct bt_l2cap_server *server, struct bt_l2cap_chan **l2cap_chan);
static bool is_free_psm(uint16_t psm);
static struct bt_l2cap_server *get_free_server(void);
#endif
static int l2cap_accept(struct bt_conn *conn, struct bt_l2cap_server *server,
			struct bt_l2cap_chan **chan);
void ble_l2cap_set_recv(uint8_t *data, uint16_t len);
void ble_l2cap_metrics(uint8_t *data, uint16_t len);
void bt_l2cap_register(uint8_t *data, uint16_t len);
void ble_l2cap_connect(uint8_t *data, uint16_t len);
void ble_l2cap_disconnect(uint8_t *data, uint16_t len);
void ble_l2cap_send_data(uint8_t *data, uint16_t len);
int ble_ncp_L2capInit(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
#if 0
NET_BUF_POOL_FIXED_DEFINE(data_pool, CHANNELS, DATA_BUF_SIZE, NULL);

static channel_t channels[CHANNELS];
static struct bt_l2cap_server servers[SERVERS];

static bool authorize_flag;
static uint8_t req_keysize;

static uint8_t recv_cb_buf[DATA_BUF_SIZE + sizeof(l2cap_data_received_ev_t)];

static const struct bt_l2cap_chan_ops l2cap_ops = {
    .alloc_buf          = alloc_buf_cb,
    .recv               = recv_cb,
    .connected          = connected_cb,
    .disconnected   = disconnected_cb,
#if (defined(CONFIG_BT_L2CAP_ECRED) && (CONFIG_BT_L2CAP_ECRED == 1))
    .reconfigured   = reconfigured_cb,
#endif /* CONFIG_BT_L2CAP_ECRED */
};
#endif

static uint8_t l2cap_policy;

static struct bt_conn *l2cap_allowlist[CONFIG_BT_MAX_CONN];
static uint32_t l2cap_rate;
static uint32_t l2cap_recv_delay_ms;
static K_FIFO_DEFINE(l2cap_recv_fifo);
static bool metrics;

#define L2CH_CHAN(_chan) CONTAINER_OF(_chan, struct l2ch, ch.chan)
#define L2CH_WORK(_work) CONTAINER_OF(k_work_delayable_from_work(_work), \
				      struct l2ch, recv_work)
#define L2CAP_CHAN(_chan) _chan->ch.chan

struct l2ch {
	bool used;
	struct k_work_delayable recv_work;
	struct bt_l2cap_le_chan ch;
};



static struct bt_l2cap_server server = {
	.accept		= l2cap_accept,
};
/*******************************************************************************
 * Code
 ******************************************************************************/

static int l2cap_recv_metrics(struct bt_l2cap_chan *chan, struct net_buf *buf)
{
	static uint32_t len;
	static uint32_t cycle_stamp;
	uint32_t delta;

	delta = k_cycle_get_32() - cycle_stamp;
	delta = k_cyc_to_ns_floor64(delta);

	/* if last data rx-ed was greater than 1 second in the past,
	 * reset the metrics.
	 */
	if (delta > 1000000000) {
		len = 0U;
		l2cap_rate = 0U;
		cycle_stamp = k_cycle_get_32();
	} else {
		len += buf->len;
		l2cap_rate = ((uint64_t)len << 3) * 1000000000U / delta;
	}
        l2cap_rate = l2cap_rate/1000;
	return NCP_CMD_RESULT_OK;
}

static void l2cap_recv_cb(struct k_work *work)
{
	struct l2ch *c = L2CH_WORK(work);
	struct net_buf *buf;

	while ((buf = net_buf_get(&l2cap_recv_fifo, K_NO_WAIT))) {
		ncp_d("Confirming reception\n");
		bt_l2cap_chan_recv_complete(&c->ch.chan, buf);
	}
}

static void dump_hex(const void *data, unsigned len)
{
    (void)PRINTF("**** Dump @ %p Len: %d ****\n\r", data, len);

    unsigned int i    = 0;
    const char *data8 = (const char *)data;
    while (i < len)
    {
        (void)PRINTF("%02x ", data8[i++]);
        if (!(i % 16))
        {
            (void)PRINTF("\n\r");
        }
    }

    (void)PRINTF("\n\r******** End Dump *******\n\r");
}

static int l2cap_recv(struct bt_l2cap_chan *chan, struct net_buf *buf)
{
    struct l2ch *l2ch = L2CH_CHAN(chan);
    struct l2cap_reveive_ev ev = {0};
    const bt_addr_le_t *addr = bt_conn_get_dst(chan->conn);

    if (metrics) {
	return l2cap_recv_metrics(chan, buf);
    }

    ncp_d("Incoming data channel %p len %u\n", chan, buf->len);
#if 0
    if (buf->len) {
	dump_hex(buf->data, buf->len);
    }
#endif
    if (addr != NULL)
    {
        memcpy(ev.address, addr->a.val, sizeof(ev.address));
        ev.address_type = addr->type;
    }
    ev.psm = l2ch->ch.psm;
    memcpy((uint8_t *)ev.data, buf->data, buf->len);
    ev.len = buf->len;
    ncp_d("sizeof(ev) len %d\n", sizeof(ev));
    ble_prepare_status(NCP_EVENT_L2CAP_RECEIVE, NCP_CMD_RESULT_OK, (uint8_t *) &ev,10 + ev.len);

    if (l2cap_recv_delay_ms > 0) {
	/* Submit work only if queue is empty */
	if (k_fifo_is_empty(&l2cap_recv_fifo)) {
		ncp_d("Delaying response in %u ms...\n",
	                            l2cap_recv_delay_ms);
	    }
        net_buf_put(&l2cap_recv_fifo, buf);
        k_work_schedule(&l2ch->recv_work, K_MSEC(l2cap_recv_delay_ms));

	    return -EINPROGRESS;
    }

    return NCP_CMD_RESULT_OK;
}

static void l2cap_sent(struct bt_l2cap_chan *chan)
{
    ncp_d("Outgoing data channel %p transmitted\n", chan);
}

static void l2cap_status(struct bt_l2cap_chan *chan, atomic_t *status)
{
    ncp_d("Channel %p status %u\n", chan, (uint32_t)*status);
}

static void l2cap_connected(struct bt_l2cap_chan *chan)
{
    struct l2ch *c = L2CH_CHAN(chan);
    struct l2cap_connect_ev ev = {0};
    const bt_addr_le_t *addr = bt_conn_get_dst(chan->conn);

    k_work_init_delayable(&c->recv_work, l2cap_recv_cb);
    ncp_d("Channel %p connected\n", chan);
    if (addr != NULL)
    {
        memcpy(ev.address, addr->a.val, sizeof(ev.address));
        ev.address_type = addr->type;
    }
    ev.psm = c->ch.psm;

    ble_prepare_status(NCP_EVENT_L2CAP_CONNECT, NCP_CMD_RESULT_OK, (uint8_t *) &ev, sizeof(ev));

}

static struct net_buf *l2cap_alloc_buf(struct bt_l2cap_chan *chan)
{
	/* print if metrics is disabled */
	if (!metrics) {
		ncp_d("Channel %p requires buffer\n", chan);
	}

	return net_buf_alloc(&data_rx_pool, osaWaitForever_c);
}
static void l2cap_disconnected(struct bt_l2cap_chan *chan);

static const struct bt_l2cap_chan_ops l2cap_ops = {
	.alloc_buf	= l2cap_alloc_buf,
	.recv		= l2cap_recv,
	.sent		= l2cap_sent,
	.status		= l2cap_status,
	.connected	= l2cap_connected,
	.disconnected	= l2cap_disconnected,
};

static struct l2ch l2ch_chan = {
	.ch.chan.ops	= &l2cap_ops,
	.ch.rx.mtu	= DATA_MTU,
};

static void l2cap_disconnected(struct bt_l2cap_chan *chan)
{
    struct l2cap_connect_ev ev = {0};
    const bt_addr_le_t *addr = bt_conn_get_dst(chan->conn);

    if (addr != NULL)
    {
        memcpy(ev.address, addr->a.val, sizeof(ev.address));
        ev.address_type = addr->type;
    }
    ev.psm = l2ch_chan.ch.psm;

    ble_prepare_status(NCP_EVENT_L2CAP_DISCONNECT, NCP_CMD_RESULT_OK, (uint8_t *) &ev, sizeof(ev));

    ncp_d("Channel %p disconnected\n", chan);
}

static void l2cap_allowlist_remove(struct bt_conn *conn, uint8_t reason)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(l2cap_allowlist); i++) {
		if (l2cap_allowlist[i] == conn) {
			bt_conn_unref(l2cap_allowlist[i]);
			l2cap_allowlist[i] = NULL;
		}
	}
}

BT_CONN_CB_DEFINE(l2cap_conn_callbacks) = {
	.disconnected = l2cap_allowlist_remove,
};

static int l2cap_accept_policy(struct bt_conn *conn)
{
	int i;

	if (l2cap_policy == L2CAP_POLICY_16BYTE_KEY) {
		uint8_t enc_key_size = bt_conn_enc_key_size(conn);

		if (enc_key_size && enc_key_size < BT_ENC_KEY_SIZE_MAX) {
			return -EPERM;
		}
	} else if (l2cap_policy == L2CAP_POLICY_ALLOWLIST) {
		for (i = 0; i < ARRAY_SIZE(l2cap_allowlist); i++) {
			if (l2cap_allowlist[i] == conn) {
				return 0;
			}
		}

		return -EACCES;
	}

	return 0;
}

static int l2cap_accept(struct bt_conn *conn, struct bt_l2cap_server *server,
			struct bt_l2cap_chan **chan)
{
	int err;

	ncp_d("Incoming conn %p\n", conn);

	err = l2cap_accept_policy(conn);
	if (err < 0) {
		return err;
	}

	if (l2ch_chan.ch.chan.conn) {
		ncp_d("No channels available\n");
		return -ENOMEM;
	}

	*chan = &l2ch_chan.ch.chan;

	return 0;
}

/*
 * @brief   recv set
 */
void ble_l2cap_set_recv(uint8_t *data, uint16_t len)
{
    const struct l2cap_recv_cmd_tag *cmd = (void *) data;

    l2cap_recv_delay_ms = cmd->l2cap_recv_delay_ms;
    ncp_d("l2cap receive delay: %u ms\n",l2cap_recv_delay_ms);
    ble_prepare_status(NCP_RSP_BLE_L2CAP_RECEIVE, NCP_CMD_RESULT_OK, NULL, 0);
}

/*
 * @brief   CMD METRICS
 */
void ble_l2cap_metrics(uint8_t *data, uint16_t len)
{
    const struct l2cap_metrics_cmd_tag *cmd = (void *) data;
    uint8_t status = NCP_CMD_RESULT_ERROR;

    ncp_d("l2cap rate: %u bps.", l2cap_rate * 1000);
    if (cmd->metrics_flag == true)
    {
        metrics = true;
        status = NCP_CMD_RESULT_OK;
    }
    else if (cmd->metrics_flag == false)
    {
        metrics = false;
        status = NCP_CMD_RESULT_OK;
    }
    else
    {
        status = NCP_CMD_RESULT_ERROR;
    }
    ble_prepare_status(NCP_RSP_BLE_L2CAP_METRICS, status, NULL, 0);
}

/*
 * @brief   Register L2CAP PSM
 */
void bt_l2cap_register(uint8_t *data, uint16_t len)
{
    const struct l2cap_register_psm_cmd_tag *cmd = (void *) data;
    uint8_t status = NCP_CMD_RESULT_ERROR;
    // int err;
    // const char *policy;

    if (server.psm)
    {
        ncp_e("Already registered\n");
        goto sta;
    }

    server.psm = cmd->psm;
    if(cmd->sec_flag == 1)
    {
        server.sec_level = (bt_security_t)cmd->sec_level;
    }

    if(cmd->policy_flag == 1)
    {
	l2cap_policy = L2CAP_POLICY_ALLOWLIST;
    }
    else if (cmd->policy_flag == 2)
    {
	l2cap_policy = L2CAP_POLICY_16BYTE_KEY;
    }
    else
    {
	//do nothing
    }

    if (bt_l2cap_server_register(&server) < 0)
    {
	ncp_e("Unable to register psm\n");
	server.psm = 0U;
    }
    else
    {
        status = NCP_CMD_RESULT_OK;
	ncp_d("L2CAP psm %u sec_level %u registered\n",
			    server.psm, server.sec_level);
    }
sta:
    ble_prepare_status(NCP_RSP_BLE_L2CAP_REGISTER, status, NULL, 0);
}

/*
 * @brief   Create an L2CAP channel
 */
void ble_l2cap_connect(uint8_t *data, uint16_t len)
{
    const struct l2cap_connect_cmd_tag *cmd = (void *) data;
    struct bt_conn *conn;
    uint8_t status = NCP_CMD_RESULT_ERROR;
    int err;

    if (l2ch_chan.ch.chan.conn) {
         ncp_e("Channel already in use\n");
         goto sta;
    }
    conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, (bt_addr_le_t *)data);
    if (conn)
    {
	if (cmd->sec_flag == 1)
        {
            l2ch_chan.ch.required_sec_level = (bt_security_t)cmd->sec;
	}
	err = bt_l2cap_chan_connect(conn, &l2ch_chan.ch.chan, cmd->psm);
        bt_conn_unref(conn);
        status = err < 0 ? NCP_CMD_RESULT_ERROR : NCP_CMD_RESULT_OK;
    }
sta:
    ble_prepare_status(NCP_RSP_BLE_L2CAP_CONNECT, status, NULL, 0);
}

/*
 * @brief   Close an L2CAP channel
 */
void ble_l2cap_disconnect(uint8_t *data, uint16_t len)
{
    struct bt_conn *conn;
    uint8_t status = NCP_CMD_RESULT_ERROR;
    int err;

    conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, (bt_addr_le_t *)data);
    if (conn)
    {
	err = bt_l2cap_chan_disconnect(&l2ch_chan.ch.chan);
	if (err) {
		ncp_e("Unable to disconnect: %u\n", -err);
	}
        bt_conn_unref(conn);
        status = err < 0 ? NCP_CMD_RESULT_ERROR : NCP_CMD_RESULT_OK;
    }

    ble_prepare_status(NCP_RSP_BLE_L2CAP_DISCONNECT, status, NULL, 0);
}

/*
 * @brief   Send data over L2CAP channel
 */
void ble_l2cap_send_data(uint8_t *data, uint16_t len)
{
    const struct l2cap_send_data_cmd_tag *cmd = (void *) data;
    struct bt_conn *conn;
    uint8_t status = NCP_CMD_RESULT_ERROR;
    static uint8_t buf_data[DATA_MTU] = { [0 ... (DATA_MTU - 1)] = 0xff };
    int ret, length = DATA_MTU, count = 1;
    struct net_buf *buf;

    conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, (bt_addr_le_t *)data);
    if (conn)
    {

	count = cmd->times;

	length = MIN(l2ch_chan.ch.tx.mtu, len);

	while (count--) {
		ncp_d("Rem %d\n", count);
		buf = net_buf_alloc(&data_tx_pool, BT_SECONDS(2));
		if (!buf) {
			if (l2ch_chan.ch.state != BT_L2CAP_CONNECTED) {
				ncp_e("Channel disconnected, stopping TX\n");
                                goto unref;
			}
			ncp_e("Allocation timeout, stopping TX\n");
                        goto unref;
		}
		net_buf_reserve(buf, BT_L2CAP_SDU_CHAN_SEND_RESERVE);

		net_buf_add_mem(buf, buf_data, length);
		ret = bt_l2cap_chan_send(&l2ch_chan.ch.chan, buf);
		if (ret < 0) {
			ncp_e("Unable to send: %d\n", -ret);
			net_buf_unref(buf);
		}
                status = ret < 0 ? NCP_CMD_RESULT_ERROR : NCP_CMD_RESULT_OK;
	}
    }
    else
    {
        LOG_ERR("Unknown connection");
        status = NCP_CMD_RESULT_ERROR;
        goto sta;
    }

unref:
    bt_conn_unref(conn);
sta:
    ble_prepare_status(NCP_RSP_BLE_L2CAP_SEND, status, NULL, 0);
}

int ble_ncp_L2capInit(void)
{
    int ret = NCP_CMD_RESULT_OK;

    k_fifo_init(&l2cap_recv_fifo);
    return (int)ret;
}

#endif /* CONFIG_NCP_BLE */