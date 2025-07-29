/*!\file ncp_inet.h
 *\brief NCP INET Socket command/response definitions.
 */
/*
 *  Copyright 2025 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __NCP_CMD_WIFI__
#define __NCP_CMD_WIFI__
#include <wm_net.h>
#include "ncp_glue_wifi.h"

/** WLAN Inet command */
/** Wi-Fi inet socket command ID */
#define NCP_CMD_WLAN_INET_SOCKET      (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000001) /* wlan-socket-open */
/** Wi-Fi inet socket command response ID */
#define NCP_RSP_WLAN_INET_SOCKET      (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x00000001)
/** Wi-Fi inet connect command ID */
#define NCP_CMD_WLAN_INET_CON       (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000002) /* wlan-socket-connect */
/** Wi-Fi inet connect command response ID */
#define NCP_RSP_WLAN_INET_CON       (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x00000002)
/** Wi-Fi inet recvfrom command ID */
#define NCP_CMD_WLAN_INET_RECVFROM  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000004)
/** Wi-Fi inet sendto command ID */
#define NCP_CMD_WLAN_INET_SENDTO    (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000006) /* wlan-socket-sendto */
/** Wi-Fi inet bind command ID */
#define NCP_CMD_WLAN_INET_BIND      (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000007) /* wlan-socket-bind */
/** Wi-Fi inet bind command response ID */
#define NCP_RSP_WLAN_INET_BIND      (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x00000007)
/** Wi-Fi inet listen command ID */
#define NCP_CMD_WLAN_INET_LISTEN    (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000008) /* wlan-socket-listen */
/** Wi-Fi inet listen command response ID */
#define NCP_RSP_WLAN_INET_LISTEN    (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x00000008)
/** Wi-Fi inet accept command ID */
#define NCP_CMD_WLAN_INET_ACCEPT    (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000009) /* wlan-socket-accept */
/** Wi-Fi inet accept command response ID */
#define NCP_RSP_WLAN_INET_ACCEPT    (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x00000009)
/** Wi-Fi inet close command ID */
#define NCP_CMD_WLAN_INET_CLOSE     (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x0000000a) /* wlan-socket-close */
/** Wi-Fi inet close command response ID */
#define NCP_RSP_WLAN_INET_CLOSE     (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x0000000a)
/** Wi-Fi inet getsockname from command ID */
#define NCP_CMD_WLAN_INET_GETSOCKNAME  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x0000000b) /* wlan-socket-getsockname */
/** Wi-Fi inet getsockname from command response ID */
#define NCP_RSP_WLAN_INET_GETSOCKNAME  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x0000000b)
/** Wi-Fi inet getpeername from command ID */
#define NCP_CMD_WLAN_INET_GETPEERNAME  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x0000000c) /* wlan-socket-getpeername */
/** Wi-Fi inet getpeername from command response ID */
#define NCP_RSP_WLAN_INET_GETPEERNAME  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x0000000c)
/** Wi-Fi inet getsockopt from command ID */
#define NCP_CMD_WLAN_INET_GETSOCKOPT  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x0000000d) /* wlan-socket-getsockopt */
/** Wi-Fi inet getsockopt from command response ID */
#define NCP_RSP_WLAN_INET_GETSOCKOPT  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x0000000d)
/** Wi-Fi inet setsockopt from command ID */
#define NCP_CMD_WLAN_INET_SETSOCKOPT  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x0000000e) /* wlan-socket-setsockopt */
/** Wi-Fi inet setsockopt from command response ID */
#define NCP_RSP_WLAN_INET_SETSOCKOPT  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x0000000e)
/** Wi-Fi inet ioctl from command ID */
#define NCP_CMD_WLAN_INET_IOCTL  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x0000000f) /* wlan-socket-ioctl */
/** Wi-Fi inet ioctl from command response ID */
#define NCP_RSP_WLAN_INET_IOCTL  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x0000000f)
/** Wi-Fi inet fcntl from command ID */
#define NCP_CMD_WLAN_INET_FCNTL  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_CMD | 0x00000010) /* wlan-socket-fcntl */
/** Wi-Fi inet fcnt from command response ID */
#define NCP_RSP_WLAN_INET_FCNTL  (NCP_CMD_WLAN | NCP_CMD_WLAN_INET | NCP_MSG_TYPE_RESP | 0x00000010)

/** Wi-Fi inet recv EVENT ID */
#define NCP_EVENT_WLAN_NCP_INET_RECV        (NCP_CMD_WLAN | NCP_CMD_WLAN_ASYNC_EVENT | NCP_MSG_TYPE_EVENT | 0x00000009) /* wlan stop network */
/** Wi-Fi inet fcntl EVENT ID */
#define NCP_EVENT_WLAN_NCP_INET_SEND_FAIL   (NCP_CMD_WLAN | NCP_CMD_WLAN_ASYNC_EVENT | NCP_MSG_TYPE_EVENT | 0x0000000a) /* wlan stop network */


/** This structure is used for inet socket configuration. */

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_SOCKET_CFG
{
    uint32_t family;
    uint32_t style;
    uint32_t protocol;
} NCP_TLV_PACK_END NCP_CMD_INET_SOCKET_CFG;
typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_SOCKET_CFG
{
    uint32_t errno;
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
    uint32_t errno;
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
    uint32_t errno;
    int32_t  ret;
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_BIND_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_CLOSE_CFG
{
    uint32_t socket;
} NCP_TLV_PACK_END NCP_CMD_INET_CLOSE_CFG;
typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_CLOSE_CFG
{
    uint32_t errno;
    int32_t  ret;
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_CLOSE_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_LISTEN_CFG
{
    uint32_t socket;
    uint32_t number;
} NCP_TLV_PACK_END NCP_CMD_INET_LISTEN_CFG;
typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_LISTEN_CFG
{
    uint32_t errno;
    int32_t  ret;
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_LISTEN_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_ACCEPT_CFG
{
    uint32_t socket;
} NCP_TLV_PACK_END NCP_CMD_INET_ACCEPT_CFG;
typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_ACCEPT_CFG
{
    uint32_t errno;
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
    uint32_t errno;
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
    uint32_t errno;
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
    uint32_t errno;
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
    uint32_t errno;
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
    uint32_t errno;
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
    uint32_t errno;
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
    uint32_t errno;
    int32_t  ret;
    uint32_t socklen;
    char optval[64];
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_GETSOCKOPT_CFG;

typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_SETSOCKOPT_CFG
{   
    uint32_t socket;
    uint32_t level;
    uint32_t optname;
    uint32_t socklen;
    char optval[64];
} NCP_TLV_PACK_END NCP_CMD_INET_SETSOCKOPT_CFG;
typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_SETSOCKOPT_CFG
{
    uint32_t errno;
    int32_t  ret;
} NCP_TLV_PACK_END NCP_CMD_INET_RESP_SETSOCKOPT_CFG;      

/* system control */
typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_IOCTL_CFG
{
    int fd;
    unsigned long cmd;
    uint8_t argp[256];
} NCP_TLV_PACK_END NCP_CMD_INET_IOCTL_CFG;
typedef NCP_TLV_PACK_START struct _NCP_CMD_INET_RESP_IOCTL_CFG
{
    uint32_t errno;
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
    uint32_t errno;
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
        NCP_CMD_INET_RESP_SOCKET_CFG inet_open;
        /** inet connect configuration */
        NCP_CMD_INET_RESP_CON_CFG inet_con;
        /** inet bind configuration */
        NCP_CMD_INET_RESP_BIND_CFG inet_bind;
        /** inet close configuration */
        NCP_CMD_INET_RESP_CLOSE_CFG inet_close;
        /** inet listen configuration */
        NCP_CMD_INET_RESP_LISTEN_CFG inet_listen;
        /** inet accept configuration */
        NCP_CMD_INET_RESP_ACCEPT_CFG inet_accept;
        /** inet send configuration */
        NCP_CMD_INET_RESP_SEND_CFG inet_send;
        /** inet sendto configuration */
        NCP_CMD_INET_RESP_SENDTO_CFG inet_sendto;
        /** inet receive configuration */
        NCP_CMD_INET_RESP_RECV_CFG inet_recv;
        /** inet recvfrom configuration */
        NCP_CMD_INET_RESP_RECVFROM_CFG inet_recvfrom;
        /** inet getsockname configuration */
        NCP_CMD_INET_RESP_GETSOCKNAME_CFG inet_getsockname;
        /** inet getpeername configuration */
        NCP_CMD_INET_RESP_GETPEERNAME_CFG inet_getpeername;
        /** inet getsockopt configuration */
        NCP_CMD_INET_RESP_GETSOCKOPT_CFG inet_getsockopt;
        /** inet setsockopt configuration */
        NCP_CMD_INET_RESP_SETSOCKOPT_CFG inet_setsockopt;
        /** inet ioctl configuration */
        NCP_CMD_INET_RESP_IOCTL_CFG inet_ioctl;
        /** inet fcntl configuration */
        NCP_CMD_INET_RESP_FCNTL_CFG inet_fcntl;
    } params;
} NCP_TLV_PACK_END NCPCmd_DS_INET_COMMAND, MCU_NCPCmd_DS_INET_COMMAND;

typedef unsigned short int linux_sa_family_t;
#define NCP_IPADDR_DATA_LEN   (2+3*4+3*4)
struct linux_sockaddr {
    linux_sa_family_t	sa_family;	/* address family, AF_xxx	*/
    char        sa_data[2];
    u32_t       sa_data2[3];
    u32_t       sa_data3[3];
};

union ncp_sockaddr_aligned {
    struct sockaddr sa;
    struct sockaddr_in6 sin6;
    struct sockaddr_in sin;
};

uint8_t *ncp_inet_prepare_socket_recv_resp(uint8_t *receive_res);
uint8_t *ncp_inet_prepare_socket_send_fail(uint8_t *send_fail);

int ncp_inet_init(void);
void ncp_inet_deinit(void);

#define LINUX_SOL_SOCKET      1
#define LINUX_SO_REUSEADDR    2
#define LINUX_SO_REUSEPORT    15
#define LINUX_SO_BINDTODEVICE 25
#define LINUX_SO_BROADCAST    6
#define LINUX_IPV6_V6ONLY     26
#define LINUX_IP_MULTICAST_IF   32
#define LINUX_IP_MULTICAST_TTL  33

#if CONFIG_NCP_USB || CONFIG_NCP_SDIO
#define NCP_INET_SOCKET_RECV_SIZE 1472
#else
/* ip fragment doesn't include 8bytes udp header, so fragment packet size is 1480 */
#define NCP_INET_SOCKET_RECV_SIZE (1472 + 1480)
#endif
#define SOCKET_SELECT_TASK_NAME "socket recv task"

#endif
