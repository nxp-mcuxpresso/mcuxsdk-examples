#!/usr/bin/env python3
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

"""
eSPI Example Auto Test

Three test categories:
  A-series: eSPI bus tests via Analyzer (requires --analyzer)
  S-series: Serial scenario tests (eSPI host + serial verification)
  C-series: Console command tests (pure serial)

Usage:
    python espi_autotest.py --host <HOST_IP> --serial <COM_PORT> --analyzer <ANALYZER_IP>
    python espi_autotest.py --host <HOST_IP> --serial <COM_PORT>
    python espi_autotest.py ... --io-mode dual
    python espi_autotest.py ... --io-mode quad
    python espi_autotest.py ... --tests A02,S01,C06
"""

import sys, os, time, argparse

# ---------------------------------------------------------------------------
# Paths & config
# ---------------------------------------------------------------------------
_SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, _SCRIPT_DIR)
if hasattr(os, 'add_dll_directory'):
    os.add_dll_directory(_SCRIPT_DIR)
SIM_MODE = '1'

# eSPI opcodes
PUT_PC = 0x00; PUT_NP = 0x02; GET_STATUS = 0x25
IORD_S1 = 0x40; IOWR_S1 = 0x44
PUT_VW = 0x04; GET_VW = 0x05; PUT_OOB = 0x06; GET_OOB = 0x07
RESP_ACCEPT_MASK = 0x08  # eSPI response byte bit 3 = ACCEPT
GET_FLASH_C = 0x0B
CH_PERIF = 0; CH_VW = 1; CH_OOB = 2; CH_FLASH = 3; CH_INDEP = 4
CRC_ERR_CMD = 0x100; CRC_ERR_RSP = 0x200; CRC_ERR_ANY = 0x300

# ---------------------------------------------------------------------------
# MCU Serial
# ---------------------------------------------------------------------------
class MCUSerial:
    def __init__(self, port, baud=115200):
        import serial as _serial
        self.ser = _serial.Serial(port, baud, timeout=0.5)

    def flush(self):
        self.ser.reset_input_buffer()

    def clear(self):
        self.ser.reset_input_buffer()
        self.ser.write(b'\r\n\r\n')
        time.sleep(0.3)
        self.ser.read(self.ser.in_waiting or 1)

    def send_raw(self, cmd, wait=0.8):
        self.ser.reset_input_buffer()
        self.ser.write(f'{cmd}\r\n'.encode())
        time.sleep(wait)
        self.ser.read(self.ser.in_waiting or 1)

    def read(self, wait=3.0):
        time.sleep(wait)
        return self.ser.read(self.ser.in_waiting or 1).decode('utf-8', errors='replace').strip()

    def send(self, cmd, wait=1.0):
        self.ser.reset_input_buffer()
        self.ser.write((cmd + '\r\n').encode())
        time.sleep(wait)
        return self.ser.read(self.ser.in_waiting or 1).decode('utf-8', errors='replace').strip()

    def close(self):
        self.ser.close()

# ---------------------------------------------------------------------------
# Host
# ---------------------------------------------------------------------------
class Host:
    def __init__(self, host_ip):
        import espi_host as _espi_host
        self._cmds = {name[5:]: obj for name, obj in vars(_espi_host).items()
                      if name.startswith('espi_') and callable(obj)}
        self.sim = _espi_host.NxpEspiSimulator(host_ip)
        self.sim.espi_config_mode(int(SIM_MODE))

    def send(self, command):
        if command == 'RESET':
            self.sim.espi_config_bus(0, 0)
            self.sim.espi_inband_reset()
            return 'OK'
        parts = command.split()
        name, args = parts[0], parts[1:]
        fn = self._cmds.get(name)
        if not fn:
            return f'ERR unknown: {name}'
        try:
            fn(self.sim, *args)
            return 'OK'
        except Exception as e:
            return f'ERR {e}'

    def run(self, commands):
        for cmd in commands:
            r = self.send(cmd)
            if r.startswith('ERR'):
                return r
        return 'OK'

    def close(self):
        self.sim.close()

# ---------------------------------------------------------------------------
# Analyzer
# ---------------------------------------------------------------------------
class Analyzer:
    def __init__(self, ip, io_mode=0, host_init_cmds=None):
        self.host_init_cmds = host_init_cmds or []
        self.io_mode = io_mode
        from promira_py import pm_open, pm_load, pm_close
        from promana_py import (pa_app_connect, pa_app_disconnect,
                                pa_capture_start, pa_capture_stop, pa_capture_status,
                                pa_espi_read,
                                pa_espi_operating_config, PromiraEspiOperatingCfg,
                                PA_PROTOCOL_ESPI, PA_TRIGGER_MODE_IMMEDIATE,
                                PA_APP_READ_EMPTY, PA_EVENT_ESPI_PACKET,
                                PA_ESPI_ALERT_PIN, PA_ESPI_ALIGN_256_BYTES)
        self._pa_capture_start = pa_capture_start
        self._pa_capture_stop = pa_capture_stop
        self._pa_capture_status = pa_capture_status
        self._pa_espi_read = pa_espi_read
        self._pa_app_disconnect = pa_app_disconnect
        self._pm_close = pm_close
        self._PA_PROTOCOL_ESPI = PA_PROTOCOL_ESPI
        self._PA_TRIGGER_MODE_IMMEDIATE = PA_TRIGGER_MODE_IMMEDIATE
        self._PA_APP_READ_EMPTY = PA_APP_READ_EMPTY
        self._PA_EVENT_ESPI_PACKET = PA_EVENT_ESPI_PACKET
        self.ip = ip
        self.pm = pm_open(ip)
        pm_load(self.pm, "com.totalphase.promana_espi")
        self.conn = pa_app_connect(ip)
        cfg = PromiraEspiOperatingCfg()
        cfg.io_mode = io_mode
        cfg.alert_pin = PA_ESPI_ALERT_PIN if io_mode > 0 else 0
        cfg.vw_max_count = 6
        cfg.oob_max_payload = PA_ESPI_ALIGN_256_BYTES
        pa_espi_operating_config(self.conn, 0, cfg)
        self.start()
        self.flush()
        self.stop()

    def _wait_ready(self):
        for _ in range(50):
            _, status, _ = self._pa_capture_status(self.conn)
            if status >= 1:
                return
            time.sleep(0.02)

    def start(self):
        self._pa_capture_start(self.conn, self._PA_PROTOCOL_ESPI, self._PA_TRIGGER_MODE_IMMEDIATE)
        self._wait_ready()

    def stop(self):
        self._pa_capture_stop(self.conn)

    def drain(self, host):
        self.start()
        host.send('RESET')
        for c in self.host_init_cmds:
            host.send(c)
        time.sleep(0.3)
        self.flush()
        self.stop()

    def capture(self, host, cmds, wait=3.0):
        self.drain(host)
        self.start()
        host.run(cmds)
        time.sleep(wait)
        pkts = self.read_packets()
        self.stop()
        return pkts

    def read_packets(self):
        pkts = []
        for _ in range(5000):
            ret, info, pkt_info, data = self._pa_espi_read(self.conn, 8192)
            if ret == self._PA_APP_READ_EMPTY: break
            if ret < 0: break
            if info.events & self._PA_EVENT_ESPI_PACKET and pkt_info.cmd_length > 0:
                pkts.append((info, pkt_info, list(data)))
        return pkts

    def flush(self):
        while self._pa_espi_read(self.conn, 8192)[0] != self._PA_APP_READ_EMPTY:
            pass

    def close(self):
        self._pa_app_disconnect(self.conn)
        self._pm_close(self.pm)

# ---------------------------------------------------------------------------
# Packet helpers
# ---------------------------------------------------------------------------
def extract_cmd_payload(pkt, data, opcode):
    cmd_len = pkt.cmd_length
    if cmd_len < 2 or data[0] != opcode:
        return None
    if opcode == IOWR_S1:
        return data[3:cmd_len-1]
    if opcode == PUT_PC:
        return data[8:cmd_len-1]
    if opcode == PUT_OOB:
        return data[4:cmd_len-1]
    if opcode == PUT_VW:
        return data[2:cmd_len-1]
    return None

def find_pkt(pkts, opcode=None, channel=None):
    for info, pkt, data in pkts:
        if opcode is not None and data[0] != opcode: continue
        if channel is not None and pkt.channel != channel: continue
        return (info, pkt, data)
    return None

def find_pkt_by_payload(pkts, opcode, channel, expected_payload):
    for info, pkt, data in pkts:
        if data[0] != opcode: continue
        if channel is not None and pkt.channel != channel: continue
        actual = extract_cmd_payload(pkt, data, opcode)
        if actual == expected_payload:
            return (info, pkt, data)
    return None

def check_crc(pkts, opcode=None, channel=None):
    for info, pkt, data in pkts:
        if opcode is not None and data[0] != opcode:
            continue
        if channel is not None and pkt.channel != channel:
            continue
        if pkt.cmd_length > 0 and (info.status & CRC_ERR_ANY):
            return False
    return True

def find_accept(rsp):
    """Skip WAIT_STATE bytes and return index of ACCEPT byte, or -1."""
    pos = 0
    while pos < len(rsp) and (rsp[pos] & 0x0F) == 0x0F and rsp[pos] != 0x08:
        pos += 1
    if pos < len(rsp) and (rsp[pos] & RESP_ACCEPT_MASK):
        return pos
    return -1

def parse_completion_data(rsp):
    """Parse tag+length+data from a completion response (after ACCEPT byte).
    Returns the data payload list, or None on failure."""
    tag_len = rsp[0]
    if len(rsp) < 2:
        return None
    length = ((tag_len & 0x0F) << 8) | rsp[1]
    if 2 + length > len(rsp):
        length = len(rsp) - 2
    return rsp[2:2 + length]

def extract_flash_read_data(pkt, data):
    """Extract flash read data from GET_FLASH_C response, handling split completions."""
    cl = pkt.cmd_length
    rsp = data[cl:]
    pos = find_accept(rsp)
    if pos < 0:
        return None
    pos += 1  # skip ACCEPT
    result = []
    first = True
    while pos + 2 < len(rsp):
        if first:
            pos += 1  # skip cycle byte
            first = False
        tag_len = rsp[pos]; pos += 1
        length = ((tag_len & 0x0F) << 8) | rsp[pos]; pos += 1
        if pos + length > len(rsp):
            length = len(rsp) - pos
        result.extend(rsp[pos:pos + length])
        pos += length
        if pos + 4 < len(rsp):
            pos += 2  # skip status bytes between completions
        else:
            break
    return result

# ---------------------------------------------------------------------------
# Result tracking
# ---------------------------------------------------------------------------
_results = []
_test_filter = None

def should_run(tid):
    return _test_filter is None or tid.upper() in _test_filter

def record(tid, name, passed, detail=''):
    _results.append((tid, name, passed, detail))
    s = '\033[32mPASS\033[0m' if passed else '\033[31mFAIL\033[0m'
    print(f'  {tid:5s} {name:38s} [{s}] {detail}')

# ---------------------------------------------------------------------------
# A-series: eSPI Bus Tests (Analyzer only)
# ---------------------------------------------------------------------------
def run_bus_tests(host, mcu, analyzer, io_mode='single', freq='20'):

    def az_test(tid, name, cmds, wait=3.0,
                opcode=None, channel=None, expected_payload=None):
        if not should_run(tid):
            return
        pkts = analyzer.capture(host, cmds, wait)
        if not check_crc(pkts, opcode, channel):
            record(tid, name, False, 'CRC error')
            return
        p = find_pkt_by_payload(pkts, opcode, channel, expected_payload)
        if p:
            cl = p[1].cmd_length
            rsp = p[2][cl:]
            if rsp and find_accept(rsp) < 0:
                record(tid, name, False, f'payload OK but no ACCEPT (rsp=0x{rsp[0]:02X})')
                return
            record(tid, name, True)
        else:
            found = find_pkt(pkts, opcode, channel)
            if found:
                actual = extract_cmd_payload(found[1], found[2], opcode)
                hex_str = ' '.join('%02X' % b for b in actual) if actual else 'None'
                record(tid, name, False, f'payload mismatch: {hex_str}')
            else:
                record(tid, name, False, f'no packet in {len(pkts)} captured')

    # --- A00: Channel Config Verify (GET_CONFIG after setup_channels + set_bus_config) ---
    GET_CONFIG = 0x21
    SETUP_CONFIGS = {
        0x10: [0x05, 0x73, 0x00, 0x00],
        0x20: [0x01, 0x00, 0x07, 0x00],
        0x30: [0x01, 0x02, 0x00, 0x00],
        0x40: [0x05, 0x2A, 0x00, 0x00],
    }
    io_mode_map = {'single': 0, 'dual': 1, 'quad': 2}
    freq_map = {'20': 0, '25': 1, '33': 2, '50': 3, '66': 4}
    if io_mode != 'single' or freq != '20':
        SETUP_CONFIGS[0x08] = []  # bus config: verify freq + io_mode fields
    if should_run('A00'):
        get_cmds = [f'raw_get_config 0x{off:02X}' for off in SETUP_CONFIGS]
        pkts = analyzer.capture(host, get_cmds)
        ok = True
        detail = ''
        for offset, expected in SETUP_CONFIGS.items():
            p = None
            for info, pkt, data in pkts:
                if data[0] == GET_CONFIG and pkt.channel == CH_INDEP:
                    cl = pkt.cmd_length
                    if cl >= 4 and (data[1] << 8 | data[2]) == offset:
                        p = (info, pkt, data)
                        break
            if not p:
                ok, detail = False, f'no GET_CONFIG(0x{offset:02X}) response'
                break
            if p[0].status & CRC_ERR_ANY:
                ok, detail = False, f'GET_CONFIG(0x{offset:02X}) CRC error'
                break
            rsp = p[2][p[1].cmd_length:]
            acc = find_accept(rsp)
            if acc < 0:
                ok, detail = False, f'GET_CONFIG(0x{offset:02X}) no ACCEPT'
                break
            cfg = rsp[acc + 1:acc + 5]
            if offset == 0x08:
                got_freq = (cfg[2] >> 4) & 0x07
                got_io = (cfg[3] >> 2) & 0x03
                exp_freq = freq_map[freq]
                exp_io = io_mode_map[io_mode]
                if got_freq != exp_freq:
                    ok, detail = False, f'GET_CONFIG(0x08) freq mismatch: got {got_freq} expected {exp_freq}'
                    break
                if got_io != exp_io:
                    ok, detail = False, f'GET_CONFIG(0x08) io_mode mismatch: got {got_io} expected {exp_io}'
                    break
            else:
                if cfg[0] & 0x01 != expected[0] & 0x01:
                    ok, detail = False, f'GET_CONFIG(0x{offset:02X}) ch_enable mismatch: got 0x{cfg[0]:02X}'
                    break
        record('A00', 'Channel Config', ok, detail)

    # --- A01: Status Get ---
    if should_run('A01'):
        pkts = analyzer.capture(host, ['mcux_sts_get'])
        p = find_pkt(pkts, GET_STATUS, CH_INDEP)
        if not p:
            record('A01', 'Status Get', False, f'no GET_STATUS in {len(pkts)} captured')
        elif p[0].status & CRC_ERR_ANY:
            record('A01', 'Status Get', False,
                   f'CRC error (cmd={"ERR" if p[0].status & CRC_ERR_CMD else "ok"}'
                   f' rsp={"ERR" if p[0].status & CRC_ERR_RSP else "ok"})')
        elif p[1].cmd_length != 2:
            record('A01', 'Status Get', False, f'cmd={p[1].cmd_length}B, expected 2B')
        elif p[1].length - p[1].cmd_length < 4:
            record('A01', 'Status Get', False, f'rsp={p[1].length - p[1].cmd_length}B, expected 4B')
        else:
            cl = p[1].cmd_length
            rsp_code = p[2][cl]
            status = p[2][cl+1] | (p[2][cl+2] << 8)
            accept = bool(rsp_code & RESP_ACCEPT_MASK)
            pc_free = bool(status & 0x01)
            np_free = bool(status & 0x02)
            ok = accept and pc_free and np_free
            record('A01', 'Status Get', ok,
                   '' if ok else f'rsp=0x{rsp_code:02X} status=0x{status:04X}')

    # --- A02: VWire Put (3 batches of ≤3 groups for Analyzer decode reliability) ---
    if should_run('A02'):
        batches = [
            ('mcux_vw_put_p1', [0x02,0x77,0x03,0x77,0x07,0x11]),
            ('mcux_vw_put_p2', [0x41,0x77,0x42,0x33,0x43,0x55]),
            ('mcux_vw_put_p3', [0x44,0x55,0x47,0x11]),
        ]
        ok = True
        detail = ''
        for cmd, expected in batches:
            pkts = analyzer.capture(host, [cmd])
            if not check_crc(pkts, PUT_VW, CH_VW):
                ok, detail = False, f'{cmd}: CRC error'
                break
            p = find_pkt_by_payload(pkts, PUT_VW, CH_VW, expected)
            if not p:
                found = find_pkt(pkts, PUT_VW, CH_VW)
                if found:
                    actual = extract_cmd_payload(found[1], found[2], PUT_VW)
                    detail = f'{cmd}: payload mismatch: {" ".join("%02X" % b for b in actual) if actual else "None"}'
                else:
                    detail = f'{cmd}: no PUT_VW in {len(pkts)} captured'
                ok = False
                break
            cl = p[1].cmd_length
            if p[1].length > cl and not (p[2][cl] & RESP_ACCEPT_MASK):
                ok, detail = False, f'{cmd}: no ACCEPT (rsp=0x{p[2][cl]:02X})'
                break
        record('A02', 'VWire Put', ok, detail)
        host.send('reset')
        time.sleep(1)

    # --- A03: VWire Get (isolated per-flag: set 0->1, verify, clear 1->0) ---
    # (WIREWO bit, VWire index, expected set data)
    VW_FLAGS = [
        ('oobrstack',    0, 0x04, 0x11),
        ('wakenscin',    1, 0x04, 0x44),
        ('pmen',         2, 0x04, 0x88),
        ('scin',         3, 0x06, 0x11),
        ('smin',         4, 0x06, 0x22),
        ('rcinn',        5, 0x06, 0x44),
        ('hostrstack',   6, 0x06, 0x88),
        ('susackn',      7, 0x40, 0x11),
        ('bootdone',    16, 0x05, 0x11),
        ('booterrn',    17, 0x05, 0x88),
    ]
    # dswpwrokrst (bit 18) excluded: does not trigger VWire packet via send_vw_mask
    if should_run('A03'):
        analyzer.drain(host)
        host.send('raw_vw_config')
        host.send('raw_vw_get')
        time.sleep(0.3)

        def vw_clear():
            mcu.send_raw('send_vw_mask 0x0', wait=0.5)
            host.send('mcux_sts_get')
            time.sleep(0.3)
            host.send('raw_vw_get')
            time.sleep(0.3)

        def vw_capture(mask_hex):
            analyzer.start()
            mcu.send_raw(f'send_vw_mask {mask_hex}', wait=0.8)
            host.send('mcux_sts_get')
            time.sleep(0.3)
            host.send('raw_vw_get')
            time.sleep(0.5)
            pkts = analyzer.read_packets()
            analyzer.stop()
            return find_pkt(pkts, GET_VW, CH_VW)

        def vw_rsp_has(pkt_tuple, expected_pairs):
            if not pkt_tuple:
                return False
            if pkt_tuple[0].status & CRC_ERR_ANY:
                return False
            rsp = pkt_tuple[2][pkt_tuple[1].cmd_length:]
            acc = find_accept(rsp)
            if acc < 0 or acc + 2 >= len(rsp):
                return False
            return all(
                any(rsp[i] == idx and rsp[i+1] == data for i in range(acc + 2, len(rsp) - 1, 2))
                for idx, data in expected_pairs)

        vw_clear()
        failed = []
        for flag_name, wo_bit, exp_idx, exp_set in VW_FLAGS:
            p = vw_capture(f'0x{1 << wo_bit:X}')
            if not vw_rsp_has(p, [(exp_idx, exp_set)]):
                failed.append(flag_name)
            vw_clear()

        p = vw_capture(f'0x{0x55 << 8:X}')
        if not vw_rsp_has(p, [(0x45, 0x55), (0x46, 0x55)]):
            failed.append('e2p')
        vw_clear()

        total = len(VW_FLAGS) + 1
        ok = len(failed) == 0
        record('A03', 'VWire Get', ok,
               '' if ok else f'{total - len(failed)}/{total} flags, failed: {",".join(failed)}')

    # --- A04: VWire IRQ Push (serial push_irq, verify IRQ value in VWire) ---
    if should_run('A04'):
        irq_num = 10
        exp_idx = 0x00 if irq_num < 128 else 0x01
        exp_data = 0x80 | (irq_num & 0x7F)
        analyzer.drain(host)
        host.send('raw_vw_config')
        time.sleep(0.3)
        analyzer.start()
        mcu.clear()
        mcu.send_raw(f'push_irq {irq_num}', wait=1.0)
        host.send('mcux_sts_get')
        time.sleep(0.3)
        host.send('raw_vw_get')
        time.sleep(0.5)
        pkts = analyzer.read_packets()
        analyzer.stop()
        p = find_pkt(pkts, GET_VW, CH_VW)
        ok = False
        detail = ''
        if not p:
            detail = f'no GET_VW packet (pkts={len(pkts)})'
        elif p[0].status & CRC_ERR_ANY:
            detail = 'CRC error'
        elif p[1].length <= p[1].cmd_length + 2:
            detail = 'GET_VW response too short'
        else:
            rsp = p[2][p[1].cmd_length:]
            acc = find_accept(rsp)
            if acc < 0:
                detail = f'no ACCEPT (rsp=0x{rsp[0]:02X})'
            else:
                for i in range(acc + 2, len(rsp) - 1, 2):
                    if rsp[i] == exp_idx and rsp[i+1] == exp_data:
                        ok = True
                        break
                if not ok:
                    detail = f'IRQ {irq_num} not found (expected idx=0x{exp_idx:02X} data=0x{exp_data:02X})'
        record('A04', 'VWire IRQ Push', ok, detail)

    # --- A05: GPIO VWire (In: host->WIREIN_GPIO decode; Out: MCU WIREOUT_GPIO->host GET_VW) ---
    if should_run('A05'):
        host.send('reset')          # clear residual VW state from prior tests
        time.sleep(1)
        ok, detail = True, ''

        # In: host drives a GPIO VW; MCU decodes it in the WireChange ISR
        idx, valid, level = 0x80, 0xF, 0x5
        data = (valid << 4) | level
        analyzer.drain(host)
        analyzer.start()
        mcu.clear()
        host.send(f'mcux_gpio_vw_put 0x{idx:02X} 0x{data:02X}')
        time.sleep(0.4)
        pkts = analyzer.read_packets()
        analyzer.stop()
        sl = mcu.read(wait=1.0)
        if not check_crc(pkts, PUT_VW, CH_VW):
            ok, detail = False, 'in: CRC error'
        elif not find_pkt_by_payload(pkts, PUT_VW, CH_VW, [idx, data]):
            got = find_pkt(pkts, PUT_VW, CH_VW)
            ok, detail = False, f'in: PUT_VW mismatch: {extract_cmd_payload(got[1], got[2], PUT_VW) if got else None}'
        elif f'eSPI VWire GPIO message: index={idx} valid=0x{valid:X} level=0x{level:X}' not in sl:
            ok, detail = False, f'in: ISR decode not seen: {sl[:120]}'

        # Out: MCU drives a GPIO VW; host reads it back via GET_VW
        if ok:
            idx, valid, level = 0x81, 0xF, 0xA
            data = (valid << 4) | level
            analyzer.drain(host)
            host.send('raw_vw_config')
            time.sleep(0.3)
            analyzer.start()
            mcu.clear()
            mcu.send_raw(f'send_vw_gpio {idx} {valid} {level}', wait=0.8)
            host.send('mcux_sts_get')
            time.sleep(0.3)
            host.send('raw_vw_get')
            time.sleep(0.5)
            pkts = analyzer.read_packets()
            analyzer.stop()
            p = find_pkt(pkts, GET_VW, CH_VW)
            if not p:
                ok, detail = False, f'out: no GET_VW (pkts={len(pkts)})'
            elif p[0].status & CRC_ERR_ANY:
                ok, detail = False, 'out: CRC error'
            else:
                rsp = p[2][p[1].cmd_length:]
                acc = find_accept(rsp)
                if acc < 0:
                    ok, detail = False, f'out: no ACCEPT (rsp=0x{rsp[0] if rsp else 0:02X})'
                elif not any(rsp[i] == idx and rsp[i + 1] == data for i in range(acc + 2, len(rsp) - 1, 2)):
                    ok, detail = False, f'out: (0x{idx:02X},0x{data:02X}) not in GET_VW rsp'
        record('A05', 'GPIO VWire', ok, detail)

    # --- A06: OOB Put ---
    az_test('A06', 'OOB Put', ['mcux_oob_put'],
            opcode=PUT_OOB, channel=CH_OOB,
            expected_payload=[(0x50<<1)|0, 0xA0, 128] + list(range(128)))

    # --- A07: OOB Get (serial triggers OOB, verify payload) ---
    if should_run('A07'):
        oob_bytes = [0xAA, 0xBB, 0xCC, 0xDD]
        oob_hex = ' '.join(f'{b:02X}' for b in oob_bytes)
        analyzer.drain(host)
        analyzer.start()
        host.run(['raw_oob_config', 'raw_oob_get'])
        time.sleep(0.3)
        analyzer.flush()
        analyzer.stop()
        # Capture: MCU sends OOB + single GET_OOB (no GET_STATUS)
        analyzer.start()
        mcu.clear()
        mcu.send_raw(f'send_oob {oob_hex}', wait=1.0)
        host.send('raw_oob_get')
        time.sleep(1.0)
        pkts = analyzer.read_packets()
        analyzer.stop()
        p = find_pkt(pkts, GET_OOB, CH_OOB)
        if not p:
            record('A07', 'OOB Get', False, f'no GET_OOB in {len(pkts)} pkts')
        elif p[0].status & CRC_ERR_ANY:
            record('A07', 'OOB Get', False, 'CRC error')
        else:
            cl = p[1].cmd_length
            rsp = p[2][cl:]
            ok = False
            detail = ''
            pos = find_accept(rsp)
            if pos < 0:
                detail = f'no ACCEPT (rsp=0x{rsp[0] if rsp else 0:02X})'
            elif pos + 5 >= len(rsp):
                detail = f'rsp too short ({len(rsp) - pos}B after ACCEPT)'
            else:
                cycle = rsp[pos + 1]
                length = ((rsp[pos + 2] & 0x0F) << 8) | rsp[pos + 3]
                payload = rsp[pos + 4:pos + 4 + length]
                if cycle != 0x21:
                    detail = f'wrong cycle type 0x{cycle:02X}, expected 0x21'
                elif len(payload) < len(oob_bytes):
                    detail = f'payload too short ({len(payload)}B, expected >={len(oob_bytes)}B)'
                elif payload[:len(oob_bytes)] != oob_bytes:
                    detail = f'payload mismatch: [{" ".join("%02X" % b for b in payload[:8])}]'
                else:
                    ok = True
            record('A07', 'OOB Get', ok, detail)

    # --- A08: SAF Verify (Erase->Read(FF)->Write->Read(data)->Erase->Read(FF)) ---
    if should_run('A08'):
        detail = ''
        ok = True
        write_data = list(range(128))

        def verify_read(label, expected):
            nonlocal ok, detail
            if not ok: return
            pkts = analyzer.capture(host, ['mcux_saf_read'], wait=5)
            if not check_crc(pkts, GET_FLASH_C, CH_FLASH):
                ok, detail = False, f'{label}: CRC error'
                return
            p = find_pkt(pkts, GET_FLASH_C, CH_FLASH)
            if not p:
                ok, detail = False, f'{label}: no completion'
                return
            rsp = p[2][p[1].cmd_length:]
            if find_accept(rsp) < 0:
                ok, detail = False, f'{label}: no ACCEPT (rsp=0x{rsp[0] if rsp else 0:02X})'
                return
            rd = extract_flash_read_data(p[1], p[2])
            if rd is None:
                ok, detail = False, f'{label}: parse failed'
                return
            if rd[:len(expected)] != expected:
                ok, detail = False, f'{label}: data mismatch (first 4: {[hex(b) for b in rd[:4]]})'

        analyzer.capture(host, ['mcux_saf_erase'], wait=5)
        verify_read('erase->read', [0xFF] * 128)
        if ok:
            pkts = analyzer.capture(host, ['mcux_saf_write'], wait=5)
            if not check_crc(pkts, 0x0A, CH_FLASH):
                ok, detail = False, 'write: CRC error'
            elif not find_pkt(pkts, 0x0A, CH_FLASH):
                ok, detail = False, 'write: no PUT_FLASH_NP'
        verify_read('write->read', write_data)
        if ok:
            analyzer.capture(host, ['mcux_saf_erase'], wait=5)
        verify_read('re-erase->read', [0xFF] * 128)

        record('A08', 'SAF Verify', ok, detail)

    # --- A09: Endpoint Port 80 ---
    az_test('A09', 'Endpoint Port 80', ['mcux_ep_write_p80'],
            opcode=IOWR_S1, channel=CH_PERIF, expected_payload=[0x88])

    # --- A10: Endpoint IO (Write Cmd + Write Data + Read back) ---
    if should_run('A10'):
        ok = True
        detail = ''
        pkts = analyzer.capture(host, ['mcux_ep_write_cmd', 'mcux_ep_write_data'])
        if not check_crc(pkts, IOWR_S1, CH_PERIF):
            ok, detail = False, 'write CRC error'
        elif not find_pkt_by_payload(pkts, IOWR_S1, CH_PERIF, [0x57]):
            ok, detail = False, 'write cmd [0x57] not found'
        elif not find_pkt_by_payload(pkts, IOWR_S1, CH_PERIF, [0x56]):
            ok, detail = False, 'write data [0x56] not found'
        if ok:
            pkts = analyzer.capture(host, ['mcux_ep_read_data'])
            p = find_pkt(pkts, IORD_S1, CH_PERIF)
            if not p:
                ok, detail = False, 'read packet not found'
            elif p[0].status & CRC_ERR_ANY:
                ok, detail = False, 'read CRC error'
            else:
                cl = p[1].cmd_length
                rsp_code = p[2][cl]
                if not (rsp_code & RESP_ACCEPT_MASK):
                    ok, detail = False, f'no ACCEPT (rsp=0x{rsp_code:02X})'
                elif p[1].length < cl + 2:
                    ok, detail = False, 'rsp too short for data byte'
                else:
                    rd = p[2][cl + 1]
                    if rd != 0x56:
                        ok, detail = False, f'read data=0x{rd:02X}, expected 0x56'
        record('A10', 'Endpoint IO', ok, detail)

    # --- A11: Index-Data Write ---
    if should_run('A11'):
        pkts = analyzer.capture(host, ['mcux_idx_write_data', 'mcux_idx_write_cmd'])
        if not check_crc(pkts, IOWR_S1, CH_PERIF):
            record('A11', 'Index-Data Write', False, 'CRC error')
        else:
            all_payloads = []
            all_accept = True
            for _, pkt, data in pkts:
                if data[0] == IOWR_S1 and pkt.channel == CH_PERIF:
                    p = extract_cmd_payload(pkt, data, IOWR_S1)
                    if p:
                        all_payloads.append(p)
                    cl = pkt.cmd_length
                    if pkt.length > cl and not (data[cl] & RESP_ACCEPT_MASK):
                        all_accept = False
            if not all_accept:
                record('A11', 'Index-Data Write', False, 'no ACCEPT')
            else:
                ok = [0x58] in all_payloads and [0x59] in all_payloads
                record('A11', 'Index-Data Write', ok,
                       '' if ok else f'payloads: {all_payloads}')

    # --- A12: Mailbox IO (Write×3 + Read + Write 256B) ---
    if should_run('A12'):
        detail = ''
        ok = True
        # Write 8B ×3 (WRSTAT flow control)
        for i in range(3):
            pkts = analyzer.capture(host, ['mcux_mb_write'], wait=4)
            if not check_crc(pkts, PUT_PC, CH_PERIF):
                ok, detail = False, f'write 8B #{i+1} CRC error'
                break
            p = find_pkt_by_payload(pkts, PUT_PC, CH_PERIF, [1,2,3,4,8,7,6,5])
            if not p:
                ok, detail = False, f'write 8B #{i+1} failed'
                break
            cl = p[1].cmd_length
            if p[1].length > cl and not (p[2][cl] & RESP_ACCEPT_MASK):
                ok, detail = False, f'write 8B #{i+1} no ACCEPT'
                break
        # Read → verify completion data matches written [1,2,3,4,8,7,6,5]
        if ok:
            pkts = analyzer.capture(host, ['mcux_mb_read'], wait=4)
            p = find_pkt(pkts, PUT_NP, CH_PERIF)
            if not p:
                ok, detail = False, 'read packet not found'
            elif p[0].status & CRC_ERR_ANY:
                ok, detail = False, 'read CRC error'
            else:
                rsp = p[2][p[1].cmd_length:]
                acc = find_accept(rsp)
                rd = None
                if acc < 0:
                    ok, detail = False, f'read: no ACCEPT (rsp=0x{rsp[0]:02X})' if rsp else 'read: empty rsp'
                else:
                    rd = parse_completion_data(rsp[acc + 2:])
                expected_mb = [1,2,3,4,8,7,6,5]
                if ok and (not rd or rd[:len(expected_mb)] != expected_mb):
                    ok, detail = False, f'read data mismatch: {[hex(b) for b in rd[:10]] if rd else None}'
        # Write 256B + Read back
        if ok:
            pkts = analyzer.capture(host, ['mcux_mb_wrd'], wait=6)
            if not check_crc(pkts, PUT_PC, CH_PERIF):
                ok, detail = False, 'write 256B CRC error'
            else:
                wr = find_pkt(pkts, PUT_PC, CH_PERIF)
                if not wr or extract_cmd_payload(wr[1], wr[2], PUT_PC) != list(range(256)):
                    ok, detail = False, 'write 256B payload mismatch'
                else:
                    wr_rsp = wr[2][wr[1].cmd_length:]
                    if wr_rsp and find_accept(wr_rsp) < 0:
                        ok, detail = False, 'write 256B no ACCEPT'
            if ok:
                rd_pkt = find_pkt(pkts, PUT_NP, CH_PERIF)
                if rd_pkt and rd_pkt[0].status & CRC_ERR_ANY:
                    ok, detail = False, 'read-back 256B CRC error'
                elif rd_pkt:
                    rsp = rd_pkt[2][rd_pkt[1].cmd_length:]
                    acc = find_accept(rsp)
                    if acc < 0:
                        ok, detail = False, 'read-back 256B no ACCEPT'
                    else:
                        rd = parse_completion_data(rsp[acc + 2:])
                        if rd != list(range(256)):
                            ok, detail = False, f'read-back 256B data mismatch (first 4: {[hex(b) for b in rd[:4]] if rd else None})'
        record('A12', 'Mailbox IO', ok, detail)


# ---------------------------------------------------------------------------
# S-series: Serial Scenario Tests
# ---------------------------------------------------------------------------
def run_serial_tests(host, mcu, io_mode='single', freq='20'):
    host.send('reset')
    time.sleep(1)

    # --- S00: Config Verify via serial (show_config after host setup) ---
    if should_run('S00'):
        host.run(['setup_channels'])
        if io_mode != 'single' or freq != '20':
            host.send(f'set_bus_config {io_mode} {freq}')
        time.sleep(0.5)
        mcu.flush()
        sl = mcu.send('show_config', wait=2)
        ok = True
        detail = ''
        io_expect = {'single': 'Single SPI', 'dual': 'Dual SPI', 'quad': 'Quad SPI'}
        freq_expect = {'20': '<=20 MHz', '25': '<=25 MHz', '33': '<=33 MHz',
                       '50': '<=50 MHz', '66': '<=66 MHz'}
        checks = [
            ('Ch0 (Memory): Enabled', 'Memory not enabled'),
            ('Ch1 (VWire):  Enabled', 'VWire not enabled'),
            ('Ch2 (OOB):    Enabled', 'OOB not enabled'),
            ('Ch3 (Flash):  Enabled', 'Flash not enabled'),
            (io_expect[io_mode], f'IO mode mismatch (expected {io_expect[io_mode]})'),
            (freq_expect[freq], f'Freq mismatch (expected {freq_expect[freq]})'),
        ]
        for expected_str, err_msg in checks:
            if expected_str not in sl:
                ok, detail = False, err_msg
                break
        record('S00', 'Config Verify', ok, detail)
        host.send('reset')
        time.sleep(1)

    # --- S01: VWire Put ---
    if should_run('S01'):
        expected_flags = ['SLP_S3N', 'SLP_S4N', 'SLP_S5N', 'SUS_STAT',
                          'PLTRST', 'OOB_RST_WARN', 'HOST_RST_WARN',
                          'SUS_WARN', 'SUS_PWRDN_ACK', 'SLP_AN',
                          'SLP_LAN', 'SLP_WLAN', 'HOST_C10N']
        mcu.flush()
        host.run(['mcux_vw_put'])
        sl = mcu.read(wait=2)
        missing = [f for f in expected_flags if f not in sl]
        ok = len(missing) == 0
        record('S01', 'VWire Put', ok,
               '' if ok else f'missing: {",".join(missing)}')
        # S01 sends system signals that alter MCU state
        host.send('reset')
        time.sleep(1)

    # --- S02: OOB Put (Host sends OOB, serial verify MCU received) ---
    if should_run('S02'):
        mcu.flush()
        host.run(['mcux_oob_put'])
        sl = mcu.read(wait=3)
        ok = True
        detail = ''
        oob_header = [0xA0, 0xA0, 0x80]
        oob_payload = list(range(128))
        expected_hex = ' '.join('%02X' % b for b in oob_header + oob_payload)
        if 'OOB received' not in sl:
            ok, detail = False, 'no OOB received'
        elif '131 bytes' not in sl:
            ok, detail = False, 'length mismatch (expected 131 bytes)'
        elif expected_hex not in sl:
            ok, detail = False, 'payload mismatch'
        record('S02', 'OOB Put', ok,
               '' if ok else f'{detail}: {sl[:120]}')

    # --- S03: SAF Verify ---
    if should_run('S03'):
        ok = True
        detail = ''
        mcu.flush()
        # Erase → Read (expect FF)
        host.run(['mcux_saf_erase', 'mcux_saf_read'])
        sl = mcu.read(wait=6)
        if 'FF FF FF FF FF FF FF FF' not in sl:
            ok, detail = False, 'erase: read not all 0xFF'
        # Write block 1 (0x500: 00..7F) + block 2 (0x580: 7F..00) → Read
        if ok:
            mcu.flush()
            host.run(['mcux_saf_write', 'mcux_saf_read'])
            sl = mcu.read(wait=6)
            if '00 01 02 03 04 05 06 07' not in sl:
                ok, detail = False, 'write block1: first bytes mismatch'
            elif '7F 7E 7D 7C' not in sl:
                ok, detail = False, 'write block2: first bytes mismatch'
        # Erase again → Read (expect FF again)
        if ok:
            mcu.flush()
            host.run(['mcux_saf_erase', 'mcux_saf_read'])
            sl = mcu.read(wait=6)
            if 'FF FF FF FF FF FF FF FF' not in sl:
                ok, detail = False, 're-erase: read not all 0xFF'
        record('S03', 'SAF Verify', ok, detail)

    # --- S04: Endpoint Port 80 ---
    if should_run('S04'):
        ok = True
        detail = ''
        mcu.flush()
        mcu.send('reset_p80')
        # Write 10 → count: 10
        for _ in range(10):
            host.run(['mcux_ep_write_p80'])
        sl = mcu.read(wait=3)
        if 'count: 10' not in sl:
            ok = False
            detail = 'after 10 writes, no count: 10'
        # Reset → write 1 → count: 1
        if ok:
            mcu.send('reset_p80')
            host.run(['mcux_ep_write_p80'])
            sl = mcu.read(wait=3)
            if 'count: 1' not in sl:
                ok = False
                detail = 'after reset+1 write, no count: 1'
        # Write 15 more → count: 0 (wrap)
        if ok:
            for _ in range(15):
                host.run(['mcux_ep_write_p80'])
            sl = mcu.read(wait=3)
            if 'count: 0' not in sl:
                ok = False
                detail = 'after 16 total writes, no count: 0 (wrap)'
        mcu.send('reset_p80')
        record('S04', 'Endpoint Port 80', ok, detail)

    # --- S05: Endpoint IO (Write Cmd + Write Data, serial verify) ---
    if should_run('S05'):
        ok = True
        detail = ''
        mcu.flush()
        host.run(['mcux_ep_write_cmd'])
        sl = mcu.read(wait=2)
        if 'Received endpoint message' not in sl:
            ok, detail = False, 'write cmd: no endpoint message'
        elif 'datain = 0x57' not in sl and 'datain = 0x57' not in sl.replace('0X', '0x'):
            ok, detail = False, f'write cmd: datain 0x57 not found: {sl[:80]}'
        if ok:
            mcu.flush()
            host.run(['mcux_ep_write_data'])
            sl = mcu.read(wait=2)
            if 'datain = 0x56' not in sl:
                ok, detail = False, 'write data: no datain 0x56'
            elif 'Endpoint data ready' not in sl:
                ok, detail = False, 'write data: no data ready'
        record('S05', 'Endpoint IO', ok, detail)

    # --- S06: Index-Data Write (serial verify) ---
    # Index-Data port: data register at 0x0200, index register at 0x0201
    # Only the index register write (0x0201) triggers kESPI_PortSpec0Interrupt → serial print
    if should_run('S06'):
        ok = True
        detail = ''
        mcu.flush()
        host.run(['mcux_idx_write_data', 'mcux_idx_write_cmd'])
        sl = mcu.read(wait=3)
        if 'Received Index-data message' not in sl:
            ok, detail = False, 'no Index-data message'
        elif '0x58' not in sl:
            ok, detail = False, f'data 0x58 not found: {sl[:80]}'
        elif '0x59' not in sl:
            ok, detail = False, f'cmd 0x59 not found: {sl[:80]}'
        record('S06', 'Index-Data Write', ok, detail)

    # --- S07: Mailbox Write (serial verify) ---
    if should_run('S07'):
        ok = True
        detail = ''
        mcu.flush()
        host.run(['mcux_mb_write'])
        sl = mcu.read(wait=3)
        if 'Mailbox received' not in sl:
            ok, detail = False, 'no Mailbox received'
        elif '01 02 03 04 08 07 06 05' not in sl:
            ok, detail = False, f'data mismatch: {sl[:100]}'
        record('S07', 'Mailbox Write', ok, detail)


# ---------------------------------------------------------------------------
# C-series: Console Commands
# ---------------------------------------------------------------------------
def run_console_tests(mcu):
    def con(tid, cmd, must):
        if not should_run(tid):
            return
        out = mcu.send(cmd)
        missing = [p for p in must if p not in out]
        record(tid, cmd, not missing,
               '' if not missing else f'missing: {missing[0][:30]}')

    con('C01', 'status', ['eSPI status:'])
    con('C02', 'help', ['Interactive commands'])
    con('C03', 'h', ['Interactive commands'])
    con('C04', 'vw_flags', ['pmen', 'bootdone'])
    con('C05', 'unknown_cmd', ['Unknown command'])
    con('C06', 'send_vw_mask 0x00040000', ['VW sends as mask'])
    con('C07', 'send_vw_flag pmen 1', ['ESPI_SendVWire', '-> 0'])
    con('C08', 'send_vw_flag bootdone 1', ['ESPI_SendVWire', '-> 0'])
    con('C09', 'send_vw_flag bad_name 1', ['Unknown VW flag'])
    con('C10', 'send_vw_flag', ['Usage'])
    con('C11', 'send_vw_flag pmen', ['Usage'])
    con('C12', 'send_oob AA BB CC', ['ESPI_SendOOB'])
    con('C13', 'send_oob A', ['Invalid hex'])
    con('C14', 'send_oob', ['No OOB data'])
    con('C15', 'push_irq 10', [])
    con('C16', 'push_irq 0', [])
    con('C17', 'push_irq 255', [])
    con('C18', 'reset_p80', ['counter reset'])


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
def main():
    parser = argparse.ArgumentParser(description='eSPI Auto Test')
    parser.add_argument('--host', required=True, help='Host Promira IP')
    parser.add_argument('--serial', required=True, help='MCU serial port')
    parser.add_argument('--analyzer', default=None, help='Analyzer Promira IP')
    parser.add_argument('--tests', default=None, help='Filter: A02,S01,C06,...')
    parser.add_argument('--io-mode', choices=['single', 'dual', 'quad'],
                        default='single', help='eSPI IO mode (default: single)')
    parser.add_argument('--freq', choices=['20', '25', '33', '50', '66'],
                        default='20', help='eSPI bus frequency in MHz (default: 20)')
    args = parser.parse_args()

    global _test_filter, _results
    _results = []
    _test_filter = None
    if args.tests:
        _test_filter = {t.strip().upper() for t in args.tests.split(',')}

    az_io_mode_map = {'single': 0, 'dual': 2, 'quad': 4}
    az_io_mode = az_io_mode_map[args.io_mode]

    host_init_cmds = ['setup_channels']
    if args.io_mode != 'single' or args.freq != '20':
        host_init_cmds.append(f'set_bus_config {args.io_mode} {args.freq}')

    analyzer = None
    if args.analyzer:
        try:
            analyzer = Analyzer(args.analyzer, io_mode=az_io_mode, host_init_cmds=host_init_cmds)
        except Exception as e:
            print(f'Analyzer connection failed: {e}')
            sys.exit(1)

    mcu = MCUSerial(args.serial)
    mcu.flush()

    host = Host(args.host)
    io_label = args.io_mode.capitalize() + ' IO'
    freq_label = args.freq + 'MHz'
    print('=' * 70)
    print(f'eSPI Auto Test -- {io_label} @ {freq_label}')
    print(f'Host: {args.host} | Analyzer: {args.analyzer or "N/A"} | Serial: {args.serial}')
    print('=' * 70)

    if analyzer:
        print('\n[Bus Tests]')
        run_bus_tests(host, mcu, analyzer, args.io_mode, args.freq)

    print('\n[Serial Tests]')
    run_serial_tests(host, mcu, args.io_mode, args.freq)

    print('\n[Console Commands Test]')
    run_console_tests(mcu)

    # Summary
    p = sum(1 for _, _, ok, _ in _results if ok)
    f = len(_results) - p
    print(f'\n{"=" * 70}')
    print(f'Results: {len(_results)} total, {p} passed, {f} failed')
    print(f'{"=" * 70}')

    host.close()
    mcu.close()
    if analyzer:
        analyzer.close()

    sys.exit(0 if f == 0 else 1)


if __name__ == '__main__':
    main()
