#!/usr/bin/env bash

socat PTY,link=/dev/ttyS10,raw,echo=0 PTY,link=/dev/ttyS11,raw,echo=0 &
socat PTY,link=/dev/ttyS12,raw,echo=0 PTY,link=/dev/ttyS13,raw,echo=0 &
sleep 2
chmod 777 /dev/ttyS11 /dev/ttyS10 /dev/ttyS12 /dev/ttyS13
echo "Communication: [HOST] /dev/ttyS10 <=> [DEV] /dev/ttyS11"
echo "GNSS Data:     [HOST] /dev/ttyS12 <=> [DEV] /dev/ttyS13"
