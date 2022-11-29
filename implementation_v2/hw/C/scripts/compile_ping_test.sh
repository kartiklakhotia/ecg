#!/bin/bash
UARTINCL="$AJIT_PROJECT_HOME/processor/TestEnvironments/core_environment/uart_interface/include"
UARTLIB="$AJIT_PROJECT_HOME/processor/TestEnvironments/core_environment/uart_interface/lib"
INCLUDES="-I $AHIR_RELEASE/include -I include -I ./ -I $UARTINCL"
LIBPATH="-L $AHIR_RELEASE/lib -L $UARTLIB"
AHIR_LIB="$AHIR_RELEASE/lib"
VHDL_LIB="$AHIR_RELEASE/vhdl"
LIBS="  -lSocketLibPipeHandler -lm -luart_interface -lpthread "
CFLAGS=" -g "
if [ ! -d ./bin ] 
then
  mkdir bin
fi
gcc $CFLAGS -o bin/test_ping driver/test_ping.c $INCLUDES $LIBPATH $LIBS
gcc $CFLAGS -DFPGA -o bin/test_pingf driver/test_pingf.c driver/tb_utils.c $INCLUDES $LIBPATH $LIBS
gcc $CFLAGS -DFPGA -o bin/test_pingf_mt driver/test_pingf_mt.c driver/tb_utils.c $INCLUDES $LIBPATH $LIBS
gcc $CFLAGS -o bin/test_ping_mt driver/test_ping_mt.c $INCLUDES $LIBPATH $LIBS
