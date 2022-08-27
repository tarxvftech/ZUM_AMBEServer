#!/usr/bin/env python

import sys
import time
import struct
import serial

amberesetvector = b"\x61\x00\x01\x00\x33"

DIR_IN = 1
DIR_OUT = 2
DIR_BOTH = DIR_IN | DIR_OUT

class AMBE:
    PKT_MAGIC = 0x61
    PARITY_MAGIC = 0x2f
    CONTROL = 0x00

    
    TYPES = {
            CONTROL: {
                "PKT_CHANNEL0":      [0x40, 0, 0, DIR_BOTH, "Select first channel (only channel for AMBE3000 chips)"],
                "PKT_ECMODE":        [0x05, 2, 0, DIR_BOTH, "Encode flags (current channel)"],
                "PKT_DCMODE":        [0x06, 2, 0, DIR_BOTH, "Decoder flags"],
                "PKT_COMPAND":       [0x32, 1, 0, DIR_BOTH, "Companding and a-law/u-law"],
                "PKT_RATET":         [0x09, 1, 0, DIR_BOTH, "Select rate from table (current channel)"],
                "PKT_RATEP":         [0x0A, 12, 0, DIR_BOTH, "select custom rate (current channel)"],
                "PKT_INIT":          [0x0B, 1, 0, DIR_BOTH, "Initialize encoder/decoder (current channel)"],
                "PKT_LOWPOWER":      [0x10, 1, 0, DIR_BOTH, "enable or disable low power mode"],
                "PKT_CODECCFG":      [0x38, -1, 0, DIR_BOTH, ""],
                "PKT_CODECSTART":    [0x2A, 1, 0, DIR_BOTH, ""],
                "PKT_CODECSTOP":     [0x2B, 0, 0, DIR_BOTH, ""],
                "PKT_CHANFMT":       [0x15, 2, 0, DIR_BOTH, "Sets channel format (encoded bits)"],
                "PKT_SPCHFMT":       [0x16, 2, 0, DIR_BOTH, "sets speech format (decoded/not-encoded audio)"],
                "PKT_PRODID":        [0x30, 0,-1, DIR_BOTH, "Query for Product ID"],
                "PKT_VERSTRING":     [0x31, 0,48, DIR_BOTH, "Query for Version String"],
                "PKT_READY":         [0x39, 0, 0,  DIR_OUT, "Chip is ready to receive packets"],
                "PKT_HALT":          [0x35, 0, 0,   DIR_IN, "Ultra low power mode"],
                "PKT_RESET":         [0x33, 0, 0,   DIR_IN, "Reset to configuration specified with hardware pins"],
                "PKT_RESETSOFTCFG":  [0x34, 6,0,  DIR_IN, "Reset to software configuration"],
                "PKT_GETCFG":        [0x36, 0, 3, DIR_BOTH, ""],
                "PKT_READCFG":       [0x37, 0, 3, DIR_BOTH, ""],
                "PKT_PARITYMODE":    [0x3f, 1, 0, DIR_BOTH, ""],
                "PKT_WRITEI2C":      [0x44, -1, 0, DIR_BOTH, ""],
                "PKT_CLRCODECRESET": [0x46, 0,0, DIR_BOTH, ""],
                "PKT_SETCODECRESET": [0x47, 0,0, DIR_BOTH, ""],
                "PKT_DISCARDCODEC":  [0x48, 2,0,DIR_BOTH, ""],
                "PKT_DELAYNUS":      [0x49, 2, 0, DIR_BOTH, ""],
                "PKT_DELAYNNS":      [0x4A, 2, 0, DIR_BOTH, ""],
                "PKT_RTSTHRESH":     [0x4E, 5, 0, DIR_BOTH, ""],
                "PKT_GAIN":          [0x4B, 2, 0, DIR_BOTH, ""]
                }
            }

    def __init__(self):
        pass
    @classmethod
    def init(cls):
        #for t in self.types:
        pass

    @classmethod
    def make_header(cls, pkttype, name, args=None):
        sz = 1 + cls.TYPES[pkttype][name][1]
        return struct.pack(">BHB", cls.PKT_MAGIC, sz, pkttype)

    @classmethod
    def make_command(cls, name, args=None):
        return cls.make_header( cls.CONTROL, name, args) + struct.pack(">B", cls.TYPES[cls.CONTROL][name][0])

    @classmethod
    def parse_header(cls, s: bytes):
        magic,length,pkttype = struct.unpack(">BHB", s[:4])
        assert magic == cls.PKT_MAGIC
        return pkttype, length

    @classmethod
    def parse(cls, s: bytes):
        pkttype, length = AMBE.parse_header(s[:4])
        offset = 4
        remaining = s[offset:offset+length]
        if pkttype == cls.CONTROL:
            fieldtype = remaining[0]
            for fn,fv in cls.TYPES[pkttype].items():
                if fv[0] == fieldtype:
                    return [fn,fv, remaining[1:]]
                    

        
while 0:
    try:
        ser = serial.Serial('/dev/ttyUSB0', 460800, timeout=.002)
        while 1:
            sys.stdout.buffer.write(ser.read())
            sys.stdout.flush() #needed to print to screen!
    except serial.serialutil.SerialException:
        time.sleep(0.001)
        continue
# while 1:
    # sys.stdout.buffer.write(ser.read())
    # sys.stdout.flush() #needed to print to screen!

a = AMBE()
x = a.make_command("PKT_RESET")
assert x == amberesetvector

ser = serial.Serial('/dev/ttyUSB0', 460800, timeout=.002)
print(ser.read(4096))
ser.write(amberesetvector)
ser.flush()
time.sleep(.002)
while 1:
    ret = ser.read(1024)
    idx = ret.find(b"a")
    print(idx, ret)
    if idx >= 0:
        x = AMBE.parse( ret[idx:] )
        print(x[0], x[2]) #should be PKT_READY
        import pdb; pdb.set_trace()

