# -*- mode:gdb-script -*-

layout asm
layout regs

set history filename ~/.gdb_history
set history save

b main

source misc/utils.gdb