#!/usr/bin/env bash

echo 0 >> /tmp/io_ext_pwr_pin
echo 0 >> /tmp/io_accel_irq_pin
echo 1 > /tmp/io_ext_pwr_pin
echo 0 > /tmp/rtc_time
