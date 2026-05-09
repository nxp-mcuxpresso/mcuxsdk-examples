#!/usr/bin/env python3
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

"""
eSPI Bus Capture Tool

Passive eSPI bus monitor using Promira Analyzer.
Replaces Data Center GUI for command-line capture and protocol decode.

Usage:
    python espi_capture.py <ANALYZER_IP>
    python espi_capture.py <ANALYZER_IP> --io-mode quad
    python espi_capture.py <ANALYZER_IP> --filter vw,oob
    python espi_capture.py <ANALYZER_IP> --packets-only
    python espi_capture.py <ANALYZER_IP> --raw
"""

import sys, os, time, signal, argparse

_SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, _SCRIPT_DIR)
if hasattr(os, 'add_dll_directory'):
    os.add_dll_directory(_SCRIPT_DIR)

# ---------------------------------------------------------------------------
# ANSI colors
# ---------------------------------------------------------------------------
class _C:
    DIM = '\033[2m'; BOLD = '\033[1m'; CYAN = '\033[36m'
    YELLOW = '\033[33m'; GREEN = '\033[32m'; RED = '\033[31m'
    MAGENTA = '\033[35m'; R = '\033[0m'

_use_color = True

def _co(code, text):
    return f'{code}{text}{_C.R}' if _use_color else str(text)

# ---------------------------------------------------------------------------
# eSPI protocol tables
# ---------------------------------------------------------------------------
OPCODE_NAMES = {
    0x00: 'PUT_PC',    0x01: 'GET_PC',
    0x02: 'PUT_NP',    0x03: 'GET_NP',
    0x04: 'PUT_VW',    0x05: 'GET_VW',
    0x06: 'PUT_OOB',   0x07: 'GET_OOB',
    0x08: 'PUT_FLASH_C', 0x09: 'GET_FLASH_NP',
    0x0A: 'PUT_FLASH_NP', 0x0B: 'GET_FLASH_C',
    0x21: 'GET_CONFIG', 0x22: 'SET_CONFIG',
    0x25: 'GET_STATUS',
    0x40: 'IORD_S1',   0x41: 'IORD_S2',   0x43: 'IORD_S4',
    0x44: 'IOWR_S1',   0x45: 'IOWR_S2',   0x47: 'IOWR_S4',
    0x48: 'MEMRD32_S1', 0x4B: 'MEMRD32_S4',
    0x4C: 'MEMWR32_S1', 0x4F: 'MEMWR32_S4',
    0xFF: 'RESET',
}

CHANNEL_NAMES = {0: 'Peripheral', 1: 'VWire', 2: 'OOB', 3: 'Flash', 4: 'Control'}

CRC_ERR_CMD = 0x100; CRC_ERR_RSP = 0x200

STATUS_BITS = [
    (0, 'PC_FREE'), (1, 'NP_FREE'), (2, 'VWIRE_FREE'), (3, 'OOB_FREE'),
    (4, 'PC_AVAIL'), (5, 'NP_AVAIL'), (6, 'VWIRE_AVAIL'), (7, 'OOB_AVAIL'),
    (8, 'FLASH_C_FREE'), (9, 'FLASH_NP_FREE'),
    (12, 'FLASH_C_AVAIL'), (13, 'FLASH_NP_AVAIL'),
]

VWIRE_SIGNALS = {
    0x02: ['SLP_S3#', 'SLP_S4#', 'SLP_S5#'],
    0x03: ['SUS_STAT#', 'PLTRST#', 'OOB_RST_WARN'],
    0x04: ['PME#', 'WAKE#', 'OOB_RST_ACK'],
    0x05: ['BOOT_DONE', 'BOOT_ERR', 'ERR_NONFATAL', 'ERR_FATAL'],
    0x06: ['SCI#', 'SMI#', 'RCIN#', 'HOST_RST_ACK'],
    0x07: ['HOST_RST_WARN', 'SMIOUT#', 'NMIOUT#'],
    0x40: ['SUS_ACK#'],
    0x41: ['SUS_WARN#', 'SUS_PWRDN_ACK', 'SLP_A#', 'SLP_LAN#'],
    0x42: ['SLP_WLAN#'],
    0x47: ['HOST_C10'],
}

PERIF_CYCLES = {
    0x00: 'MEMRD32', 0x01: 'MEMWR32', 0x02: 'MEMRD64', 0x03: 'MEMWR64',
    0x04: 'MSG', 0x05: 'MSG_DATA',
    0x06: 'SC', 0x07: 'SC_DATA', 0x0F: 'SC_DATA',
    0x09: 'UC', 0x0A: 'UC_DATA',
}

OOB_CYCLES = {0x21: 'OOB'}

FLASH_CYCLES = {0x00: 'FLASH_RD', 0x01: 'FLASH_WR', 0x02: 'FLASH_ER'}

# ---------------------------------------------------------------------------
# Decode helpers
# ---------------------------------------------------------------------------
def _hex(data, start, end):
    return ' '.join(f'{data[i]:02X}' for i in range(start, min(end, len(data))))

def _hex_list(data):
    if len(data) > 16:
        return _hex(data, 0, 8) + ' ... ' + _hex(data, len(data) - 4, len(data))
    return _hex(data, 0, len(data))

def _rsp_name(b):
    if b == 0xFF:
        return 'NO_RESPONSE'
    code = b & 0x0F
    if code == 0x08: return 'ACCEPT'
    if code == 0x01: return 'DEFER'
    if code == 0x02: return 'NON_FATAL_ERR'
    if code == 0x03: return 'FATAL_ERR'
    if code == 0x0F: return 'WAIT_STATE'
    return f'UNKNOWN(0x{b:02X})'

def _status_flags(b, byte_idx):
    flags = []
    for bit_pos, name in STATUS_BITS:
        local = bit_pos - byte_idx * 8
        if 0 <= local < 8 and (b >> local) & 1:
            flags.append(name)
    return ' '.join(flags)

def _vwire_signals(idx, data_byte):
    valid = (data_byte >> 4) & 0x0F
    state = data_byte & 0x0F
    sigs = VWIRE_SIGNALS.get(idx, [])
    parts = []
    for i, name in enumerate(sigs):
        if valid & (1 << i):
            parts.append(f'{name}={(state >> i) & 1}')
    return ' '.join(parts)

# ---------------------------------------------------------------------------
# CMD decode
# ---------------------------------------------------------------------------
def _decode_cmd(opcode, data, cmd_len):
    lines = []
    op_name = OPCODE_NAMES.get(opcode, f'0x{opcode:02X}')

    if opcode == 0x25:  # GET_STATUS
        lines.append(f'op=GET_STATUS(0x25)')

    elif opcode == 0x04:  # PUT_VW
        count_raw = data[1] if cmd_len > 1 else 0
        lines.append(f'op=PUT_VW(0x04) count=0x{count_raw:02X}({count_raw + 1} groups)')
        for i in range(2, cmd_len - 1, 2):
            if i + 1 < cmd_len - 1:
                idx, val = data[i], data[i + 1]
                sig = _vwire_signals(idx, val)
                sig_str = f'  ({sig})' if sig else ''
                lines.append(f'idx=0x{idx:02X} data=0x{val:02X}{sig_str}')

    elif opcode == 0x05:  # GET_VW
        count = data[1] if cmd_len > 1 else 0
        lines.append(f'op=GET_VW(0x05) request_count=0x{count:02X}')

    elif opcode == 0x22:  # SET_CONFIG
        if cmd_len >= 8:
            offset = (data[1] << 8) | data[2]
            cfg = ', '.join(f'0x{data[i]:02X}' for i in range(3, 7))
            lines.append(f'op=SET_CONFIG(0x22) offset=0x{offset:02X} config=[{cfg}]')
        else:
            lines.append(f'op=SET_CONFIG(0x22)')

    elif opcode == 0x21:  # GET_CONFIG
        if cmd_len >= 4:
            offset = (data[1] << 8) | data[2]
            lines.append(f'op=GET_CONFIG(0x21) offset=0x{offset:02X}')
        else:
            lines.append(f'op=GET_CONFIG(0x21)')

    elif opcode in (0x40, 0x41, 0x43, 0x44, 0x45, 0x47,
                    0x48, 0x4B, 0x4C, 0x4F):  # Short IO / MEM
        if cmd_len >= 4:
            addr = (data[1] << 8) | data[2]
            line = f'op={op_name}(0x{opcode:02X}) addr=0x{addr:04X}'
            if opcode >= 0x44 and cmd_len > 4:
                payload = list(data[3:cmd_len - 1])
                line += f' data=[{_hex_list(payload)}]'
            lines.append(line)
        else:
            lines.append(f'op={op_name}(0x{opcode:02X})')

    elif opcode in (0x00, 0x01, 0x02, 0x03):  # PUT_PC/GET_PC/PUT_NP/GET_NP
        if cmd_len >= 8:
            cycle = data[1]
            tag = (data[2] >> 4) & 0x0F
            length = ((data[2] & 0x0F) << 8) | data[3]
            addr = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7]
            cy_name = PERIF_CYCLES.get(cycle, f'0x{cycle:02X}')
            lines.append(f'op={op_name}(0x{opcode:02X}) cycle={cy_name}(0x{cycle:02X})'
                         f' tag=0x{tag:02X} len=0x{length:02X} addr=0x{addr:08X}')
            if cmd_len > 9:
                payload = list(data[8:cmd_len - 1])
                lines.append(f'data=[{_hex_list(payload)}]')
        else:
            lines.append(f'op={op_name}(0x{opcode:02X})')

    elif opcode == 0x06:  # PUT_OOB
        if cmd_len >= 5:
            cycle = data[1]
            tag = (data[2] >> 4) & 0x0F
            length = ((data[2] & 0x0F) << 8) | data[3]
            cy_name = OOB_CYCLES.get(cycle, f'0x{cycle:02X}')
            lines.append(f'op=PUT_OOB(0x06) cycle={cy_name}(0x{cycle:02X})'
                         f' tag=0x{tag:X} len=0x{length:02X}')
            if cmd_len > 7:
                payload = list(data[4:cmd_len - 1])
                if len(payload) >= 3:
                    lines.append(f'slave_addr=0x{payload[0]:02X}'
                                 f' cmd_code=0x{payload[1]:02X}'
                                 f' byte_count=0x{payload[2]:02X}'
                                 f' data=[{_hex_list(payload[3:])}]')
        else:
            lines.append(f'op=PUT_OOB(0x06)')

    elif opcode == 0x07:  # GET_OOB
        lines.append(f'op=GET_OOB(0x07)')

    elif opcode in (0x08, 0x09, 0x0A):  # Flash
        if cmd_len >= 8:
            cycle = data[1]
            tag = (data[2] >> 4) & 0x0F
            length = ((data[2] & 0x0F) << 8) | data[3]
            addr = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7]
            cy_name = FLASH_CYCLES.get(cycle, f'0x{cycle:02X}')
            lines.append(f'op={op_name}(0x{opcode:02X}) cycle={cy_name}(0x{cycle:02X})'
                         f' tag=0x{tag:02X} addr=0x{addr:08X} len=0x{length:02X}')
            if cmd_len > 9:
                payload = list(data[8:cmd_len - 1])
                lines.append(f'data=[{_hex_list(payload)}]')
        else:
            lines.append(f'op={op_name}(0x{opcode:02X})')

    else:
        lines.append(f'op={op_name}(0x{opcode:02X})')

    return lines

# ---------------------------------------------------------------------------
# RSP decode
# ---------------------------------------------------------------------------
def _decode_rsp(opcode, data, rsp_start, rsp_end):
    lines = []
    rsp_len = rsp_end - rsp_start
    if rsp_len < 1:
        return lines

    rsp_code = data[rsp_start]
    name = _rsp_name(rsp_code)
    is_accept = (rsp_code & 0x0F) == 0x08
    if is_accept:
        lines.append(f'{_co(_C.GREEN, f"{rsp_code:02X}")} - {_co(_C.GREEN, name)}')
    else:
        lines.append(f'{_co(_C.RED, f"{rsp_code:02X}")} - {_co(_C.RED, name)}')

    if rsp_len < 2:
        return lines

    # GET_VW response: rsp + count + [idx, data]... (no status)
    if opcode == 0x05:
        count = data[rsp_start + 1]
        lines.append(f'count=0x{count:02X}')
        for i in range(count + 1):
            pos = rsp_start + 2 + i * 2
            if pos + 1 < rsp_end:
                idx, val = data[pos], data[pos + 1]
                sig = _vwire_signals(idx, val)
                sig_str = f'  ({sig})' if sig else ''
                lines.append(f'idx=0x{idx:02X} data=0x{val:02X}{sig_str}')
        return lines

    # GET_VW has no status (handled above); GET_OOB/Flash completions are complex
    if opcode in (0x07,):
        return lines

    # PUT_NP completion response: rsp + cycle + tag_len + len_lo + data... + status + CRC
    if opcode == 0x02 and rsp_len >= 5:
        cycle = data[rsp_start + 1]
        tag = (data[rsp_start + 2] >> 4) & 0x0F
        length = ((data[rsp_start + 2] & 0x0F) << 8) | data[rsp_start + 3]
        cy_name = PERIF_CYCLES.get(cycle, f'0x{cycle:02X}')
        lines.append(f'{data[rsp_start + 1]:02X} - cycle={cy_name}(0x{cycle:02X})')
        lines.append(f'{data[rsp_start + 2]:02X} {data[rsp_start + 3]:02X}'
                     f' - tag=0x{tag:02X} len=0x{length:02X}')
        data_start = rsp_start + 4
        data_end = rsp_end - 2
        if data_end > data_start:
            payload = list(data[data_start:data_end])
            lines.append(f'data=[{_hex_list(payload)}]')
        _append_status(lines, data, rsp_end - 2, rsp_end)
        return lines

    # IORD response: rsp + data + status
    if opcode in (0x40, 0x41, 0x43):
        data_len = {0x40: 1, 0x41: 2, 0x43: 4}.get(opcode, 1)
        pos = rsp_start + 1
        if pos + data_len <= rsp_end:
            payload = list(data[pos:pos + data_len])
            lines.append(f'{_hex(data, pos, pos + data_len)}'
                         f' - data=[{_hex_list(payload)}]')

    # Status: last 2 bytes before CRC
    _append_status(lines, data, rsp_end - 2, rsp_end)
    return lines


def _append_status(lines, data, lo_pos, hi_end):
    if lo_pos < 0 or lo_pos + 1 >= len(data):
        return
    lo, hi = data[lo_pos], data[lo_pos + 1]
    lo_flags = _status_flags(lo, 0)
    hi_flags = _status_flags(hi, 1)
    if lo_flags:
        lines.append(f'{lo:02X} - {lo_flags}')
    if hi_flags:
        lines.append(f'{hi:02X} - {hi_flags}')

# ---------------------------------------------------------------------------
# Packet / Event formatting
# ---------------------------------------------------------------------------
def format_event(info, events, show_alerts=False):
    parts = []
    if events & 0x00080000:  # RESET_FALLING
        parts.append('RESET↓')
    if events & 0x00040000:  # RESET_RISING
        parts.append('RESET↑')
    if events & 0x00100000:  # INBAND_RESET
        parts.append('INBAND RESET')
    if show_alerts:
        if events & 0x00020000:  # ALERT_FALLING
            parts.append('ALERT↓')
        if events & 0x00010000:  # ALERT_RISING
            parts.append('ALERT↑')
    return '  ' + _co(_C.MAGENTA, '---- ' + ' | '.join(parts)) if parts else None


def format_packet(index, info, pkt_info, data, raw_mode, show_ts, t0):
    lines = []
    opcode = data[0] if len(data) > 0 else 0
    op_name = OPCODE_NAMES.get(opcode, f'0x{opcode:02X}')
    ch_name = CHANNEL_NAMES.get(pkt_info.channel, 'Unknown')
    cmd_len = pkt_info.cmd_length
    total = pkt_info.length
    cmd_crc_err = bool(info.status & CRC_ERR_CMD)
    rsp_crc_err = bool(info.status & CRC_ERR_RSP)

    ts = ''
    if show_ts and t0 is not None:
        ts = f' {(info.timestamp - t0) / 1e9:.3f}s'

    lines.append(f'{_co(_C.DIM, f"#{index:03d}")}{ts}'
                 f' {_co(_C.BOLD, op_name)} {_co(_C.CYAN, f"[{ch_name}]")}')

    # CMD
    if cmd_len > 0:
        cmd_hex = _hex(data, 0, cmd_len - 1)
        crc = f'{data[cmd_len - 1]:02X}'
        crc_str = _co(_C.RED, f'({crc}*)') if cmd_crc_err else f'({crc})'
        lines.append(f'  cmd: {_co(_C.YELLOW, cmd_hex)} {crc_str}')
        if not raw_mode:
            for dl in _decode_cmd(opcode, data, cmd_len):
                lines.append(f'    {dl}')
            if cmd_crc_err:
                lines.append(f'    {_co(_C.RED, "*** CMD CRC ERROR ***")}')

    # RSP
    rsp_start = cmd_len
    rsp_len = total - cmd_len
    if rsp_len > 0:
        rsp_hex = _hex(data, rsp_start, total - 1)
        crc = f'{data[total - 1]:02X}'
        crc_str = _co(_C.RED, f'({crc}*)') if rsp_crc_err else f'({crc})'
        lines.append(f'  rsp: {_co(_C.GREEN, rsp_hex)} {crc_str}')
        if not raw_mode:
            rsp_end = total - 1  # exclude CRC
            for dl in _decode_rsp(opcode, data, rsp_start, rsp_end):
                lines.append(f'    {dl}')
            if rsp_crc_err:
                lines.append(f'    {_co(_C.RED, "*** RSP CRC ERROR ***")}')

    return '\n'.join(lines)

# ---------------------------------------------------------------------------
# CSV output
# ---------------------------------------------------------------------------
def csv_header():
    return 'index,opcode,channel,cmd_hex,rsp_hex,rsp_code,cmd_crc_err,rsp_crc_err'

def csv_line(index, info, pkt_info, data):
    opcode = data[0] if len(data) > 0 else 0
    op_name = OPCODE_NAMES.get(opcode, f'0x{opcode:02X}')
    ch_name = CHANNEL_NAMES.get(pkt_info.channel, '?')
    cmd_len = pkt_info.cmd_length
    total = pkt_info.length
    cmd_hex = _hex(data, 0, cmd_len)
    rsp_hex = _hex(data, cmd_len, total)
    rsp_code = _rsp_name(data[cmd_len]) if total > cmd_len else ''
    cmd_crc = 1 if info.status & CRC_ERR_CMD else 0
    rsp_crc = 1 if info.status & CRC_ERR_RSP else 0
    return f'{index},{op_name},{ch_name},{cmd_hex},{rsp_hex},{rsp_code},{cmd_crc},{rsp_crc}'

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
_running = True

def _sigint(sig, frame):
    global _running
    _running = False

def main():
    global _use_color, _running

    parser = argparse.ArgumentParser(description='eSPI Bus Capture Tool')
    parser.add_argument('ip', help='Analyzer Promira IP')
    parser.add_argument('--io-mode', choices=['single', 'dual', 'quad'],
                        default='single')
    parser.add_argument('--filter', default=None,
                        help='Channel filter: peripheral,vw,oob,flash (comma-separated)')
    parser.add_argument('--packets-only', action='store_true',
                        help='Hide all events (ALERT/RESET)')
    parser.add_argument('--show-alerts', action='store_true',
                        help='Show ALERT events (hidden by default)')
    parser.add_argument('--raw', action='store_true',
                        help='Disable protocol decode')
    parser.add_argument('--timestamp', action='store_true',
                        help='Show elapsed time')
    parser.add_argument('--csv', default=None, metavar='FILE',
                        help='Export CSV to file')
    parser.add_argument('--no-color', action='store_true')
    args = parser.parse_args()

    if args.no_color:
        _use_color = False

    io_mode_map = {'single': 0, 'dual': 2, 'quad': 4}
    io_mode = io_mode_map[args.io_mode]

    # --- Connect ---
    from promira_py import pm_open, pm_load, pm_close
    from promana_py import (pa_app_connect, pa_app_disconnect,
                            pa_capture_start, pa_capture_stop,
                            pa_espi_read, pa_espi_operating_config,
                            pa_espi_hw_filter_config,
                            PromiraEspiOperatingCfg, PromiraEspiPacketMatch,
                            PA_PROTOCOL_ESPI, PA_TRIGGER_MODE_IMMEDIATE,
                            PA_APP_READ_EMPTY, PA_EVENT_ESPI_PACKET,
                            PA_ESPI_ALERT_PIN, PA_ESPI_ALIGN_256_BYTES)

    channel_match_map = {}
    try:
        from promana_py import (PA_ESPI_CHANNEL_MATCH_PERIF,
                                PA_ESPI_CHANNEL_MATCH_VW,
                                PA_ESPI_CHANNEL_MATCH_OOB,
                                PA_ESPI_CHANNEL_MATCH_FLASH)
        channel_match_map = {
            'peripheral': PA_ESPI_CHANNEL_MATCH_PERIF,
            'vw': PA_ESPI_CHANNEL_MATCH_VW,
            'oob': PA_ESPI_CHANNEL_MATCH_OOB,
            'flash': PA_ESPI_CHANNEL_MATCH_FLASH,
        }
    except ImportError:
        pass

    pm = pm_open(args.ip)
    if pm <= 0:
        print(f'Cannot open Promira on {args.ip} (error {pm})')
        sys.exit(1)
    pm_load(pm, "com.totalphase.promana_espi")
    conn = pa_app_connect(args.ip)
    if conn <= 0:
        print(f'Cannot connect analyzer app (error {conn})')
        pm_close(pm)
        sys.exit(1)

    # --- Operating config ---
    cfg = PromiraEspiOperatingCfg()
    cfg.io_mode = io_mode
    cfg.alert_pin = PA_ESPI_ALERT_PIN if io_mode > 0 else 0
    cfg.vw_max_count = 6
    cfg.oob_max_payload = PA_ESPI_ALIGN_256_BYTES
    pa_espi_operating_config(conn, 0, cfg)

    # --- Channel filter ---
    if args.filter and channel_match_map:
        match = 0
        for name in args.filter.split(','):
            name = name.strip().lower()
            if name in channel_match_map:
                match |= channel_match_map[name]
            else:
                print(f'Unknown filter: {name} (valid: {", ".join(channel_match_map)})')
                sys.exit(1)
        pkt_match = PromiraEspiPacketMatch()
        pkt_match.channel_match = match
        pa_espi_hw_filter_config(conn, 0, pkt_match)

    # --- CSV ---
    csv_file = None
    if args.csv:
        csv_file = open(args.csv, 'w')
        csv_file.write(csv_header() + '\n')

    # --- Capture ---
    signal.signal(signal.SIGINT, _sigint)
    pa_capture_start(conn, PA_PROTOCOL_ESPI, PA_TRIGGER_MODE_IMMEDIATE)

    io_label = args.io_mode.capitalize()
    print(f'eSPI Capture — {io_label} IO — {args.ip}')
    print(f'Press Ctrl+C to stop.\n')

    index = 0
    t0 = None

    while _running:
        ret, info, pkt_info, data = pa_espi_read(conn, 8192)

        if ret == PA_APP_READ_EMPTY:
            time.sleep(0.001)
            continue
        if ret < 0:
            break

        if t0 is None:
            t0 = info.timestamp

        is_packet = bool(info.events & PA_EVENT_ESPI_PACKET) and pkt_info.cmd_length > 0

        if is_packet:
            index += 1
            line = format_packet(index, info, pkt_info, list(data),
                                 args.raw, args.timestamp, t0)
            print(line)
            if csv_file:
                csv_file.write(csv_line(index, info, pkt_info, list(data)) + '\n')
                csv_file.flush()
        elif not args.packets_only:
            ev = format_event(info, info.events, args.show_alerts)
            if ev:
                print(ev)

    # --- Cleanup ---
    pa_capture_stop(conn)
    pa_app_disconnect(conn)
    pm_close(pm)
    if csv_file:
        csv_file.close()

    print(f'\nCapture stopped. {index} packets captured.')


if __name__ == '__main__':
    main()
