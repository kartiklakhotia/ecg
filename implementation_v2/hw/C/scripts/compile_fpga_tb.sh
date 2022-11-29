#!/bin/bash
UARTINCL="$AJIT_PROJECT_HOME/processor/TestEnvironments/core_environment/uart_interface/include"
UARTLIB="$AJIT_PROJECT_HOME/processor/TestEnvironments/core_environment/uart_interface/lib"
DEFINES="-D FPGA -D NREADBACK -D NUSE_DOUBLE -D DEBUGPRINT "
INCLUDES="-I $AHIR_RELEASE/include -I include -I ./ -I $UARTINCL"
LIBPATH="-L $AHIR_RELEASE/lib -L $UARTLIB"
AHIR_LIB="$AHIR_RELEASE/lib"
VHDL_LIB="$AHIR_RELEASE/vhdl"
LIBS="  -lSocketLibPipeHandler -lm -luart_interface -lpthread "
CFLAGS=" -g "
rm -rf objfpga
mkdir objfpga
gcc $CFLAGS -c driver/config.c $DEFINES $INCLUDES -o objfpga/config.o
gcc $CFLAGS -c driver/hermite.c $DEFINES $INCLUDES -o objfpga/hermite.o
gcc $CFLAGS -c driver/test_best_fit.c $DEFINES $INCLUDES -o objfpga/test_best_fit.o
gcc $CFLAGS -c driver/tb_utils.c $DEFINES $INCLUDES -o objfpga/tb_utils.o
gcc $CFLAGS -c vhdlCStubs.c $DEFINES $INCLUDES -o objfpga/vhdlCStubs.o
# link
if [ ! -d ./bin ] 
then
  mkdir bin
fi
gcc $CFLAGS -o bin/test_best_fit_fpga objfpga/*.o $LIBPATH $LIBS
gcc $CFLAGS -o bin/test_ping driver/test_ping.c $INCLUDES $LIBPATH $LIBS
