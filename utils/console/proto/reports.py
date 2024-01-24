#!/usr/bin/env python3
import ctypes as ct

from cobs import cobsr
from console.proto.ack import AckV1


class GnssRecordV1(ct.Structure):
    _fields_ = [('version', ct.c_uint8), ('time', ct.c_uint64),
                ('latitude', ct.c_double), ('longitude', ct.c_double),
                ('altitude', ct.c_double)]


def receive_handler(data: bytes) -> None:
    data = cobsr.decode(data[:-1])
    chunk_size = ct.sizeof(GnssRecordV1)
    chunks = [data[i:i + chunk_size] for i in range(0, len(data), chunk_size)]
    print(f"number of chunk {len(chunks)}")
    if len(chunks) == 1:
        try:
            ack = AckV1.from_buffer_copy(chunks[0])
        except Exception as e:
            print(e)
            return
        print(ack)



def main():
    print(f"GnssRecordV1 size: {ct.sizeof(GnssRecordV1)}")


if __name__ == "__main__":
    main()
