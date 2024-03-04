#!/bin/bash
CWD=$(pwd)
#
# go to the C directory and
# build.
#
cd C
./generate_vhdl.gated_clock.sh
./compile_vhdl_driver.sh
cd ghdl
. ghdl.do
cd $CWD
