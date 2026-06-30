/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Workaround for CAAM DMA / BT UART DMA conflict in Wi-Fi + BT coex mode.
 *
 * Problem: When PSA crypto (CAAM hardware) is used for BT AES encryption
 * (bt_encrypt_le), the CAAM DMA operation corrupts/blocks the BT UART DMA,
 * causing the BT controller to become permanently unresponsive.
 *
 * Solution: Use linker --wrap to redirect bt_encrypt_le calls to this function
 * which uses HCI_LE_Encrypt command (opcode 0x2017) instead of CAAM hardware.
 */

#include <stdint.h>
#include <string.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/net_buf.h>
#include <zephyr/sys/byteorder.h>

struct bt_hci_cp_le_encrypt_coex {
    uint8_t key[16];
    uint8_t plaintext[16];
} __packed;

struct bt_hci_rp_le_encrypt_coex {
    uint8_t status;
    uint8_t enc_data[16];
} __packed;

#define BT_HCI_OP_LE_ENCRYPT_COEX BT_OP(BT_OGF_LE, 0x0017)

extern struct net_buf *bt_hci_cmd_alloc(k_timeout_t timeout);
extern int bt_hci_cmd_send_sync(uint16_t opcode, struct net_buf *buf, struct net_buf **rsp);

/* __wrap_bt_encrypt_le: called instead of bt_encrypt_le via linker --wrap */
int __wrap_bt_encrypt_le(const uint8_t key[16], const uint8_t plaintext[16],
                         uint8_t enc_data[16])
{
    struct net_buf *buf, *rsp;
    struct bt_hci_cp_le_encrypt_coex *cp;
    struct bt_hci_rp_le_encrypt_coex *rp;
    int err;

    if (key == NULL || plaintext == NULL || enc_data == NULL) {
        return -EINVAL; /* -EINVAL */
    }

    buf = bt_hci_cmd_alloc(K_FOREVER);
    if (!buf) {
        return -ENOBUFS; /* -ENOBUFS */
    }

    cp = net_buf_add(buf, sizeof(*cp));

    /* HCI LE Encrypt expects big-endian key and plaintext */
    sys_memcpy_swap(cp->key, key, 16);
    sys_memcpy_swap(cp->plaintext, plaintext, 16);

    err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_ENCRYPT_COEX, buf, &rsp);
    if (err) {
        return err;
    }

    rp = (void *)rsp->data;
    /* Response is big-endian, convert back to little-endian */
    sys_memcpy_swap(enc_data, rp->enc_data, 16);
    net_buf_unref(rsp);

    return 0;
}
