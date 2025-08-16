#!/usr/bin/env bash
qemu-system-arm \
    -M stm32f4discovery \
    -cpu cortex-m4 \
    -nographic \
    -serial mon:stdio \
    -kernel hello.bin

