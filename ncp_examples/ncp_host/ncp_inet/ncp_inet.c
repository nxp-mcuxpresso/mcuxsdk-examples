/** @file ncp_inet.c
 *
 *  Copyright 2024-2025 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "ncp_cmd_node.h"
#include <errno.h>
#include <signal.h>
#include "ncp_host_command.h"
#include <ncp_adapter.h>
#include <ncp_wifi_api.h>
#include <errno.h>
#include "ncp_inet.h"
#include "ncp_host_utils_wifi.h"
#include "stream_buffer.h"


static int inet_socket_recv_queue_open(int socket, int socket_type);
static void inet_socket_recv_queue_close(int socket);
static struct ncp_socket_recv_t *inet_get_sock_handle(int socket);
static int inet_sock_recv_queue_init(void);
static void inet_sock_recv_queue_deinit(void);

static void ncp_inet_socket_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_INET_COMMAND *cmd_res = (NCPCmd_DS_INET_COMMAND *)res;
    NCP_CMD_INET_RESP_SOCKET_CFG *inet_socket_resp = &cmd_res->params.wlan_inet_resp_socket;

    memcpy(cmd_node->resp_buf, (char *)inet_socket_resp, sizeof(NCP_CMD_INET_RESP_SOCKET_CFG));
}

int ncp_socket(int family, int style, int protocol)
{
    int socket;
    int ret = 0;
    int socket_type = IPPROTO_NONE;
	NCP_CMD_INET_RESP_SOCKET_CFG *inet_socket_resp = NULL;
    if (style & SOCK_STREAM)
        socket_type = IPPROTO_TCP;
    else if (style & SOCK_DGRAM)
        socket_type = IPPROTO_UDP;
    else if (style & SOCK_RAW)
        socket_type = IPPROTO_RAW;

    NCP_CMD_INET_RESP_SOCKET_CFG *cmd_resp_buf =  OSA_MemoryAllocate(sizeof(NCP_CMD_INET_RESP_SOCKET_CFG));
    if(cmd_resp_buf == NULL)
    {
        ncp_adap_e("failed to malloc cmd_resp_buf!\r\n");
        return -WM_FAIL;
    }

    (void) memset((uint8_t *)cmd_resp_buf, 0, sizeof(NCP_CMD_INET_RESP_SOCKET_CFG));

    NCPCmd_DS_INET_COMMAND *cmd = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_SOCKET_CFG));
    if(cmd == NULL)
    {
        ncp_adap_e("failed to malloc cmd buff.\r\n");
        free(cmd_resp_buf);
        return -WM_FAIL;
    }
    (void)memset((uint8_t *) cmd, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_SOCKET_CFG));
    
    cmd->header.cmd    = NCP_CMD_WLAN_INET_SOCKET;
    cmd->header.size   = NCP_CMD_HEADER_LEN;
    cmd->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_INET_SOCKET_CFG *inet_socket = (NCP_CMD_INET_SOCKET_CFG *)&cmd->params.wlan_inet_socket;

    inet_socket->family   = family;
    inet_socket->style    = style;
    inet_socket->protocol = protocol;

    cmd->header.size += sizeof(NCP_CMD_INET_SOCKET_CFG);

    ret = ncp_tlv_send_wait_resp(cmd, cmd_resp_buf, ncp_inet_socket_cb);
    if (ret < 0)
        goto exit;

    inet_socket_resp = (NCP_CMD_INET_RESP_SOCKET_CFG *)cmd_resp_buf;
    socket = inet_socket_resp->ret;
    errno = inet_socket_resp->errorn;

    if (socket >= 0)
    {
        ret = inet_socket_recv_queue_open(socket, socket_type);
        if (ret < 0)
            ncp_adap_e("create ncp socket receive queue for socket %d fail", socket);
    }
    ncp_adap_d("create ncp socket receive queue for socket %d", socket);

exit:
    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(cmd);
    if (socket < 0)
        return socket;
    else
        return socketfd_lwip_to_host(socket);
}

static void ncp_inet_bind_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_INET_COMMAND *cmd_res = (NCPCmd_DS_INET_COMMAND *)res;
    NCP_CMD_INET_RESP_BIND_CFG *inet_bind_resp = &cmd_res->params.wlan_inet_resp_bind;

    memcpy(cmd_node->resp_buf, (char *)inet_bind_resp, sizeof(NCP_CMD_INET_RESP_BIND_CFG));
}

int ncp_bind(int socket, const struct sockaddr *addr, socklen_t length)
{
    int ret = 0;
	NCP_CMD_INET_RESP_BIND_CFG *inet_bind_resp = NULL;
    socket = socketfd_host_to_lwip(socket);
    NCP_CMD_INET_RESP_BIND_CFG *cmd_resp_buf = OSA_MemoryAllocate(sizeof(NCP_CMD_INET_RESP_BIND_CFG));
    if(cmd_resp_buf == NULL)
    {
        ncp_adap_e("failed to malloc cmd_resp_buf!\r\n");
        return -WM_FAIL;
    }
    (void)memset((uint8_t *)cmd_resp_buf, 0, sizeof(NCP_CMD_INET_RESP_BIND_CFG));

    NCPCmd_DS_INET_COMMAND *cmd = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_BIND_CFG));
    if(cmd == NULL)
    {
        ncp_adap_e("failed to malloc cmd buff.\r\n");
        free(cmd_resp_buf);
        return -WM_FAIL;
    }
    (void) memset((uint8_t *) cmd, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_BIND_CFG));
    
    cmd->header.cmd    = NCP_CMD_WLAN_INET_BIND;
    cmd->header.size   = NCP_CMD_HEADER_LEN;
    cmd->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_INET_BIND_CFG *inet_bind = (NCP_CMD_INET_BIND_CFG *) &cmd->params.wlan_inet_bind;

    inet_bind->socket   = socket;
    inet_bind->socklen    = length;
    memcpy(inet_bind->sockaddr, (char *)addr, length);
    cmd->header.size += sizeof(NCP_CMD_INET_BIND_CFG);

    ret = ncp_tlv_send_wait_resp(cmd, cmd_resp_buf, ncp_inet_bind_cb);
    if (ret < 0)
        goto exit;

    inet_bind_resp = (NCP_CMD_INET_RESP_BIND_CFG *)cmd_resp_buf;
    ret   =  inet_bind_resp->ret;
    errno = inet_bind_resp->errorn;

exit:
    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(cmd);
    return ret;
}

static void ncp_inet_con_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_INET_COMMAND *cmd_res = (NCPCmd_DS_INET_COMMAND *)res;
    NCP_CMD_INET_RESP_CON_CFG *inet_con_resp = &cmd_res->params.wlan_inet_resp_con;

    memcpy(cmd_node->resp_buf, (char *)inet_con_resp, sizeof(NCP_CMD_INET_RESP_CON_CFG));
}

int ncp_connect(int socket, const struct sockaddr *addr, socklen_t length)
{
    int ret = 0;
	NCP_CMD_INET_RESP_CON_CFG *inet_con_resp = NULL;
    socket = socketfd_host_to_lwip(socket);
    NCP_CMD_INET_RESP_CON_CFG *cmd_resp_buf = OSA_MemoryAllocate(sizeof(NCP_CMD_INET_RESP_CON_CFG));
    if(cmd_resp_buf == NULL)
    {
        ncp_adap_e("failed to malloc cmd_resp_buf!\r\n");
        return -WM_FAIL;
    }
    (void)memset((uint8_t *)cmd_resp_buf, 0, sizeof(NCP_CMD_INET_RESP_CON_CFG));

    NCPCmd_DS_INET_COMMAND *cmd = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_CON_CFG));
    if(cmd == NULL)
    {
        ncp_adap_e("failed to malloc cmd buff.\r\n");
        free(cmd_resp_buf);
        return -WM_FAIL;
    }
    (void) memset((uint8_t *) cmd, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_CON_CFG));

    cmd->header.cmd    = NCP_CMD_WLAN_INET_CON;
    cmd->header.size   = NCP_CMD_HEADER_LEN;
    cmd->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_INET_CON_CFG * inet_con = (NCP_CMD_INET_CON_CFG *) &cmd->params.wlan_inet_con;

    inet_con->socket   = socket;
    inet_con->socklen    = length;
    memcpy(inet_con->sockaddr, (char *)addr, length);
    cmd->header.size += sizeof(NCP_CMD_INET_CON_CFG);

    ret = ncp_tlv_send_wait_resp(cmd, cmd_resp_buf, ncp_inet_con_cb);
    if (ret < 0)
        goto exit;

    inet_con_resp = (NCP_CMD_INET_RESP_CON_CFG *)cmd_resp_buf;
    ret   =  inet_con_resp->ret;
    errno = inet_con_resp->errorn;
    if (ret > 0)
    {
        struct ncp_socket_recv_t *handle = inet_get_sock_handle(ret);
        if (!handle || handle->socket == -1)
        {
            ncp_adap_e("%s-%d, socket %d already closed\n", __func__, __LINE__, ret);
        }
        handle->socklen = length;
        memcpy((char *)&handle->addr, (char *)addr, length);
    }

exit:
    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(cmd);
    return ret;
}

static void ncp_inet_listen_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_INET_COMMAND *cmd_res = (NCPCmd_DS_INET_COMMAND *)res;
    NCP_CMD_INET_RESP_LISTEN_CFG *inet_listen_resp = &cmd_res->params.wlan_inet_resp_listen;

    memcpy(cmd_node->resp_buf, (char *)inet_listen_resp, sizeof(NCP_CMD_INET_RESP_LISTEN_CFG));
}

int ncp_listen(int socket, int number)
{
    int ret = 0;
	NCP_CMD_INET_RESP_LISTEN_CFG *inet_listen_resp = NULL;
    socket = socketfd_host_to_lwip(socket);
    NCP_CMD_INET_RESP_LISTEN_CFG * cmd_resp_buf = OSA_MemoryAllocate(sizeof(NCP_CMD_INET_RESP_LISTEN_CFG));
    if(cmd_resp_buf == NULL)
    {
        ncp_adap_e("failed to malloc cmd_resp_buf!\r\n");
        return -WM_FAIL;
    }
    (void)memset((uint8_t *)cmd_resp_buf, 0, sizeof(NCP_CMD_INET_RESP_LISTEN_CFG));

    NCPCmd_DS_INET_COMMAND *cmd = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_LISTEN_CFG));
    if(cmd == NULL)
    {
        ncp_adap_e("failed to malloc cmd buff.\r\n");
        free(cmd_resp_buf);
        return -WM_FAIL;
    }
    (void) memset((uint8_t *)cmd, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_LISTEN_CFG));
    
    cmd->header.cmd    = NCP_CMD_WLAN_INET_LISTEN;
    cmd->header.size   = NCP_CMD_HEADER_LEN;
    cmd->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_INET_LISTEN_CFG *inet_listen = (NCP_CMD_INET_LISTEN_CFG *) &cmd->params.wlan_inet_listen;

    inet_listen->socket   = socket;
    inet_listen->number   = number;
    cmd->header.size += sizeof(NCP_CMD_INET_LISTEN_CFG);

    ret = ncp_tlv_send_wait_resp(cmd, cmd_resp_buf, ncp_inet_listen_cb);
    if (ret < 0)
        goto exit;

    inet_listen_resp = (NCP_CMD_INET_RESP_LISTEN_CFG *)cmd_resp_buf;
    ret   =  inet_listen_resp->ret;
    errno = inet_listen_resp->errorn;

exit:
    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(cmd);
    return ret;
}

static void ncp_inet_accept_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_INET_COMMAND *cmd_res = (NCPCmd_DS_INET_COMMAND*) res;
    NCP_CMD_INET_RESP_ACCEPT_CFG *inet_accept_resp = &cmd_res->params.wlan_inet_resp_accept;

    memcpy(cmd_node->resp_buf, (char *)inet_accept_resp, sizeof(NCP_CMD_INET_RESP_ACCEPT_CFG));
}

int ncp_accept(int socket, struct sockaddr *addr, socklen_t *length)
{
    int ret = 0;
	NCP_CMD_INET_RESP_ACCEPT_CFG *inet_accept_resp = NULL;
    socket = socketfd_host_to_lwip(socket);

    int accept_socket = 0;
    NCP_CMD_INET_RESP_ACCEPT_CFG *cmd_resp_buf = OSA_MemoryAllocate(sizeof(NCP_CMD_INET_RESP_ACCEPT_CFG));
    if(cmd_resp_buf == NULL)
    {
        ncp_adap_e("failed to malloc cmd_resp_buf!\r\n");
        return -WM_FAIL;
    }
    (void)memset((uint8_t *)cmd_resp_buf, 0, sizeof(NCP_CMD_INET_RESP_ACCEPT_CFG));

    NCPCmd_DS_INET_COMMAND *cmd = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_ACCEPT_CFG));
    if(cmd == NULL)
    {
        ncp_adap_e("failed to malloc cmd buff.\r\n");
        free(cmd_resp_buf);
        return -WM_FAIL;
    }
    (void) memset((uint8_t *)cmd, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_ACCEPT_CFG));
    
    cmd->header.cmd    = NCP_CMD_WLAN_INET_ACCEPT;
    cmd->header.size   = NCP_CMD_HEADER_LEN;
    cmd->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_INET_ACCEPT_CFG *inet_accept = (NCP_CMD_INET_ACCEPT_CFG *) &cmd->params.wlan_inet_accept;

    inet_accept->socket   = socket;
    cmd->header.size += sizeof(NCP_CMD_INET_ACCEPT_CFG);

    ret = ncp_tlv_send_wait_resp(cmd, cmd_resp_buf, ncp_inet_accept_cb);
    if (ret < 0)
        goto exit;

    inet_accept_resp = (NCP_CMD_INET_RESP_ACCEPT_CFG *)cmd_resp_buf;
    ret           =  inet_accept_resp->ret;
    accept_socket = ret;
    errno = inet_accept_resp->errorn;
    *length       = inet_accept_resp->socklen;
    memcpy((char *)addr, inet_accept_resp->sockaddr, *length);
    if (accept_socket >= 0)
    {
        ret = inet_socket_recv_queue_open(accept_socket, IPPROTO_TCP);
        if (ret < 0)
            ncp_adap_e("create ncp socket receive queue for socket %d fail", accept_socket);
        else
            ret = socketfd_lwip_to_host(accept_socket);
    }

exit:
    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(cmd);
    return ret;
}

static void ncp_inet_close_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_INET_COMMAND *cmd_res = (NCPCmd_DS_INET_COMMAND*) res;
    NCP_CMD_INET_RESP_CLOSE_CFG *inet_close_resp = &cmd_res->params.wlan_inet_resp_close;

    memcpy(cmd_node->resp_buf, (char *)inet_close_resp, sizeof(NCP_CMD_INET_RESP_CLOSE_CFG));
}

int ncp_close(int socket)
{
    int ret = 0;
	NCP_CMD_INET_RESP_CLOSE_CFG *inet_close_resp = NULL;
    socket = socketfd_host_to_lwip(socket);
    NCP_CMD_INET_RESP_CLOSE_CFG *cmd_resp_buf = OSA_MemoryAllocate(sizeof(NCP_CMD_INET_RESP_CLOSE_CFG));
    if(cmd_resp_buf == NULL)
    {
        ncp_adap_e("failed to malloc cmd_resp_buf!\r\n");
        return -WM_FAIL;
    }
    (void)memset((uint8_t *)cmd_resp_buf, 0, sizeof(NCP_CMD_INET_RESP_CLOSE_CFG));

    NCPCmd_DS_INET_COMMAND *cmd = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_CLOSE_CFG));
    if(cmd == NULL)
    {
        ncp_adap_e("failed to malloc cmd buff.\r\n");
        free(cmd_resp_buf);
        return -WM_FAIL;
    }
    (void) memset((uint8_t *)cmd, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_CLOSE_CFG));
    
    cmd->header.cmd    = NCP_CMD_WLAN_INET_CLOSE;
    cmd->header.size   = NCP_CMD_HEADER_LEN;
    cmd->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_INET_CLOSE_CFG *inet_close = (NCP_CMD_INET_CLOSE_CFG *) &cmd->params.wlan_inet_close;

    inet_close->socket   = socket;
    cmd->header.size += sizeof(NCP_CMD_INET_CLOSE_CFG);

    ret = ncp_tlv_send_wait_resp(cmd, cmd_resp_buf, ncp_inet_close_cb);
    if (ret < 0)
        goto exit;

    inet_close_resp = (NCP_CMD_INET_RESP_CLOSE_CFG *)cmd_resp_buf;
    ret   =  inet_close_resp->ret;
    errno = inet_close_resp->errorn;
    inet_socket_recv_queue_close(socket);

exit:
    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(cmd);
    return ret;
}

static void ncp_inet_shutdown_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_INET_COMMAND *cmd_res = (NCPCmd_DS_INET_COMMAND*) res;
    NCP_CMD_INET_RESP_SHUTDOWN_CFG *inet_shutdown_resp = &cmd_res->params.wlan_inet_resp_shutdown;

    memcpy(cmd_node->resp_buf, (char *)inet_shutdown_resp, sizeof(NCP_CMD_INET_RESP_SHUTDOWN_CFG));
}

int ncp_shutdown(int socket, int how)
{
    int ret = 0;
	NCP_CMD_INET_RESP_SHUTDOWN_CFG *inet_shutdown_resp = NULL;
    socket = socketfd_host_to_lwip(socket);
    NCP_CMD_INET_RESP_SHUTDOWN_CFG *cmd_resp_buf = OSA_MemoryAllocate(sizeof(NCP_CMD_INET_RESP_SHUTDOWN_CFG));
    if(cmd_resp_buf == NULL)
    {
        ncp_adap_e("failed to malloc cmd_resp_buf!\r\n");
        return -WM_FAIL;
    }
    (void)memset((uint8_t *)cmd_resp_buf, 0, sizeof(NCP_CMD_INET_RESP_SHUTDOWN_CFG));

    NCPCmd_DS_INET_COMMAND *cmd = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_SHUTDOWN_CFG));
    if(cmd == NULL)
    {
        ncp_adap_e("failed to malloc cmd buff.\r\n");
        free(cmd_resp_buf);
        return -WM_FAIL;
    }
    (void) memset((uint8_t *)cmd, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_SHUTDOWN_CFG));

    cmd->header.cmd    = NCP_CMD_WLAN_INET_SHUTDOWN;
    cmd->header.size   = NCP_CMD_HEADER_LEN;
    cmd->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_INET_SHUTDOWN_CFG *inet_shutdown = (NCP_CMD_INET_SHUTDOWN_CFG *) &cmd->params.wlan_inet_shutdown;

    inet_shutdown->socket   = socket;
    cmd->header.size += sizeof(NCP_CMD_INET_SHUTDOWN_CFG);

    ret = ncp_tlv_send_wait_resp(cmd, cmd_resp_buf, ncp_inet_shutdown_cb);
    if (ret < 0)
        goto exit;

    inet_shutdown_resp = (NCP_CMD_INET_RESP_SHUTDOWN_CFG *)cmd_resp_buf;
    ret   =  inet_shutdown_resp->ret;
    errno = inet_shutdown_resp->errorn;
    inet_socket_recv_queue_close(socket);

exit:
    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(cmd);
    return ret;
}

static void ncp_inet_getsockname_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_INET_COMMAND *cmd_res = (NCPCmd_DS_INET_COMMAND*) res;
    NCP_CMD_INET_RESP_GETSOCKNAME_CFG *inet_getsockname_resp = &cmd_res->params.wlan_inet_resp_getsockname;

    memcpy(cmd_node->resp_buf, (char *)inet_getsockname_resp, sizeof(NCP_CMD_INET_RESP_GETSOCKNAME_CFG));
}

int ncp_getsockname(int socket, struct sockaddr *addr, socklen_t *length)
{
    int ret = 0;
	NCP_CMD_INET_RESP_GETSOCKNAME_CFG *inet_getsockname_resp = NULL;
    socket = socketfd_host_to_lwip(socket);
    NCP_CMD_INET_RESP_GETSOCKNAME_CFG *cmd_resp_buf = OSA_MemoryAllocate(sizeof(NCP_CMD_INET_RESP_GETSOCKNAME_CFG));
    if(cmd_resp_buf == NULL)
    {
        ncp_adap_e("failed to malloc cmd_resp_buf!\r\n");
        return -WM_FAIL;
    }
    (void)memset((uint8_t *)cmd_resp_buf, 0, sizeof(NCP_CMD_INET_RESP_GETSOCKNAME_CFG));

    NCPCmd_DS_INET_COMMAND *cmd = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_GETSOCKNAME_CFG));
    if(cmd == NULL)
    {
        ncp_adap_e("failed to malloc cmd buff.\r\n");
        free(cmd_resp_buf);
        return -WM_FAIL;
    }
    (void) memset((uint8_t *)cmd, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_GETSOCKNAME_CFG));
    
    cmd->header.cmd    = NCP_CMD_WLAN_INET_GETSOCKNAME;
    cmd->header.size   = NCP_CMD_HEADER_LEN;
    cmd->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_INET_GETSOCKNAME_CFG *inet_getsockname = (NCP_CMD_INET_GETSOCKNAME_CFG *) &cmd->params.wlan_inet_getsockname;

    inet_getsockname->socket   = socket;
    cmd->header.size += sizeof(NCP_CMD_INET_GETSOCKNAME_CFG);

    ret = ncp_tlv_send_wait_resp(cmd, cmd_resp_buf, ncp_inet_getsockname_cb);
    if (ret < 0)
        goto exit;

    inet_getsockname_resp = (NCP_CMD_INET_RESP_GETSOCKNAME_CFG *)cmd_resp_buf;
    ret   =  inet_getsockname_resp->ret;
    errno = inet_getsockname_resp->errorn;
    *length = inet_getsockname_resp->socklen;
    memcpy((char *)addr, inet_getsockname_resp->sockaddr, *length);

exit:
    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(cmd);
    return ret;
}

static void ncp_inet_getpeername_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_INET_COMMAND *cmd_res = (NCPCmd_DS_INET_COMMAND*) res;
    NCP_CMD_INET_RESP_GETPEERNAME_CFG *inet_getpeername_resp = &cmd_res->params.wlan_inet_resp_getpeername;

    memcpy(cmd_node->resp_buf, (char *)inet_getpeername_resp, sizeof(NCP_CMD_INET_RESP_GETPEERNAME_CFG));
}

int ncp_getpeername(int socket, struct sockaddr *addr, socklen_t *length)
{
    int ret = 0;
	NCP_CMD_INET_RESP_GETPEERNAME_CFG *inet_getpeername_resp = NULL;
    socket = socketfd_host_to_lwip(socket);

    NCP_CMD_INET_RESP_GETPEERNAME_CFG *cmd_resp_buf = OSA_MemoryAllocate(sizeof(NCP_CMD_INET_RESP_GETPEERNAME_CFG));
    if(cmd_resp_buf == NULL)
    {
        ncp_adap_e("failed to malloc cmd_resp_buf!\r\n");
        return -WM_FAIL;
    }
    (void)memset((uint8_t *)cmd_resp_buf, 0, sizeof(NCP_CMD_INET_RESP_GETPEERNAME_CFG));

    NCPCmd_DS_INET_COMMAND *cmd = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_GETPEERNAME_CFG));
    if(cmd == NULL)
    {
        ncp_adap_e("failed to malloc cmd buff.\r\n");
        free(cmd_resp_buf);
        return -WM_FAIL;
    }
    (void) memset((uint8_t *)cmd, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_GETPEERNAME_CFG));
    
    cmd->header.cmd    = NCP_CMD_WLAN_INET_GETPEERNAME;
    cmd->header.size   = NCP_CMD_HEADER_LEN;
    cmd->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_INET_GETPEERNAME_CFG *inet_getpeername = (NCP_CMD_INET_GETPEERNAME_CFG *) &cmd->params.wlan_inet_getpeername;

    inet_getpeername->socket   = socket;
    cmd->header.size += sizeof(NCP_CMD_INET_GETPEERNAME_CFG);

    ret = ncp_tlv_send_wait_resp(cmd, cmd_resp_buf, ncp_inet_getpeername_cb);
    if (ret < 0)
        goto exit;

    inet_getpeername_resp = (NCP_CMD_INET_RESP_GETPEERNAME_CFG *)cmd_resp_buf;
    ret   =  inet_getpeername_resp->ret;
    errno = inet_getpeername_resp->errorn;
    *length = inet_getpeername_resp->socklen;
    memcpy((char *)addr, inet_getpeername_resp->sockaddr, *length);

exit:
    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(cmd);
    return ret;
}

static void ncp_inet_getsockopt_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_INET_COMMAND *cmd_res = (NCPCmd_DS_INET_COMMAND*) res;
    NCP_CMD_INET_RESP_GETSOCKOPT_CFG *inet_getsockopt_resp = &cmd_res->params.wlan_inet_resp_getsockopt;

    memcpy(cmd_node->resp_buf, (char *)inet_getsockopt_resp, sizeof(NCP_CMD_INET_RESP_GETSOCKOPT_CFG));
}

int ncp_getsockopt(int socket, int level, int optname, void *optval, socklen_t *optlen)
{
    int ret = 0;
	NCP_CMD_INET_RESP_GETSOCKOPT_CFG *inet_getsockopt_resp = NULL;
    socket = socketfd_host_to_lwip(socket);

    NCP_CMD_INET_RESP_GETSOCKOPT_CFG *cmd_resp_buf = OSA_MemoryAllocate(sizeof(NCP_CMD_INET_RESP_GETSOCKOPT_CFG));
    if(cmd_resp_buf == NULL)
    {
        ncp_adap_e("failed to malloc cmd_resp_buf!\r\n");
        return -WM_FAIL;
    }
    (void)memset((uint8_t *)cmd_resp_buf, 0, sizeof(NCP_CMD_INET_RESP_GETSOCKOPT_CFG));

    NCPCmd_DS_INET_COMMAND *cmd = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_GETSOCKOPT_CFG));
    if(cmd == NULL)
    {
        ncp_adap_e("failed to malloc cmd buff.\r\n");
        free(cmd_resp_buf);
        return -WM_FAIL;
    }
    (void) memset((uint8_t *)cmd, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_GETSOCKOPT_CFG));
    
    cmd->header.cmd    = NCP_CMD_WLAN_INET_GETSOCKOPT;
    cmd->header.size   = NCP_CMD_HEADER_LEN;
    cmd->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_INET_GETSOCKOPT_CFG * inet_getsockopt = (NCP_CMD_INET_GETSOCKOPT_CFG *) &cmd->params.wlan_inet_getsockopt;

    inet_getsockopt->socket   = socket;
    inet_getsockopt->level    = level;
    inet_getsockopt->optname  = optname;
    cmd->header.size += sizeof(NCP_CMD_INET_GETSOCKOPT_CFG);

    ret = ncp_tlv_send_wait_resp(cmd, cmd_resp_buf, ncp_inet_getsockopt_cb);
    if (ret < 0)
        goto exit;

    inet_getsockopt_resp = (NCP_CMD_INET_RESP_GETSOCKOPT_CFG *)cmd_resp_buf;
    ret   =  inet_getsockopt_resp->ret;
    errno = inet_getsockopt_resp->errorn;
    *optlen = inet_getsockopt_resp->socklen;
    memcpy((char *)optval, inet_getsockopt_resp->optval, *optlen);

exit:
    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(cmd);
    return ret;
}

static void ncp_inet_setsockopt_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_INET_COMMAND *cmd_res = (NCPCmd_DS_INET_COMMAND *)res;
    NCP_CMD_INET_RESP_SETSOCKOPT_CFG *inet_setsockopt_resp = &cmd_res->params.wlan_inet_resp_setsockopt;

    memcpy(cmd_node->resp_buf, (char *)inet_setsockopt_resp, sizeof(NCP_CMD_INET_RESP_SETSOCKOPT_CFG));
}

int ncp_setsockopt(int socket, int level, int optname, const void *optval, socklen_t optlen)
{
    int ret = 0;
    NCP_CMD_INET_RESP_SETSOCKOPT_CFG *inet_setsockopt_resp = NULL;
    socket = socketfd_host_to_lwip(socket);

    struct ncp_socket_recv_t *handle = inet_get_sock_handle(socket);
    if (!handle)
        return -1;
    
    if (level == SOL_SOCKET && optname == SO_RCVTIMEO)
    {
        struct timestamp *tv = (struct timestamp *)optval;
        handle->recv_timeout_ms = tv->tv_sec * 1000 + tv->tv_usec / 1000;
    }

    NCP_CMD_INET_RESP_SETSOCKOPT_CFG *cmd_resp_buf = OSA_MemoryAllocate(sizeof(NCP_CMD_INET_RESP_SETSOCKOPT_CFG));
    if(cmd_resp_buf == NULL)
    {
        ncp_adap_e("failed to malloc cmd_resp_buf!\r\n");
        return -WM_FAIL;
    }
    (void)memset((uint8_t *)cmd_resp_buf, 0, sizeof(NCP_CMD_INET_RESP_SETSOCKOPT_CFG));

    NCPCmd_DS_INET_COMMAND *cmd = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_SETSOCKOPT_CFG));
    if(cmd == NULL)
    {
        ncp_adap_e("failed to malloc cmd buff.\r\n");
        free(cmd_resp_buf);
        return -WM_FAIL;
    }
    (void) memset((uint8_t *)cmd, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_SETSOCKOPT_CFG));
    
    cmd->header.cmd    = NCP_CMD_WLAN_INET_SETSOCKOPT;
    cmd->header.size   = NCP_CMD_HEADER_LEN;
    cmd->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_INET_SETSOCKOPT_CFG * inet_setsockopt = (NCP_CMD_INET_SETSOCKOPT_CFG *) &cmd->params.wlan_inet_setsockopt;

    inet_setsockopt->socket   = socket;
    inet_setsockopt->level    = level;
    inet_setsockopt->optname  = optname;
    inet_setsockopt->socklen   = optlen;
    memcpy(inet_setsockopt->optval, optval, optlen);
    cmd->header.size += sizeof(NCP_CMD_INET_SETSOCKOPT_CFG);

    ret = ncp_tlv_send_wait_resp(cmd, cmd_resp_buf, ncp_inet_setsockopt_cb);
    if (ret < 0)
        goto exit;

    inet_setsockopt_resp = (NCP_CMD_INET_RESP_SETSOCKOPT_CFG *)cmd_resp_buf;
    ret   =  inet_setsockopt_resp->ret;
    errno = inet_setsockopt_resp->errorn;

exit:
    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(cmd);
    return ret;
}

/* socket send data fail event */
int inet_sock_send_fail_event(uint8_t *res)
{	
    NCP_CMD_INET_RESP_SEND_CFG *inet_sock_recv_tlv = 0;

    NCPCmd_DS_INET_COMMAND *evt_res = (NCPCmd_DS_INET_COMMAND *)res;
    if (evt_res->header.result != NCP_CMD_RESULT_OK)
    {
        return -WM_FAIL;
    }
    inet_sock_recv_tlv = (NCP_CMD_INET_RESP_SEND_CFG *)&evt_res->params.wlan_inet_resp_send;
    /* socket handle */
    int socket = inet_sock_recv_tlv->socket;
    int ret = inet_sock_recv_tlv->ret;
    int errorno = inet_sock_recv_tlv->errorn;

    ncp_adap_e("socket[%d] send fail ret[%d], errno[%d]", socket, ret, errorno);
    //	inet_socket_recv_queue_close(socket);
    struct ncp_socket_recv_t *handle = inet_get_sock_handle(socket);
    if (!handle || handle->socket == -1)
    {
        return WM_SUCCESS;
    }
    handle->write_errorn = errorno;
    return WM_SUCCESS;
}

static int ncp_common_send(int socket, const void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t length)
{
    int ret = 0;
    struct ncp_socket_recv_t *handle = inet_get_sock_handle(socket);
    if (!handle || handle->socket == -1)
    {
        errno = ENOTCONN;
        return -1;
    }

    int cmd_size = sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_SENDTO_CFG) + size;
    if (cmd_size > NCP_COMMAND_LEN)
    {
        ncp_adap_e("send data size is too large\r\n");
        return -WM_FAIL;
    }
    NCPCmd_DS_INET_COMMAND *cmd = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_SENDTO_CFG) + size);
    if(cmd == NULL)
    {
        ncp_adap_e("failed to malloc cmd buff.\r\n");
        return -WM_FAIL;
    }
    (void) memset((uint8_t *)cmd, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_SENDTO_CFG));
    
    cmd->header.cmd    = NCP_CMD_WLAN_INET_SENDTO;
    cmd->header.size   = NCP_CMD_HEADER_LEN;
    cmd->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_INET_SENDTO_CFG *inet_sendto = (NCP_CMD_INET_SENDTO_CFG *) &cmd->params.wlan_inet_sendto;

    inet_sendto->socket    = socket;
    inet_sendto->size      = size;
    inet_sendto->flags     = flags;
    inet_sendto->socklen   = length;
    memcpy(inet_sendto->send_data, buffer, size);

    if (!length)
        memset(inet_sendto->sockaddr, '\0', sizeof(inet_sendto->sockaddr));
    else
        memcpy((char *)inet_sendto->sockaddr, (char *)addr, length);

    cmd->header.size += sizeof(NCP_CMD_INET_SENDTO_CFG);
    cmd->header.size += size;
    ncp_adap_d("send data %d size to lwip\n", size);
    ret = ncp_tlv_send_no_resp(cmd);
    OSA_MemoryFree(cmd);
    if (ret == 0)
        return size;
    else
        return ret;
}

int ncp_send(int socket, const void *buffer, size_t size, int flags)
{
    socket = socketfd_host_to_lwip(socket);
    return ncp_common_send(socket, buffer, size, flags, 0, 0);
}

int ncp_sendto(int socket, const void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t length)
{
    socket = socketfd_host_to_lwip(socket);
    return ncp_common_send(socket, buffer, size, flags, addr, length);
}

/* ncp socket enhance receive queue */
void stop_ncp_host(int signo)
{
    inet_sock_recv_queue_init();
}

/* ncp inet data path RX */
static struct ncp_socket_recv_t inet_sock_handle_array[NCP_INET_SOCK_NUMBER];
static int inet_sock_recv_queue_init(void)
{
    struct ncp_socket_recv_t *handle;
    for (int i = 0; i < NCP_INET_SOCK_NUMBER; i++)
    {
        handle = &inet_sock_handle_array[i];
        handle->socket = -1;
        handle->rx_fifo_fd = NULL;
        handle->write_errorn = 0;
        memset(handle->name, '\0', sizeof(handle->name));
        handle->socket_type = IPPROTO_NONE;
    }
    return WM_SUCCESS;
}

static void inet_sock_recv_queue_deinit(void)
{
    struct ncp_socket_recv_t *handle;
    for (int i = 0; i < NCP_INET_SOCK_NUMBER; i++)
    {
        handle = &inet_sock_handle_array[i];
        handle->socket = -1;
        if (!handle->rx_fifo_fd)
        {
            vStreamBufferDelete(handle->rx_fifo_fd);
            handle->rx_fifo_fd = NULL;
            memset(handle->name, '\0', sizeof(handle->name));
        }
        handle->socket_type = IPPROTO_NONE;
        handle->write_errorn = 0;
    }
}

static int inet_socket_recv_queue_open(int socket, int socket_type)
{
    char inet_sock_recv_name[64];
    struct ncp_socket_recv_t *handle = &inet_sock_handle_array[socket];

    if (handle->rx_fifo_fd)
        goto created;

    sprintf(inet_sock_recv_name, "%s-%d", INET_SOCKET_RECV_QUEUE_NAME, handle->socket);
    handle->rx_fifo_fd = xStreamBufferCreate(STREAM_BUFFER_SIZE, 1);
    if (!handle->rx_fifo_fd)
    {
        perror("open: ");
        goto err;
    }
    memcpy(handle->name, inet_sock_recv_name, sizeof(handle->name));

created:
    ncp_adap_d("create ncp socket receive queue %d for socket %d", handle->rx_fifo_fd, socket);
    handle->socket = socket;
    handle->socket_type = socket_type;
    handle->recv_timeout_ms = 0;
    return WM_SUCCESS;
err:
    return -WM_FAIL;
}

static void inet_socket_recv_queue_close(int socket)
{
    struct ncp_socket_recv_t *handle = &inet_sock_handle_array[socket];

    if (!handle->rx_fifo_fd)
    {
        ncp_adap_e("ERROR: ncp socket receive queue has been release");
        return ;
    }

    vStreamBufferDelete(handle->rx_fifo_fd);
    handle->rx_fifo_fd = 0;
    handle->socket = -1;
    handle->socket_type = IPPROTO_NONE;	
    handle->write_errorn = 0;
    handle->read_errorn = 0;
    handle->recv_timeout_ms = 0;
    memset(handle->name, '\0', sizeof(handle->name));
}

void ncp_inet_init()
{
    inet_sock_recv_queue_init();
}

void ncp_inet_deinit()
{
    inet_sock_recv_queue_deinit();
}

static struct ncp_socket_recv_t *inet_get_sock_handle(int socket)
{
    struct ncp_socket_recv_t *handle = &inet_sock_handle_array[socket];
    if (handle->socket == -1)
    {
        return NULL;
    }
    return handle;
}

/* enqueue receive data from bus to queue */
int inet_sock_recv_send_queue_data(int socket, char *buf, int size)
{
    int32_t ret = 0;

    struct ncp_socket_recv_t *handle = inet_get_sock_handle(socket);
    if (!handle || handle->socket == -1)
    {
        errno = ENOTCONN;
        return -1;
    }
    ncp_adap_d("send data %d size to fifo rx fd %d\n", size, handle->rx_fifo_fd);
    /* enqueue buffer address to queue */
    ret = xStreamBufferSend(handle->rx_fifo_fd, buf, size, osaWaitForever_c);
    if (ret < 0)
    {
        ncp_adap_e("ncp enhance receive enqueue failure, errno = %d", errno);
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

/* receive ncp device inet socket recv event */
int inet_sock_recv_event(uint8_t *res)
{	
    NCP_CMD_INET_RESP_RECVFROM_CFG *inet_sock_recv_tlv = 0;
    struct ncp_socket_recv_t *handle = 0;
    NCPCmd_DS_INET_COMMAND *evt_res = (NCPCmd_DS_INET_COMMAND *)res;
    if (evt_res->header.result != NCP_CMD_RESULT_OK)
    {
        return -WM_FAIL;
    }
    inet_sock_recv_tlv = (NCP_CMD_INET_RESP_RECVFROM_CFG *)&evt_res->params.wlan_inet_resp_recvfrom;
    /* socket handle */
    int socket = inet_sock_recv_tlv->socket;

    /* receive queue handle */
    handle = inet_get_sock_handle(socket);
    if (!handle || handle->socket == -1)
    {
        errno = ENOTCONN;
        return WM_SUCCESS;
    }

    if (!handle->rx_fifo_fd)
    {
        ncp_adap_e("ncp socket receive rx queue isn't created");
        return -WM_FAIL;
    }
    handle->read_errorn = inet_sock_recv_tlv->errorn;
    if (inet_sock_recv_tlv->ret < 0)
    {
        return WM_SUCCESS;
    }
    memcpy((char *)&(handle->addr), (char *)inet_sock_recv_tlv->sockaddr, inet_sock_recv_tlv->socklen);
    handle->socklen = inet_sock_recv_tlv->socklen;

    /* send to ncp socket enhance receive queue */
    if (handle->socket_type == IPPROTO_TCP)
        inet_sock_recv_send_queue_data(handle->socket, inet_sock_recv_tlv->recv_data, inet_sock_recv_tlv->recv_size);
    else if (handle->socket_type == IPPROTO_UDP || handle->socket_type == IPPROTO_RAW)
        inet_sock_recv_send_queue_data(handle->socket, (char *)inet_sock_recv_tlv, sizeof(NCP_CMD_INET_RESP_RECVFROM_CFG) - 1 + inet_sock_recv_tlv->recv_size);

    return WM_SUCCESS;
}

static int ncp_common_recv(int socket, void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t *length)
{
    int32_t ret = 0;
    StreamBufferHandle_t read_fd;
    struct ncp_socket_recv_t *handle = inet_get_sock_handle(socket);
    TickType_t timeout_ticks;
    if (!handle || handle->socket == -1)
    {
        ncp_adap_e("%s ncp socket receive get queue fail", __func__);
        ret = -WM_FAIL;
        goto exit;
    }

    if (handle->read_errorn)
    {
        errno = handle->read_errorn;
    }
    if (!handle->rx_fifo_fd)
    {
        ncp_adap_e("%s ncp socket receive rx queue isn't created", __func__);
        ret = -WM_FAIL;
        goto exit;
    }

    if (handle->recv_timeout_ms > 0)
    {
        timeout_ticks = pdMS_TO_TICKS(handle->recv_timeout_ms);
    }
    else
    {
        timeout_ticks = portMAX_DELAY;
    }

    read_fd = handle->rx_fifo_fd;
    if (handle->socket_type == IPPROTO_TCP)
    {
        ret = xStreamBufferReceive(read_fd, buffer, size, timeout_ticks);
        if (ret < 0)
        {
            perror("read: ");
        }
        if (length)
        {
            /* for tcp, every packet addr is fixed */
            memcpy((char *)addr, (char *)&(handle->addr), handle->socklen);
            *length = handle->socklen;
        }
    }
    else if (handle->socket_type == IPPROTO_UDP)
    {
        char buf[256];
        /* recv_data need one byte. */
        ret = xStreamBufferReceive(read_fd, buf, sizeof(NCP_CMD_INET_RESP_RECVFROM_CFG) - 1, timeout_ticks);
        if(ret == -1)
            perror("read recvfrom header: ");
        else
        {
            NCP_CMD_INET_RESP_RECVFROM_CFG *inet_sock_recv_tlv = (NCP_CMD_INET_RESP_RECVFROM_CFG *)buf;
            ret = xStreamBufferReceive(read_fd, buffer, inet_sock_recv_tlv->recv_size, timeout_ticks);
            if(ret == -1)
            {
                perror("read recvfrom buffer: ");
            }
            /* for udp, every packet addr isn't fixed */
            else if (length)
            {
                memcpy(addr, inet_sock_recv_tlv->sockaddr, inet_sock_recv_tlv->socklen);
                *length = inet_sock_recv_tlv->socklen;
            }
        }
    }
    else if (handle->socket_type == IPPROTO_RAW)
    {
        char buf[256];
        /* recv_data need one byte. */
        ret = xStreamBufferReceive(read_fd, buf, sizeof(NCP_CMD_INET_RESP_RECVFROM_CFG) - 1, timeout_ticks);
        if(ret == -1)
            perror("read recvfrom header: ");
        else
        {
            NCP_CMD_INET_RESP_RECVFROM_CFG *inet_sock_recv_tlv = (NCP_CMD_INET_RESP_RECVFROM_CFG *)buf;
            ret = xStreamBufferReceive(read_fd, buffer, inet_sock_recv_tlv->recv_size, timeout_ticks);
            if(ret == -1)
            {
                perror("read recvfrom buffer: ");
            }
            /* for udp, every packet addr isn't fixed */
            else if (length)
            {
                memcpy(addr, inet_sock_recv_tlv->sockaddr, inet_sock_recv_tlv->socklen);
                *length = inet_sock_recv_tlv->socklen;
            }
        }
    }
    else
    {
        ncp_adap_e("don't support this socket type %d", handle->socket_type);
    }
    if (errno)
       return -WM_FAIL;
exit:
    return ret;
}

int ncp_recv(int socket, void *buffer, size_t size, int flags)
{
    socket = socketfd_host_to_lwip(socket);
    return ncp_common_recv(socket, buffer, size, flags, 0, 0);
}

int ncp_recvfrom(int socket, void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t *length)
{
    socket = socketfd_host_to_lwip(socket);
    return ncp_common_recv(socket, buffer, size, flags, addr, length);
}


/* system control */
static void ncp_inet_ioctl_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_INET_COMMAND *cmd_res = (NCPCmd_DS_INET_COMMAND *)res;
    NCP_CMD_INET_RESP_IOCTL_CFG *inet_ioctl_resp = &cmd_res->params.wlan_inet_resp_ioctl;

    memcpy(cmd_node->resp_buf, (char *)inet_ioctl_resp, sizeof(NCP_CMD_INET_RESP_IOCTL_CFG));
}

int ncp_ioctl(int fd, long cmd, void *argp)
{
    int ret = 0;
	NCP_CMD_INET_RESP_IOCTL_CFG *inet_ioctl_resp = NULL;
    fd = socketfd_host_to_lwip(fd);
    NCP_CMD_INET_RESP_IOCTL_CFG *cmd_resp_buf = OSA_MemoryAllocate(sizeof(NCP_CMD_INET_RESP_IOCTL_CFG));
    if(cmd_resp_buf == NULL)
    {
        ncp_adap_e("failed to malloc cmd_resp_buf!\r\n");
        return -WM_FAIL;
    }
    (void)memset((uint8_t *)cmd_resp_buf, 0, sizeof(NCP_CMD_INET_RESP_IOCTL_CFG));

    NCPCmd_DS_INET_COMMAND *tlv_cmd = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_IOCTL_CFG));
    if(tlv_cmd == NULL)
    {
        ncp_adap_e("failed to malloc cmd buff.\r\n");
        free(cmd_resp_buf);
        return -WM_FAIL;
    }
    (void) memset((uint8_t *)tlv_cmd, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_IOCTL_CFG));
    
    tlv_cmd->header.cmd    = NCP_CMD_WLAN_INET_IOCTL;
    tlv_cmd->header.size   = NCP_CMD_HEADER_LEN;
    tlv_cmd->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_INET_IOCTL_CFG *inet_ioctl = (NCP_CMD_INET_IOCTL_CFG *) &tlv_cmd->params.wlan_inet_ioctl;

    inet_ioctl->fd   = fd;
    inet_ioctl->cmd   = cmd;
    memcpy(inet_ioctl->argp, argp, 256);
    tlv_cmd->header.size += (sizeof(NCP_CMD_INET_IOCTL_CFG));

    ret = ncp_tlv_send_wait_resp(tlv_cmd, cmd_resp_buf, ncp_inet_ioctl_cb);
    if (ret < 0)
		goto exit;

    inet_ioctl_resp = (NCP_CMD_INET_RESP_IOCTL_CFG *)cmd_resp_buf;
    ret   =  inet_ioctl_resp->ret;
    errno = inet_ioctl_resp->errorno;
    memcpy(argp, inet_ioctl_resp->argp, sizeof(inet_ioctl_resp));

exit:
    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(tlv_cmd);
    return ret;
}


static void ncp_inet_fcntl_cb(void *res, ncp_cmd_node_t * cmd_node)
{
    NCPCmd_DS_INET_COMMAND *cmd_res = (NCPCmd_DS_INET_COMMAND *)res;
    NCP_CMD_INET_RESP_FCNTL_CFG *inet_fcntl_resp = &cmd_res->params.wlan_inet_resp_fcntl;

    memcpy(cmd_node->resp_buf, (char *)inet_fcntl_resp, sizeof(NCP_CMD_INET_RESP_FCNTL_CFG));
}

int ncp_fcntl(int fd, int cmd, int val)
{
    int ret = 0;
	NCP_CMD_INET_RESP_FCNTL_CFG *inet_fcntl_resp = NULL;
    fd = socketfd_host_to_lwip(fd);

    NCP_CMD_INET_RESP_FCNTL_CFG * cmd_resp_buf = OSA_MemoryAllocate(sizeof(NCP_CMD_INET_RESP_FCNTL_CFG));
    if(cmd_resp_buf == NULL)
    {
        ncp_adap_e("failed to malloc cmd_resp_buf!\r\n");
        return -WM_FAIL;
    }
    (void)memset((uint8_t *)cmd_resp_buf, 0, sizeof(NCP_CMD_INET_RESP_FCNTL_CFG));

    NCPCmd_DS_INET_COMMAND *tlv_cmd = OSA_MemoryAllocate(sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_FCNTL_CFG));
    if(tlv_cmd == NULL)
    {
        ncp_adap_e("failed to malloc cmd buff.\r\n");
        free(cmd_resp_buf);
        return -WM_FAIL;
    }
    (void) memset((uint8_t *)tlv_cmd, 0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_FCNTL_CFG));

    tlv_cmd->header.cmd    = NCP_CMD_WLAN_INET_FCNTL;
    tlv_cmd->header.size   = NCP_CMD_HEADER_LEN;
    tlv_cmd->header.result = NCP_CMD_RESULT_OK;

    NCP_CMD_INET_FCNTL_CFG *inet_fcntl = (NCP_CMD_INET_FCNTL_CFG *) &tlv_cmd->params.wlan_inet_fcntl;

    inet_fcntl->fd   = fd;
    inet_fcntl->cmd   = cmd;
    inet_fcntl->val   = val;
    tlv_cmd->header.size += (sizeof(NCP_CMD_INET_FCNTL_CFG) + 256);

    ret = ncp_tlv_send_wait_resp(tlv_cmd, cmd_resp_buf, ncp_inet_fcntl_cb);
    if (ret < 0)
        goto exit;

    inet_fcntl_resp = (NCP_CMD_INET_RESP_FCNTL_CFG *)cmd_resp_buf;
    ret   =  inet_fcntl_resp->ret;
    errno = inet_fcntl_resp->errorno;

exit:
    OSA_MemoryFree(cmd_resp_buf);
    OSA_MemoryFree(tlv_cmd);
    return ret;
}
