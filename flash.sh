#!/usr/bin/env bash

set -euo pipefail
make

openocd -f interface/stlink.cfg \
        -f target/stm32f4x.cfg \
        -c "program build/boot.bin 0x08000000 verify reset exit"

echo 'flashing complete...'