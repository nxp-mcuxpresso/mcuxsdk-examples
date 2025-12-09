/** @file ncp_inet.h
 *
 *  Copyright 2024-2025 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __NCP_INET_H__
#define __NCP_INET_H__

#include "ncp_host_command.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ncp_host_utils_wifi.h"
#include "stream_buffer.h"

#define SOL_SOCKET      0xfff  /* options for socket level */
#define SO_RCVTIMEO     0x1006 /* receive timeout */

/** WLAN Inet command */
#define NCP_CMD_WLAN_INET        0x01000000
/** Wi-Fi socket socket command ID */
#define NCP_CMD_WLAN_INET_SOCKET      (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000001) /* wlan-socket-open */
/** Wi-Fi socket socket command response ID */
#define NCP_RSP_WLAN_INET_OPEN      (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x00000001)
/** Wi-Fi socket connect command ID */
#define NCP_CMD_WLAN_INET_CON       (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000002) /* wlan-socket-connect */
/** Wi-Fi socket connect command response ID */
#define NCP_RSP_WLAN_INET_CON       (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x00000002)
/** Wi-Fi socket recvfrom command ID */
#define NCP_CMD_WLAN_INET_RECVFROM  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000004)
/** Wi-Fi socket sendto command ID */
#define NCP_CMD_WLAN_INET_SENDTO    (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000006) /* wlan-socket-sendto */
/** Wi-Fi socket bind command ID */
#define NCP_CMD_WLAN_INET_BIND      (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000007) /* wlan-socket-bind */
/** Wi-Fi socket bind command response ID */
#define NCP_RSP_WLAN_INET_BIND      (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x00000007)
/** Wi-Fi socket listen command ID */
#define NCP_CMD_WLAN_INET_LISTEN    (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000008) /* wlan-socket-listen */
/** Wi-Fi socket listen command response ID */
#define NCP_RSP_WLAN_INET_LISTEN    (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x00000008)
/** Wi-Fi socket accept command ID */
#define NCP_CMD_WLAN_INET_ACCEPT    (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000009) /* wlan-socket-accept */
/** Wi-Fi socket accept command response ID */
#define NCP_RSP_WLAN_INET_ACCEPT    (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x00000009)
/** Wi-Fi socket close command ID */
#define NCP_CMD_WLAN_INET_CLOSE     (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x0000000a) /* wlan-socket-close */
/** Wi-Fi socket close command response ID */
#define NCP_RSP_WLAN_INET_CLOSE     (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x0000000a)
/** Wi-Fi socket getsockname from command ID */
#define NCP_CMD_WLAN_INET_GETSOCKNAME  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x0000000b) /* wlan-socket-recvfrom */
/** Wi-Fi socket getsockname from command response ID */
#define NCP_RSP_WLAN_INET_GETSOCKNAME  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x0000000b)
/** Wi-Fi socket getpeername from command ID */
#define NCP_CMD_WLAN_INET_GETPEERNAME  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x0000000c) /* wlan-socket-recvfrom */
/** Wi-Fi socket getpeername from command response ID */
#define NCP_RSP_WLAN_INET_GETPEERNAME  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x0000000c)
/** Wi-Fi socket getsockopt from command ID */
#define NCP_CMD_WLAN_INET_GETSOCKOPT  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x0000000d) /* wlan-socket-recvfrom */
/** Wi-Fi socket getsockopt from command response ID */
#define NCP_RSP_WLAN_INET_GETSOCKOPT  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x0000000d)
/** Wi-Fi socket setsockopt from command ID */
#define NCP_CMD_WLAN_INET_SETSOCKOPT  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x0000000e) /* wlan-socket-recvfrom */
/** Wi-Fi socket setsockopt from command response ID */
#define NCP_RSP_WLAN_INET_SETSOCKOPT  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x0000000e)

/** Wi-Fi socket ioctl from command ID */
#define NCP_CMD_WLAN_INET_IOCTL  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x0000000f) /* wlan-socket-recvfrom */
/** Wi-Fi socket ioctl from command response ID */
#define NCP_RSP_WLAN_INET_IOCTL  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x0000000f)
/** Wi-Fi socket fcntl from command ID */
#define NCP_CMD_WLAN_INET_FCNTL  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000010) /* wlan-socket-recvfrom */
/** Wi-Fi socket fcntl from command response ID */
#define NCP_RSP_WLAN_INET_FCNTL  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x00000010)
/** Wi-Fi socket shutdown from command ID */
#define NCP_CMD_WLAN_INET_SHUTDOWN  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000011) /* wlan-socket-shutdown */
/** Wi-Fi socket shutdown from command response ID */
#define NCP_RSP_WLAN_INET_SHUTDOWN  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x00000011)

/** Wi-Fi socket recv event ID */
#define NCP_EVENT_WLAN_NCP_INET_RECV   (NCP_CMD_WLAN | NCP_CMD_WLAN_ASYNC_EVENT | NCP_MSG_TYPE_EVENT | 0x00000009) /* wlan stop network */
/** Wi-Fi socket send fail event ID */
#define NCP_EVENT_WLAN_NCP_INET_SEND_FAIL   (NCP_CMD_WLAN | NCP_CMD_WLAN_ASYNC_EVENT | NCP_MSG_TYPE_EVENT | 0x0000000a) /* wlan stop network */

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_SOCKET_CFG
{
    uint32_t family;
    uint32_t style;
    uint32_t protocol;
} NCP_TLV_PACK_END NCP_CMD_INET_SOCKET_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_SOCKET_CFG
{
    uint32_t errorn;
    int32_t  ret;
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_SOCKET_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_CON_CFG
{
    uint32_t socket;
    uint32_t socklen;
    char sockaddr[128];
} NCP_TLV_PACK_END NCP_CMD_INET_CON_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_CON_CFG
{
    uint32_t errorn;
    int32_t  ret;
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_CON_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_BIND_CFG
{
    uint32_t socket;
    uint32_t socklen;
    char sockaddr[128];
} NCP_TLV_PACK_END NCP_CMD_INET_BIND_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_BIND_CFG
{
    uint32_t errorn;
    int32_t  ret;
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_BIND_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_CLOSE_CFG
{
    uint32_t socket;
} NCP_TLV_PACK_END NCP_CMD_INET_CLOSE_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_CLOSE_CFG
{
    uint32_t errorn;
    int32_t  ret;
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_CLOSE_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_SHUTDOWN_CFG
{
    uint32_t socket;
	uint32_t how;
} NCP_TLV_PACK_END NCP_CMD_INET_SHUTDOWN_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_SHUTDOWN_CFG
{
    uint32_t errorn;
    int32_t  ret;
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_SHUTDOWN_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_LISTEN_CFG
{
    uint32_t socket;
    uint32_t number;
} NCP_TLV_PACK_END NCP_CMD_INET_LISTEN_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_LISTEN_CFG
{
    uint32_t errorn;
    int32_t  ret;
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_LISTEN_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_ACCEPT_CFG
{
    uint32_t socket;
} NCP_TLV_PACK_END NCP_CMD_INET_ACCEPT_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_ACCEPT_CFG
{
    uint32_t errorn;
    int32_t  ret;
    uint32_t socklen;
    char sockaddr[128];
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_ACCEPT_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_SEND_CFG
{
    uint32_t socket;
    uint32_t size;
    uint32_t flags;
    char send_data[1];
} NCP_TLV_PACK_END NCP_CMD_INET_SEND_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_SEND_CFG
{
    uint32_t errorn;
    int32_t  ret;
    uint32_t socket;
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_SEND_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_SENDTO_CFG
{
    uint32_t socket;
    uint32_t size;
    uint32_t flags;
    uint32_t socklen;
    char sockaddr[128];
    char send_data[1];
} NCP_TLV_PACK_END NCP_CMD_INET_SENDTO_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_SENDTO_CFG
{
    uint32_t errorn;
    int32_t  ret;
    uint32_t send_size;
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_SENDTO_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RECV_CFG
{   
    uint32_t socket;
    uint32_t size;
    uint32_t flags;
} NCP_TLV_PACK_END NCP_CMD_INET_RECV_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_RECV_CFG
{
    uint32_t errorn;
    uint32_t ret;
    char recv_data[1];
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_RECV_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RECVFROM_CFG
{   
    uint32_t socket;
    uint32_t size;
    uint32_t flags;
} NCP_TLV_PACK_END NCP_CMD_INET_RECVFROM_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_RECVFROM_CFG
{
    uint32_t errorn;
    int32_t  ret;
    uint32_t socket;
    uint32_t recv_size;
    uint32_t socklen;
    char sockaddr[128];
    char recv_data[1];
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_RECVFROM_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_GETSOCKNAME_CFG
{   
    uint32_t socket;
} NCP_TLV_PACK_END NCP_CMD_INET_GETSOCKNAME_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_GETSOCKNAME_CFG
{
    uint32_t errorn;
    int32_t  ret;
    uint32_t socklen;
    char sockaddr[128];
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_GETSOCKNAME_CFG;
     
typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_GETPEERNAME_CFG
{   
    uint32_t socket;
} NCP_TLV_PACK_END NCP_CMD_INET_GETPEERNAME_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_GETPEERNAME_CFG
{
    uint32_t errorn;
    int32_t  ret;
    uint32_t socklen;
    char sockaddr[128];
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_GETPEERNAME_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_GETSOCKOPT_CFG
{   
    uint32_t socket;
    uint32_t level;
    uint32_t optname;
} NCP_TLV_PACK_END NCP_CMD_INET_GETSOCKOPT_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_GETSOCKOPT_CFG
{
    uint32_t errorn;
    int32_t  ret;
    uint32_t socklen;
    char optval[64];
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_GETSOCKOPT_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_SETSOCKOPT_CFG
{   
    uint32_t socket;
    int32_t level;
    int32_t optname;
    uint32_t socklen;
    char optval[64];
} NCP_TLV_PACK_END NCP_CMD_INET_SETSOCKOPT_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_SETSOCKOPT_CFG
{
    uint32_t errorn;
    int32_t  ret;
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_SETSOCKOPT_CFG;      

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_IOCTL_CFG
{   
    int fd;
    unsigned long cmd;
    uint8_t argp[256];
} NCP_TLV_PACK_END NCP_CMD_INET_IOCTL_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_IOCTL_CFG
{
    uint32_t errorno;
    int32_t  ret;
    uint8_t argp[256];
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_IOCTL_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_FCNTL_CFG
{   
    int fd;
    long cmd;
    int val;
} NCP_TLV_PACK_END NCP_CMD_INET_FCNTL_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_FCNTL_CFG
{
    uint32_t errorno;
    int32_t  ret;
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_FCNTL_CFG; 

/** NCP command structure. */
typedef NCP_TLV_PACK_START struct _NCPCmd_DS_INET_COMMAND
{
    /** Command header. */
    NCP_COMMAND header;
    /** Command body. */
    union
    {
        /** inet socket configuration */
        NCP_CMD_INET_SOCKET_CFG wlan_inet_socket;
        /** inet connect configuration */
        NCP_CMD_INET_CON_CFG wlan_inet_con;
        /** inet bind configuration */
        NCP_CMD_INET_BIND_CFG wlan_inet_bind;
        /** inet close configuration */
        NCP_CMD_INET_CLOSE_CFG wlan_inet_close;
        /** inet shutdown configuration */
        NCP_CMD_INET_SHUTDOWN_CFG wlan_inet_shutdown;
        /** inet listen configuration */
        NCP_CMD_INET_LISTEN_CFG wlan_inet_listen;
        /** inet accept configuration */
        NCP_CMD_INET_ACCEPT_CFG wlan_inet_accept;
        /** inet send configuration */
        NCP_CMD_INET_SEND_CFG wlan_inet_send;
        /** inet sendto configuration */
        NCP_CMD_INET_SENDTO_CFG wlan_inet_sendto;
        /** inet receive configuration */
        NCP_CMD_INET_RECV_CFG wlan_inet_recv;
        /** inet recvfrom configuration */
        NCP_CMD_INET_RECVFROM_CFG wlan_inet_recvfrom;
        /** inet getsockname configuration */
        NCP_CMD_INET_GETSOCKNAME_CFG wlan_inet_getsockname;
        /** inet getpeername configuration */
        NCP_CMD_INET_GETPEERNAME_CFG wlan_inet_getpeername;
        /** inet getsockopt configuration */
        NCP_CMD_INET_GETSOCKOPT_CFG wlan_inet_getsockopt;
        /** inet setsockopt configuration */
        NCP_CMD_INET_SETSOCKOPT_CFG wlan_inet_setsockopt;
        /** inet ioctl configuration */
        NCP_CMD_INET_IOCTL_CFG wlan_inet_ioctl;
        /** inet fcntl configuration */
        NCP_CMD_INET_FCNTL_CFG wlan_inet_fcntl;

        /** inet socket resp configuration */
        NCP_CMD_INET_RESP_SOCKET_CFG wlan_inet_resp_socket;
        /** inet connect resp configuration */
        NCP_CMD_INET_RESP_CON_CFG wlan_inet_resp_con;
        /** inet bind resp configuration */
        NCP_CMD_INET_RESP_BIND_CFG wlan_inet_resp_bind;
        /** inet close resp configuration */
        NCP_CMD_INET_RESP_CLOSE_CFG wlan_inet_resp_close;
        /** inet shutdown resp configuration */
        NCP_CMD_INET_RESP_SHUTDOWN_CFG wlan_inet_resp_shutdown;
        /** inet listen resp configuration */
        NCP_CMD_INET_RESP_LISTEN_CFG wlan_inet_resp_listen;
        /** inet accept resp configuration */
        NCP_CMD_INET_RESP_ACCEPT_CFG wlan_inet_resp_accept;
        /** inet send resp configuration */
        NCP_CMD_INET_RESP_SEND_CFG wlan_inet_resp_send;
        /** inet sendto resp configuration */
        NCP_CMD_INET_RESP_SENDTO_CFG wlan_inet_resp_sendto;
        /** inet receive resp configuration */
        NCP_CMD_INET_RESP_RECV_CFG wlan_inet_resp_recv;
        /** inet recvfrom resp configuration */
        NCP_CMD_INET_RESP_RECVFROM_CFG wlan_inet_resp_recvfrom;
        /** inet getsockname resp configuration */
        NCP_CMD_INET_RESP_GETSOCKNAME_CFG wlan_inet_resp_getsockname;
        /** inet getpeername resp configuration */
        NCP_CMD_INET_RESP_GETPEERNAME_CFG wlan_inet_resp_getpeername;
        /** inet getsockopt resp configuration */
        NCP_CMD_INET_RESP_GETSOCKOPT_CFG wlan_inet_resp_getsockopt;
        /** inet setsockopt resp configuration */
        NCP_CMD_INET_RESP_SETSOCKOPT_CFG wlan_inet_resp_setsockopt;
        /** inet ioctl resp configuration */
        NCP_CMD_INET_RESP_IOCTL_CFG wlan_inet_resp_ioctl;
        /** inet fcntl resp configuration */
        NCP_CMD_INET_RESP_FCNTL_CFG wlan_inet_resp_fcntl;
    } params;
} NCP_TLV_PACK_END NCPCmd_DS_INET_COMMAND;

void ncp_inet_init();
void ncp_inet_deinit();

int ncp_socket(int family, int style, int protocol);
int ncp_bind(int socket, const struct sockaddr *addr, socklen_t length);
int ncp_connect(int socket, const struct sockaddr *addr, socklen_t length);
int ncp_listen(int socket, int number);
int ncp_accept(int socket, struct sockaddr *addr, socklen_t *length);
int ncp_getsockname(int socket, struct sockaddr *addr, socklen_t *length);
int ncp_getpeername(int socket, struct sockaddr *addr, socklen_t *length);
int ncp_getsockopt(int socket, int level, int optname, void *optval, socklen_t *optlen);
int ncp_setsockopt(int socket, int level, int optname, const void *optval, socklen_t optlen);
int ncp_send(int socket, const void *buffer, size_t size, int flags);
int ncp_sendto(int socket, const void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t length);
int ncp_recv(int socket, void *buffer, size_t size, int flags);
int ncp_recvfrom(int socket, void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t *length_ptr);
int ncp_close(int socket);
int ncp_shutdown(int s, int how);

int inet_sock_recv_event(uint8_t *res);
int inet_sock_send_fail_event(uint8_t *res);

int ncp_ioctl(int fd, long cmd, void *argp);
int ncp_fcntl(int fd, int cmd, int val);

#define INET_SOCKET_RECV_QUEUE_NAME "/ncp_enhance_receive"

#define NCP_INET_SOCK_NUMBER    64
struct ncp_socket_recv_t
{
    int socket;
    int socket_type;
    StreamBufferHandle_t rx_fifo_fd;
    uint32_t socklen;
    struct sockaddr addr;
    char name[64];
    int read_errorn;
    int write_errorn;
    int recv_timeout_ms;
};


#define STREAM_BUFFER_SIZE 16*1500

#define NCP_SOCKET_FD_OFFSET 128
#define socketfd_lwip_to_host(fd) ((fd) + NCP_SOCKET_FD_OFFSET)
#define socketfd_host_to_lwip(fd) ((fd) - NCP_SOCKET_FD_OFFSET)

#endif
