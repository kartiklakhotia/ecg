#!/bin/bash
UARTINCL="$AJIT_PROJECT_HOME/processor/TestEnvironments/core_environment/uart_interface/include"
UARTLIB="$AJIT_PROJECT_HOME/processor/TestEnvironments/core_environment/uart_interface/lib"
DEFINES="-D FPGA -D NUSE_DOUBLE -D NFREADWRITE -D NREADBACK "
INCLUDES="-I $AHIR_RELEASE/include -I include -I ./ -I $UARTINCL"
LIBPATH="-L $AHIR_RELEASE/lib -L $UARTLIB"
LIBS="  -lBitVectors -lPipeHandler -l SockPipes -lm -luart_interface -lpthread "
CFLAGS=" -g "
rm -rf objsw
mkdir objsw
#./driver:
gcc $CFLAGS -c driver/config.c $DEFINES $INCLUDES -o objsw/config.o
gcc $CFLAGS -c driver/hermite.c $DEFINES $INCLUDES -o objsw/hermite.o
gcc $CFLAGS -c driver/configure_and_send.c $DEFINES $INCLUDES -o objsw/configure_and_send.o
gcc $CFLAGS -c driver/tb_utils.c $DEFINES $INCLUDES -o objsw/tb_utils.o
# link
if [ ! -d ./bin ] 
then
  mkdir bin
fi
gcc $CFLAGS -o bin/configure_and_send objsw/*.o $LIBPATH $LIBS
