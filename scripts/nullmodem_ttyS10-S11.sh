#!/usr/bin/env bash

socat PTY,link=/dev/ttyS10,raw,echo=0 PTY,link=/dev/ttyS11,raw,echo=0 &
sleep 2
chmod 777 /dev/ttyS11 /dev/ttyS10
echo "/dev/ttyS10 <=> /dev/ttyS11"
