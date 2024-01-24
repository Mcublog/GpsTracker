import ctypes as ct


class AckV1(ct.Structure):
    _fields_ = [('channel', ct.c_uint32), ('status', ct.c_uint32)]