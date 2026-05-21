#!/usr/bin/env bash

set -euo pipefail
make

TMP=$(mktemp)
openocd -f interface/stlink.cfg \
        -f target/stm32f4x.cfg \
        -c "program build/boot.bin 0x08000000 verify" \
        &>"$TMP" &
OPENOCD_PID=$!

cleanup() {
    rm -f "$TMP"
    if [[ -v OPENOCD_PID ]]; then
        kill "$OPENOCD_PID" &>/dev/null || true
    fi
}
trap cleanup EXIT

echo 'Polling OpenOCD GDB server...'
until nc -z 127.0.0.1 3333; do
    if ! kill -0 "$OPENOCD_PID" &>/dev/null ; then
        unset OPENOCD_PID
        cat "$TMP"
        exit 1
    fi
    sleep 0.1
done

arm-none-eabihf-gdb -ex 'set pagination off' \
                    -ex 'symbol-file build/boot.syms' \
                    -ex 'target extended-remote localhost:3333' \
                    -ex 'monitor reset halt' \
                    -ex 'layout asm'