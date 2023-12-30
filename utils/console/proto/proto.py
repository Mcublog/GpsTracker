#!/usr/bin/env python3
import ctypes as ct
from enum import Enum, auto

from cobs import cobsr


class CommandId(Enum):
    CMDID_SET_SETTINGS = 0x00
    CMDID_GET_SETTINGS = 0x01
    CMDID_SET_RTC = 0x02
    CMDID_GET_RTC = 0x03
    CMDID_LAST = auto()


class RtcTime(ct.Structure):
    _fields_ = [
        ('time', ct.c_uint32),
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
    def serialize(id: Enum, channel: int, data: ct.Structure) -> bytes:
        size = ct.sizeof(data) + Message.len()
        msg = MessageLowLevel(size=size, id=id.value, channel=channel)
        buffer = (ct.c_uint8 * size)()
        ct.memmove(ct.addressof(buffer), ct.addressof(msg), Message.len())
        ct.memmove(
            ct.addressof(buffer) + Message.len(), ct.addressof(data), ct.sizeof(data))
        return cobsr.encode(bytes(buffer)) + b'\x00'