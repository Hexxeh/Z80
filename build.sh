#!/bin/bash

clear
python gen_opcodes.py > z80_opcodes.h
clang++ main.cc z80.cc -DDEBUG -o z80
./z80