#!/usr/bin/env bash

echo 0 > /tmp/io_accel_irq_pin
sleep 0.1
echo 1 > /tmp/io_accel_irq_pin
sleep 0.1
echo 0 > /tmp/io_accel_irq_pin