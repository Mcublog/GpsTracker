#!/usr/bin/env python3
import ctypes as ct


class GnssRecordV1(ct.Structure):
    _fields_ = [('version', ct.c_uint8), ('time', ct.c_uint64),
                ('latitude', ct.c_double), ('longitude', ct.c_double),
                ('altitude', ct.c_double)]


def main():
    print(f"GnssRecordV1 size: {ct.sizeof(GnssRecordV1)}")


if __name__ == "__main__":
    main()
