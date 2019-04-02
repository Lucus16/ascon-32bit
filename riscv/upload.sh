#!/usr/bin/env bash

GDB_PORT=3333
openocd -s sdk -f openocd.cfg &
${PREFIX}-gdb "$1" --batch \
  -ex "set remotetimeout 240" \
  -ex "target extended-remote localhost:$GDB_PORT" \
  -ex "monitor reset halt" \
  -ex "monitor flash protect 0 64 last off" \
  -ex "load" \
  -ex "monitor resume" \
  -ex "monitor shutdown" \
  -ex "quit"
