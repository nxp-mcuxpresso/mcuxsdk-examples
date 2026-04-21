/*!\file ncp_inet.c
 *\brief NCP INET Socket command/response definitions.
 */
/*
 *  Copyright 2025 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "ncp_inet.h"
#include "ncp_adapter.h"
#include "ncp_wifi.h"

#include "ncp_config.h"
#include "app_notify.h"

#include "uap_prov.h"
#include "serial_httpc.h"
#include "serial_socket.h"

#include "cli_utils.h"
#include "fsl_rtc.h"
#include "fsl_power.h"
#include "fsl_pm_core.h"
#include "fsl_pm_device.h"

#include <lwip/sockets.h>
#include <stddef.h>

void ncp_inet_set_bit(int bit_index);
void ncp_inet_clear_bit(int bit_index);

extern int wifi_ncp_send_response(uint8_t *pbuf);
extern int wifi_ncp_forward_data(uint8_t *pbuf);

/* INET socket type */
#define MAX_SOCKETS 64
typedef struct {
    int sockfd;
    int sock_type;
} socket_type_t;

static socket_type_t socket_type_table[MAX_SOCKETS] = {{-1, 0}};
static int socket_type_count = 0;
OSA_SEMAPHORE_HANDLE_DEFINE(ncp_socket_sem);

static void set_socket_socket(int sockfd, int type)
{
    if (sockfd >= 0 && socket_type_count < MAX_SOCKETS)
    {
        socket_type_table[socket_type_count].sockfd = sockfd;
        socket_type_table[socket_type_count].sock_type = type;
        socket_type_count++;
    }
}

static int get_socket_type(int sockfd)
{
    for (int i = 0; i < socket_type_count; i++)
    {
        if (socket_type_table[i].sockfd == sockfd)
        {
            return socket_type_table[i].sock_type;
        }
    }
    return -1;
}

static void inet_close_socket(int sockfd)
{
    for (int i = 0; i < socket_type_count; i++)
    {
        if (socket_type_table[i].sockfd == sockfd)
        {
            for (int j = i; j < socket_type_count - 1; j++)
            {
                socket_type_table[j] = socket_type_table[j + 1];
            }
            socket_type_count--;
            socket_type_table[socket_type_count].sockfd = -1;
            break;
        }
    }
}

/* INET SOCKET COMMAND*/
/*inet socket command*/
static int wlan_ncp_inet_socket(void *data)
{
    int ret = 0;
    ncp_d("NCP: run %s!\r\n", __func__);
    NCP_CMD_INET_SOCKET_CFG *tlv = (NCP_CMD_INET_SOCKET_CFG *)data;
    tlv->style &= ~BIT(19);
    ret = socket(tlv->family, tlv->style, tlv->protocol);
    if (ret < 0)
    {
        ncp_e("NCP: inet open fail!\r\n", errno);
    }
    else
    {
        set_socket_socket(ret, tlv->style);
        if (get_socket_type(ret) & SOCK_DGRAM)
            ncp_inet_set_bit(ret);
    }
    NCPCmd_DS_INET_COMMAND *cmd_res     = (NCPCmd_DS_INET_COMMAND *)wlan_ncp_get_response_buffer();
    NCP_CMD_INET_RESP_SOCKET_CFG *tlv_res = (NCP_CMD_INET_RESP_SOCKET_CFG *)&cmd_res->params.inet_open;
    tlv_res->ret             = ret;
    tlv_res->errno           = errno;
    cmd_res->header.cmd      = NCP_RSP_WLAN_INET_SOCKET;
    cmd_res->header.size     = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum   = 0x00;
    cmd_res->header.result   = NCP_CMD_RESULT_OK;
    cmd_res->header.size += sizeof(NCP_CMD_SOCKET_OPEN_CFG);
    ncp_d("NCP: %s done!\r\n", __func__);
    return WM_SUCCESS;
}

/*inet connect command*/
static int wlan_ncp_inet_connect(void *data)
{
    int ret = 0;
    int sa_data_offset = 0;
    ncp_d("NCP: run %s!\r\n", __func__);
    NCP_CMD_INET_CON_CFG *tlv = (NCP_CMD_INET_CON_CFG *)data;
    union ncp_sockaddr_aligned align_name;
    memset(&align_name, 0, sizeof(align_name));
    struct sockaddr *name = &align_name.sa;
    struct linux_sockaddr *linux_name = (struct linux_sockaddr *)tlv->sockaddr;
    name->sa_family = linux_name->sa_family;
    sa_data_offset = offsetof(struct sockaddr, sa_data);
    name->sa_len = tlv->socklen;
    memcpy(name->sa_data, linux_name->sa_data, tlv->socklen-sa_data_offset);
    /* for tcp socket, select this socket recv data before connect */
    if (get_socket_type(tlv->socket) & SOCK_STREAM)
    {
        ncp_inet_set_bit(tlv->socket);
    }
    ret = connect(tlv->socket, name, tlv->socklen);
    if (ret)
    {
        if (get_socket_type(tlv->socket) & SOCK_STREAM)
        {
            ncp_inet_clear_bit(tlv->socket);
        }
    }
    NCPCmd_DS_INET_COMMAND *cmd_res     = (NCPCmd_DS_INET_COMMAND *)wlan_ncp_get_response_buffer();
    NCP_CMD_INET_RESP_CON_CFG *tlv_res = (NCP_CMD_INET_RESP_CON_CFG *)&cmd_res->params.inet_con;
    tlv_res->ret               = ret;
    tlv_res->errno             = errno;
    cmd_res->header.cmd      = NCP_RSP_WLAN_INET_CON;
    cmd_res->header.size     = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum   = 0x00;
    cmd_res->header.result   = NCP_CMD_RESULT_OK;
    cmd_res->header.size += sizeof(NCP_CMD_INET_RESP_CON_CFG);

    ncp_d("NCP: %s done!\r\n", __func__);
    return WM_SUCCESS;
}

/*inet bind command*/
static int wlan_ncp_inet_bind(void *data)
{
    int ret = 0;
    int sa_data_offset = 0;
    ncp_d("NCP: run %s!\r\n", __func__);
    NCP_CMD_INET_BIND_CFG *tlv = (NCP_CMD_INET_BIND_CFG *)data;

    union ncp_sockaddr_aligned align_name;
    memset(&align_name, 0, sizeof(align_name));
    struct sockaddr *name = &align_name.sa;
    struct linux_sockaddr *linux_name = (struct linux_sockaddr *)tlv->sockaddr;
    name->sa_family = linux_name->sa_family;
    name->sa_len = tlv->socklen;
    sa_data_offset = offsetof(struct sockaddr, sa_data);
    memcpy(name->sa_data, linux_name->sa_data, tlv->socklen-sa_data_offset);
    ret                          = bind(tlv->socket, name, tlv->socklen);
    NCPCmd_DS_INET_COMMAND *cmd_res     = (NCPCmd_DS_INET_COMMAND *)wlan_ncp_get_response_buffer();
    NCP_CMD_INET_RESP_CON_CFG *tlv_res = (NCP_CMD_INET_RESP_CON_CFG *)&cmd_res->params.inet_bind;
    tlv_res->ret               = ret;
    tlv_res->errno             = errno;
    cmd_res->header.cmd      = NCP_RSP_WLAN_INET_BIND;
    cmd_res->header.size     = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum   = 0x00;
    cmd_res->header.result   = NCP_CMD_RESULT_OK;
    cmd_res->header.size += sizeof(NCP_CMD_INET_RESP_BIND_CFG);

    ncp_d("NCP: %s done!\r\n", __func__);
    return WM_SUCCESS;
}

/*inet close command*/
static int wlan_ncp_inet_close(void *data)
{
    int ret = 0;
    ncp_d("NCP: run %s!\r\n", __func__);
    NCP_CMD_INET_CLOSE_CFG *tlv = (NCP_CMD_INET_CLOSE_CFG *)data;
    ncp_inet_clear_bit(tlv->socket);
    inet_close_socket(tlv->socket);
    ret = close(tlv->socket);
    if (ret != 0)
        ncp_e("NCP: close socket[%d] fail!\r\n", tlv->socket);
    NCPCmd_DS_INET_COMMAND *cmd_res     = (NCPCmd_DS_INET_COMMAND *)wlan_ncp_get_response_buffer();
    NCP_CMD_INET_RESP_CLOSE_CFG *tlv_res = (NCP_CMD_INET_RESP_CLOSE_CFG *)&cmd_res->params.inet_close;
    tlv_res->ret               = ret;
    tlv_res->errno             = errno;
    cmd_res->header.cmd      = NCP_RSP_WLAN_INET_CLOSE;
    cmd_res->header.size     = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum   = 0x00;
    cmd_res->header.result   = NCP_CMD_RESULT_OK;
    cmd_res->header.size += sizeof(NCP_CMD_INET_RESP_CLOSE_CFG);
    ncp_d("NCP: %s done!\r\n", __func__);
    return WM_SUCCESS;
}

/*inet listen command*/
static int wlan_ncp_inet_listen(void *data)
{
    int ret = 0;
    ncp_d("NCP: run %s!\r\n", __func__);
    NCP_CMD_INET_LISTEN_CFG *tlv = (NCP_CMD_INET_LISTEN_CFG *)data;
    ret                            = listen(tlv->socket, tlv->number);
    NCPCmd_DS_INET_COMMAND *cmd_res     = (NCPCmd_DS_INET_COMMAND *)wlan_ncp_get_response_buffer();
    NCP_CMD_INET_RESP_LISTEN_CFG *tlv_res = (NCP_CMD_INET_RESP_LISTEN_CFG *)&cmd_res->params.inet_listen;
    tlv_res->ret               = ret;
    tlv_res->errno             = errno;
    cmd_res->header.cmd      = NCP_RSP_WLAN_INET_LISTEN;
    cmd_res->header.size     = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum   = 0x00;
    cmd_res->header.result   = NCP_CMD_RESULT_OK;
    cmd_res->header.size += sizeof(NCP_CMD_INET_RESP_LISTEN_CFG);
    ncp_d("NCP: %s done!\r\n", __func__);
    return WM_SUCCESS;
}

/*inet accept command*/
static int wlan_ncp_inet_accept(void *data)
{
    ncp_d("NCP: run %s!\r\n", __func__);
    int ret           = WM_SUCCESS;
    int new_socket    = -1;
    socklen_t length = sizeof(union ncp_sockaddr_aligned);
    unsigned long sa_data_offset = 0;
    union ncp_sockaddr_aligned align_name;
    memset(&align_name, 0, sizeof(align_name));
    struct sockaddr *client_addr = &align_name.sa;
    NCP_CMD_INET_ACCEPT_CFG *tlv = (NCP_CMD_INET_ACCEPT_CFG *)data;
    new_socket                  = accept(tlv->socket, (struct sockaddr *)&align_name.sin6, &length);
    if (new_socket >= 0)
    {
        set_socket_socket(new_socket, SOCK_STREAM);
        ncp_inet_set_bit(new_socket);
    }
    NCPCmd_DS_INET_COMMAND *cmd_res     = (NCPCmd_DS_INET_COMMAND *)wlan_ncp_get_response_buffer();
    NCP_CMD_INET_RESP_ACCEPT_CFG *tlv_res = (NCP_CMD_INET_RESP_ACCEPT_CFG *)&cmd_res->params.inet_accept;
    tlv_res->ret               = new_socket;
    tlv_res->errno             = errno;
    tlv_res->socklen           = length;
    struct linux_sockaddr *linux_addr = (struct linux_sockaddr *)tlv_res->sockaddr;
    linux_addr->sa_family      = client_addr->sa_family;
    sa_data_offset = offsetof(struct sockaddr, sa_data);
    memcpy(linux_addr->sa_data, ((char *)&align_name) +sa_data_offset, length-sa_data_offset);
    cmd_res->header.cmd        = NCP_RSP_WLAN_INET_ACCEPT;
    cmd_res->header.size       = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum     = 0x00;
    cmd_res->header.result     = NCP_CMD_RESULT_OK;
    cmd_res->header.size += sizeof(NCP_CMD_INET_RESP_ACCEPT_CFG);
    return ret;
}

/*inet getsockname command*/
static int wlan_ncp_inet_getsockname(void *data)
{
    ncp_d("NCP: run %s!\r\n", __func__);
    int ret           = WM_SUCCESS;
    int sa_data_offset = 0;
    socklen_t length = sizeof(union ncp_sockaddr_aligned);
    union ncp_sockaddr_aligned align_name;
    memset(&align_name, 0, sizeof(align_name));
    struct sockaddr *client_addr = &align_name.sa;
    NCP_CMD_INET_GETSOCKNAME_CFG *tlv = (NCP_CMD_INET_GETSOCKNAME_CFG *)data;
    ret                  = getsockname(tlv->socket, client_addr, &length);
    NCPCmd_DS_INET_COMMAND *cmd_res     = (NCPCmd_DS_INET_COMMAND *)wlan_ncp_get_response_buffer();
    NCP_CMD_INET_RESP_GETSOCKNAME_CFG *tlv_res = (NCP_CMD_INET_RESP_GETSOCKNAME_CFG *)&cmd_res->params.inet_getsockname;
    tlv_res->ret               = ret;
    tlv_res->errno             = errno;
    tlv_res->socklen           = length;
    struct linux_sockaddr *linux_addr = (struct linux_sockaddr *)tlv_res->sockaddr;
    linux_addr->sa_family      = client_addr->sa_family;
    sa_data_offset = offsetof(struct sockaddr, sa_data);
    memcpy(linux_addr->sa_data, ((char *)&align_name) +sa_data_offset, length-sa_data_offset);
    cmd_res->header.cmd        = NCP_RSP_WLAN_INET_GETSOCKNAME;
    cmd_res->header.size       = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum     = 0x00;
    cmd_res->header.result     = NCP_CMD_RESULT_OK;
    cmd_res->header.size += sizeof(NCP_CMD_INET_RESP_GETSOCKNAME_CFG);
    return WM_SUCCESS;
}

/*inet getpeername command*/
static int wlan_ncp_inet_getpeername(void *data)
{
    ncp_d("NCP: run %s!\r\n", __func__);
    int ret           = WM_SUCCESS;
    int sa_data_offset = 0;
    socklen_t length = sizeof(union ncp_sockaddr_aligned);
    union ncp_sockaddr_aligned align_name;
    memset(&align_name, 0, sizeof(align_name));
    struct sockaddr *client_addr = &align_name.sa;
    NCP_CMD_INET_GETPEERNAME_CFG *tlv = (NCP_CMD_INET_GETPEERNAME_CFG *)data;
    ret                  = getpeername(tlv->socket, client_addr, &length);
    NCPCmd_DS_INET_COMMAND *cmd_res     = (NCPCmd_DS_INET_COMMAND *)wlan_ncp_get_response_buffer();
    NCP_CMD_INET_RESP_GETPEERNAME_CFG *tlv_res = (NCP_CMD_INET_RESP_GETPEERNAME_CFG *)&cmd_res->params.inet_getpeername;
    tlv_res->ret               = ret;
    tlv_res->errno             = errno;
    tlv_res->socklen           = length;
    struct linux_sockaddr *linux_addr = (struct linux_sockaddr *)tlv_res->sockaddr;
    linux_addr->sa_family      = client_addr->sa_family;
    sa_data_offset = offsetof(struct sockaddr, sa_data);
    memcpy(linux_addr->sa_data, ((char *)&align_name) +sa_data_offset, length-sa_data_offset);
    cmd_res->header.cmd        = NCP_RSP_WLAN_INET_GETPEERNAME;
    cmd_res->header.size       = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum     = 0x00;
    cmd_res->header.result     = NCP_CMD_RESULT_OK;
    cmd_res->header.size += sizeof(NCP_CMD_INET_RESP_GETPEERNAME_CFG);
    return WM_SUCCESS;
}

static int sockopt_linuxtolwip(int sockopt)
{
    switch (sockopt)
    {
        case LINUX_SOL_SOCKET:
            return SOL_SOCKET;
        case LINUX_SO_REUSEADDR:
            return SO_REUSEADDR;
        case LINUX_SO_REUSEPORT:
            return SO_REUSEPORT;
        case LINUX_SO_BINDTODEVICE:
            return SO_BINDTODEVICE;
        case LINUX_SO_BROADCAST:
            return SO_BROADCAST;
        case LINUX_IPV6_V6ONLY:
            return IPV6_V6ONLY;
        case LINUX_IP_MULTICAST_IF:
            return IP_MULTICAST_IF;
        case LINUX_IP_MULTICAST_TTL:
            return IP_MULTICAST_TTL;
        default:
            break;
    }
    return sockopt;
}

/*inet getsockopt command*/
static int wlan_ncp_inet_getsockopt(void *data)
{
    ncp_d("NCP: run %s!\r\n", __func__);
    int ret           = WM_SUCCESS;
    socklen_t length = 0;
    NCP_CMD_INET_GETSOCKOPT_CFG *tlv = (NCP_CMD_INET_GETSOCKOPT_CFG *)data;
    char optval[64] = {0};
    tlv->level = sockopt_linuxtolwip(tlv->level);
    tlv->optname = sockopt_linuxtolwip(tlv->optname);
    ret                  = getsockopt(tlv->socket, tlv->level, tlv->optname, optval, &length);
    NCPCmd_DS_INET_COMMAND *cmd_res     = (NCPCmd_DS_INET_COMMAND *)wlan_ncp_get_response_buffer();
    NCP_CMD_INET_RESP_GETSOCKOPT_CFG *tlv_res = (NCP_CMD_INET_RESP_GETSOCKOPT_CFG *)&cmd_res->params.inet_getsockopt;
    tlv_res->ret               = ret;
    tlv_res->errno             = errno;
    tlv_res->socklen           = length;
    memcpy(tlv_res->optval, optval, length);
    cmd_res->header.cmd        = NCP_RSP_WLAN_INET_GETSOCKOPT;
    cmd_res->header.size       = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum     = 0x00;
    cmd_res->header.result     = NCP_CMD_RESULT_OK;
    cmd_res->header.size += sizeof(NCP_CMD_INET_RESP_GETSOCKOPT_CFG);
    return WM_SUCCESS;
}

/*inet setsockopt command*/
static int wlan_ncp_inet_setsockopt(void *data)
{
    ncp_d("NCP: run %s!\r\n", __func__);
    int ret           = WM_SUCCESS;

    NCP_CMD_INET_SETSOCKOPT_CFG *tlv = (NCP_CMD_INET_SETSOCKOPT_CFG *)data;

    tlv->level = sockopt_linuxtolwip(tlv->level);
    tlv->optname = sockopt_linuxtolwip(tlv->optname);
    ret                  = setsockopt(tlv->socket, tlv->level, tlv->optname, tlv->optval, (socklen_t)tlv->socklen);
    NCPCmd_DS_INET_COMMAND *cmd_res     = (NCPCmd_DS_INET_COMMAND *)wlan_ncp_get_response_buffer();
    NCP_CMD_INET_RESP_SETSOCKOPT_CFG *tlv_res = (NCP_CMD_INET_RESP_SETSOCKOPT_CFG *)&cmd_res->params.inet_setsockopt;
    tlv_res->ret               = ret;
    tlv_res->errno             = errno;
    cmd_res->header.cmd        = NCP_RSP_WLAN_INET_SETSOCKOPT;
    cmd_res->header.size       = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum     = 0x00;
    cmd_res->header.result     = NCP_CMD_RESULT_OK;
    cmd_res->header.size += sizeof(NCP_CMD_INET_RESP_SETSOCKOPT_CFG);
    return WM_SUCCESS;
}

/*INET System control*/
/*inet ioctl command*/
static int wlan_ncp_inet_ioctl(void *data)
{
    ncp_d("NCP: run %s!\r\n", __func__);
    int ret           = WM_SUCCESS;
    NCP_CMD_INET_IOCTL_CFG *tlv = (NCP_CMD_INET_IOCTL_CFG *)data;
    
    ret                  = lwip_ioctl(tlv->fd, tlv->cmd, tlv->argp);
    NCPCmd_DS_INET_COMMAND *cmd_res     = (NCPCmd_DS_INET_COMMAND *)wlan_ncp_get_response_buffer();
    NCP_CMD_INET_RESP_IOCTL_CFG *tlv_res = (NCP_CMD_INET_RESP_IOCTL_CFG *)&cmd_res->params.inet_ioctl;
    tlv_res->ret               = ret;
    tlv_res->errno             = errno;
    memcpy(tlv_res->argp, tlv->argp, sizeof(tlv->argp));
    cmd_res->header.cmd        = NCP_RSP_WLAN_INET_IOCTL;
    cmd_res->header.size       = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum     = 0x00;
    cmd_res->header.result     = NCP_CMD_RESULT_OK;
    cmd_res->header.size += sizeof(NCP_CMD_INET_RESP_IOCTL_CFG);
    
    return WM_SUCCESS;
}

/*inet fcnt command*/
static int wlan_ncp_inet_fcntl(void *data)
{
    ncp_d("NCP: run %s!\r\n", __func__);
    int ret           = WM_SUCCESS;
    NCP_CMD_INET_FCNTL_CFG *tlv = (NCP_CMD_INET_FCNTL_CFG *)data;
    
    ret                  = lwip_fcntl(tlv->fd, tlv->cmd, tlv->val);

    NCPCmd_DS_INET_COMMAND *cmd_res     = (NCPCmd_DS_INET_COMMAND *)wlan_ncp_get_response_buffer();
    NCP_CMD_INET_RESP_FCNTL_CFG *tlv_res = (NCP_CMD_INET_RESP_FCNTL_CFG *)&cmd_res->params.inet_fcntl;
    tlv_res->ret               = ret;
    tlv_res->errno             = errno;
    cmd_res->header.cmd        = NCP_RSP_WLAN_INET_FCNTL;
    cmd_res->header.size       = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum     = 0x00;
    cmd_res->header.result     = NCP_CMD_RESULT_OK;
    cmd_res->header.size += sizeof(NCP_CMD_INET_RESP_FCNTL_CFG);
    
    return WM_SUCCESS;
}

/* INET recv command */
static int ncp_inet_recv_create_task(void);
struct recv_send_data_t
{
    int ret;
    int errno;
    int socket;
    uint8_t *recv_buf;
    int recv_size;
    socklen_t socklen;
    union ncp_sockaddr_aligned client_addr;
};

static uint64_t ncp_bitmap = 0;
static int max_sock_fd = 0;
void ncp_inet_set_bit(int bit_index)
{
    if (bit_index >= 0 && bit_index < 64)
    {
        ncp_bitmap |= (1ULL << bit_index);
        OSA_SemaphorePost((osa_semaphore_handle_t)ncp_socket_sem);
    }
}

void ncp_inet_clear_bit(int bit_index)
{
    if (bit_index >= 0 && bit_index < 64)
    {
        ncp_bitmap &= ~(1ULL << bit_index);
        OSA_SemaphorePost((osa_semaphore_handle_t)ncp_socket_sem);
    }
}

int check_bit(uint64_t bitmap, int bit_index)
{
    if (bit_index >= 0 && bit_index < 64)
    {
        return (bitmap & (1ULL << bit_index)) != 0;
    }
    return 0;
}

fd_set traverse_bitmap_and_setfd(uint64_t bitmap)
{
    fd_set readset;
    FD_ZERO(&readset);
    int i = 0;
    max_sock_fd = 0;
    int has_socket = 0;
    for (; i < 64; i++)
    {
        int bit_value = check_bit(bitmap, i);
        if (bit_value)
        {
            has_socket = 1;
            max_sock_fd = max_sock_fd > i? max_sock_fd : i+1;
            FD_SET(i, &readset);
        }
    }
    if (!has_socket)
        max_sock_fd = 0;

    return readset;
}

#define WIFI_NCP_RX_ALLOC_TRY_NUM  20
static void socket_recv_task(void *arg)
{
#define NCP_SOCKET_RECEIVE_TIMEOUT_INTVAL  1000
    NCP_CMD_INET_RESP_RECVFROM_CFG *tlv_res = 0;
    int buf_len = 0;
    int ret = 0;
    uint8_t *recv_buf = 0;
    int chksum_len = 4;
    struct pbuf *p;
    int sa_data_offset = offsetof(struct sockaddr, sa_data);
    int mem_alloc_try_num;

    while(1)
    {
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = NCP_SOCKET_RECEIVE_TIMEOUT_INTVAL;
        fd_set readset = traverse_bitmap_and_setfd(ncp_bitmap);
        if (max_sock_fd <= 0)
        {
            OSA_SemaphoreWait((osa_semaphore_handle_t)ncp_socket_sem, osaWaitForever_c);
            continue;
        }
        if (lwip_select(max_sock_fd, (fd_set *)&readset, 0, 0, &timeout) > 0)
        {
            for (int i = 0; i < max_sock_fd; i++)
            {
                if (!check_bit(ncp_bitmap, i))
                    continue;
                if (FD_ISSET(i, &readset))
                {
                    do {
                        mem_alloc_try_num = WIFI_NCP_RX_ALLOC_TRY_NUM;
                        buf_len = ALIGN_D(NCP_INET_SOCKET_RECV_SIZE + sizeof(NCP_CMD_INET_RESP_RECVFROM_CFG)
                          + sizeof(NCP_COMMAND)
#if CONFIG_NCP_USE_ENCRYPT
                          + NCP_GCM_TAG_LEN
#endif
                          + chksum_len);
realloc_mem:
                        p = ncp_buf_alloc(buf_len, 0);
                        if (!p)
                        {
                            if (mem_alloc_try_num)
                            {
                                mem_alloc_try_num--;
                                OSA_TaskYield();
                                goto realloc_mem;
                            }
                            break;
                        }
                        recv_buf = p->payload;
                        union ncp_sockaddr_aligned client_addr;
                        socklen_t socklen = sizeof(client_addr);
                        socklen = sizeof(client_addr);
                        tlv_res = (NCP_CMD_INET_RESP_RECVFROM_CFG *)(recv_buf + sizeof(NCP_COMMAND));
                        memset(recv_buf, 0x0, sizeof(NCP_COMMAND) + sizeof(NCP_CMD_INET_RESP_RECVFROM_CFG));
                        /* read data from tcp/ip stack */
                        ret = recvfrom(i, tlv_res->recv_data, NCP_INET_SOCKET_RECV_SIZE, MSG_DONTWAIT, (struct sockaddr *)&client_addr.sin6, &socklen);
                        tlv_res->ret = ret;
                        tlv_res->errno = errno;
                        tlv_res->socket = i;
                        if (ret > 0)
                        {
                            tlv_res->recv_size = ret;
                        }
                        else if (ret == 0)
                        {
                            /* TCP FIN */
                            ncp_d("[%s-%d], ret = %d, errno = %d\n", __func__, __LINE__, ret, errno);
                            ncp_inet_clear_bit(i);
                        }
                        else
                        {
                            if (errno == EAGAIN || errno == EWOULDBLOCK)
                            { /* no data */
                                ncp_buf_free(p);
                                break;
                            }
                            else if (errno == ECONNRESET)
                            { /* TCP RST */
                                ncp_d("socket %d: RST received\n", i);
                                ncp_inet_clear_bit(i);
                            }
                            else
                            { /* other issue */
                                ncp_d("socket %d: recv error %d\n", i, errno);
                                ncp_inet_clear_bit(i);
                            }
                        }
                        struct linux_sockaddr *linux_addr = (struct linux_sockaddr *)tlv_res->sockaddr;
                        linux_addr->sa_family      = client_addr.sa.sa_family;
                        memcpy(linux_addr->sa_data, client_addr.sa.sa_data, socklen-sa_data_offset);
                        tlv_res->socklen = socklen;
                        ncp_inet_prepare_socket_recv_resp(recv_buf);
                        if (wifi_ncp_forward_data((void *)p) != WM_SUCCESS)
                        {
                            ncp_e("ncp inet send receive event fail\r\n");
                            ncp_buf_free(p);
                            OSA_TaskYield();
                        }
                    } while (ret > 0);
                }
            }
        }
    }
}

uint8_t *ncp_inet_prepare_socket_recv_resp(uint8_t *receive_res)
{
    uint8_t *event_buf         = (uint8_t *)receive_res;
    NCPCmd_DS_INET_COMMAND *evt_res = NULL;

    evt_res                                = (NCPCmd_DS_INET_COMMAND *)event_buf;
    NCP_CMD_INET_RESP_RECVFROM_CFG *recv_res = (NCP_CMD_INET_RESP_RECVFROM_CFG *)&evt_res->params.inet_recvfrom;

    evt_res->header.cmd        = NCP_EVENT_WLAN_NCP_INET_RECV;
    evt_res->header.size       = NCP_CMD_HEADER_LEN;
    evt_res->header.seqnum     = 0x00;
    evt_res->header.result     = NCP_CMD_RESULT_OK;

    evt_res->header.size       += (sizeof(NCP_CMD_INET_RESP_RECVFROM_CFG) + recv_res->recv_size);
    ncp_d("prepare receive buffer ready");
    return event_buf;
}

static OSA_TASK_HANDLE_DEFINE(ncp_inet_recv_thread);
static OSA_TASK_DEFINE(socket_recv_task, PRIORITY_RTOS_TO_OSA((configMAX_PRIORITIES-3)), 1, 5000, 0);
static int ncp_inet_recv_create_task(void)
{
    ncp_d("NCP: run %s!\r\n", __func__);
    (void)OSA_SemaphoreCreateBinary((osa_semaphore_handle_t)ncp_socket_sem);
    (void)OSA_TaskCreate((osa_task_handle_t)ncp_inet_recv_thread, OSA_TASK(socket_recv_task), NULL);
    return WM_SUCCESS;
}

int ncp_inet_init(void)
{
    return ncp_inet_recv_create_task();
}

void ncp_inet_deinit(void)
{
    OSA_TaskDestroy(ncp_inet_recv_thread);
}

/* INET send command */
static int ncp_inet_send_fail_resp(int socket, int status, int errorno)
{
    uint8_t *buf = 0;
    int buf_len = 0;
    NCP_CMD_INET_RESP_SEND_CFG *tlv_res = 0;

    buf_len = sizeof(NCP_CMD_INET_RESP_SEND_CFG)+sizeof(NCP_COMMAND);
    buf = (uint8_t *)OSA_MemoryAllocate(buf_len);
    if (!buf)
    {
        ncp_e("ncp socket receive buffer alloc fail\r\n");
        goto exit;
    }

    memset(buf, 0, buf_len);
    /* reserve memory for NCP_COMMAND */
    tlv_res = (NCP_CMD_INET_RESP_SEND_CFG *)(buf + sizeof(NCP_COMMAND));
    tlv_res->ret = status;
    tlv_res->errno = errorno;
    tlv_res->socket = socket;
    if (app_notify_event(APP_EVT_INET_SOCKET_SEND_FAIL, APP_EVT_REASON_SUCCESS, buf, sizeof(NCP_CMD_INET_RESP_SEND_CFG)) !=
            WM_SUCCESS)
    {
        ncp_e("NCP: APP_EVT_SOCKET_RECEIVE event fail\r\n");
        OSA_MemoryFree(buf);
        return -WM_FAIL;
    }
exit:
    return WM_SUCCESS;
}

uint8_t *ncp_inet_prepare_socket_send_fail(uint8_t *send_fail)
{
    uint8_t *event_buf         = (uint8_t *)send_fail;
    NCPCmd_DS_INET_COMMAND *evt_res = NULL;

    evt_res                                = (NCPCmd_DS_INET_COMMAND *)event_buf;

    evt_res->header.cmd        = NCP_EVENT_WLAN_NCP_INET_SEND_FAIL;
    evt_res->header.size       = NCP_CMD_HEADER_LEN;
    evt_res->header.seqnum     = 0x00;
    evt_res->header.result     = NCP_CMD_RESULT_OK;

    evt_res->header.size       += sizeof(NCP_CMD_INET_RESP_SEND_CFG);
    ncp_d("prepare receive buffer ready");
    return event_buf;
}

extern OSA_SEMAPHORE_HANDLE_DEFINE(wifi_ncp_lock);
static int wlan_ncp_inet_sendto(void *data)
{
    int ret = WM_SUCCESS;
    ncp_d("NCP: run %s!\r\n", __func__);
    NCP_CMD_INET_SENDTO_CFG *tlv = (NCP_CMD_INET_SENDTO_CFG *)data;
    union ncp_sockaddr_aligned align_name;
    memset(&align_name, 0, sizeof(align_name));
    struct sockaddr *to = &align_name.sa;
    struct linux_sockaddr *linux_name = (struct linux_sockaddr *)tlv->sockaddr;
    to->sa_family = linux_name->sa_family;
    to->sa_len = tlv->socklen;
    memcpy(to->sa_data, linux_name->sa_data, tlv->socklen);
    if (tlv->socklen)
    {
        ret = sendto(tlv->socket, tlv->send_data, tlv->size, tlv->flags, to, tlv->socklen);
    }
    else
    {
        ret = send(tlv->socket, tlv->send_data, tlv->size, tlv->flags);
    }
    if (ret < 0)
    {
        ncp_inet_send_fail_resp(tlv->socket, ret, errno);
    }

    OSA_SemaphorePost(wifi_ncp_lock);
    ncp_d("NCP: %s done!\r\n", __func__);

    return 0;
}

struct cmd_t wlan_cmd_inet[] = {
    {NCP_CMD_WLAN_INET_SOCKET, "wlan-inet-socket", wlan_ncp_inet_socket, CMD_SYNC},
    {NCP_CMD_WLAN_INET_CON, "wlan-inet-connect", wlan_ncp_inet_connect, CMD_SYNC},
    {NCP_CMD_WLAN_INET_BIND, "wlan-inet-bind", wlan_ncp_inet_bind, CMD_SYNC},
    {NCP_CMD_WLAN_INET_CLOSE, "wlan-inet-close", wlan_ncp_inet_close, CMD_SYNC},
    {NCP_CMD_WLAN_INET_LISTEN, "wlan-inet-listen", wlan_ncp_inet_listen, CMD_SYNC},
    {NCP_CMD_WLAN_INET_ACCEPT, "wlan-inet-accept", wlan_ncp_inet_accept, CMD_SYNC},
//    {NCP_CMD_WLAN_INET_SEND, "wlan-inet-send", wlan_ncp_inet_send, CMD_SYNC},
    {NCP_CMD_WLAN_INET_SENDTO, "wlan-inet-sendto", wlan_ncp_inet_sendto, CMD_ASYNC},
//    {NCP_CMD_WLAN_INET_RECV, "wlan-inet-receive", wlan_ncp_inet_recv, CMD_SYNC}
//    {NCP_CMD_WLAN_INET_RECVFROM, "wlan-inet-recvfrom", wlan_ncp_inet_recvfrom, CMD_SYNC},
    {NCP_CMD_WLAN_INET_GETSOCKNAME, "wlan-inet-getsockname", wlan_ncp_inet_getsockname, CMD_SYNC},
    {NCP_CMD_WLAN_INET_GETPEERNAME, "wlan-inet-getpeername", wlan_ncp_inet_getpeername, CMD_SYNC},
    {NCP_CMD_WLAN_INET_GETSOCKOPT, "wlan-inet-getsockopt", wlan_ncp_inet_getsockopt, CMD_SYNC},
    {NCP_CMD_WLAN_INET_SETSOCKOPT, "wlan-inet-setsockopt", wlan_ncp_inet_setsockopt, CMD_SYNC},
    {NCP_CMD_WLAN_INET_IOCTL, "wlan-inet-ioctl", wlan_ncp_inet_ioctl, CMD_SYNC},
    {NCP_CMD_WLAN_INET_FCNTL, "wlan-inet-fcntl", wlan_ncp_inet_fcntl, CMD_SYNC},
    {NCP_CMD_INVALID, NULL, NULL, NULL},
};
