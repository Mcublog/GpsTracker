#! /bin/env python3

import argparse
import sys
import time
from threading import Thread

import serial
from nmeasim.simulator import Simulator

VERSION = "0.0.1"
DESCRIPTION = f"NMEA sim v{VERSION}"

USAGE = '\n'.join((f"{DESCRIPTION} generation NMEA messages and sendng to port",
                  "Usage example: sim.py -p /dev/tyyS12"))

UART_DEFAULT_SPEED = 57600


def main():
    parser = argparse.ArgumentParser(prog='NMEA sim',
                                     description=f"{DESCRIPTION}",
                                     usage=USAGE)
    parser.add_argument(f'-p',
                        '--port',
                        type=str,
                        help=f'Target device port name (COM1)',
                        default='/dev/ttyS12',
                        required=True)

    try:
        port = parser.parse_args().port
    except Exception as e:
        print(f"ERROR: {e}")
        sys.exit()

    try:
        ser = serial.Serial(port)
    except Exception as e:
        print(f"ERROR: {e}")
        sys.exit()

    ser.write_timeout = 0  # Do not block simulator on serial writing

    sim = Simulator()
    worker = Thread(target=sim.serve, kwargs=dict(output=ser, blocking=True))
    worker.start()
    try:
        while True:
            time.sleep(1)
            print(f"{int(time.time())}: ...")
    except Exception as e:
        print(e)

    sim.kill()
    worker.join()


if __name__ == "__main__":
    main()
