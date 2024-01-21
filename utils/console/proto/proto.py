#!/usr/bin/env python3
import ctypes as ct
from enum import Enum, auto

from cobs import cobsr


class CommandId(Enum):
    CMDID_SET_SETTINGS = 0x00
    CMDID_GET_SETTINGS = 0x01
    CMDID_SET_RTC = 0x02
    CMDID_GET_RTC = 0x03
    CMDID_GET_REPORTS = 0x04
    CMDID_STORAGE_CLEAR = 0x05
    CMDID_LAST = auto()


class RtcTime(ct.Structure):
    _fields_ = [
        ('time', ct.c_uint32),
    ]

class Reports(ct.Structure):
    _fields_ = [
    ]

DataPointer = ct.POINTER(ct.c_uint8)

class MessageLowLevel(ct.Structure):
    _pack_ = 1
    _fields_ = [
        ('size', ct.c_uint32),
        ('id', ct.c_uint32),
        ('channel', ct.c_uint32),
        ('data', DataPointer),
    ]


class Message:

    @staticmethod
    def len() -> int:
        return ct.sizeof(MessageLowLevel) - ct.sizeof(DataPointer)

    @staticmethod
    def serialize(cmd: Enum, channel: int, data: ct.Structure | None) -> bytes:
        size = Message.len() if data is None else ct.sizeof(data) + Message.len()
        msg = MessageLowLevel(size=size, id=cmd.value, channel=channel)
        buffer = (ct.c_uint8 * size)()
        ct.memmove(ct.addressof(buffer), ct.addressof(msg), Message.len())
        if data is not None:
            ct.memmove(
                ct.addressof(buffer) + Message.len(), ct.addressof(data),
                ct.sizeof(data))
        return cobsr.encode(bytes(buffer)) + b'\x00'
