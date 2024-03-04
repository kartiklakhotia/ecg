#!/bin/bash
DEFINES="-D DEBUGPRINT -D VHDLSIM -D NSW -D NAA2C -D NUSE_DOUBLE -D NMONITORBACKEND -D NMONITORFRONTEND -D NREADBACK -D NDEBUGPRINT "
INCLUDES="-I $AHIR_RELEASE/include -I include -I ./ "
LIBPATH="-L $AHIR_RELEASE/lib -L $UARTLIB"
LIBS="  -lBitVectors -lSocketLibPipeHandler -l SockPipes -lm -lpthread "
CFLAGS=" -g "
rm -rf objvhdlsim
mkdir objvhdlsim
#./driver:
gcc $CFLAGS -c driver/config.c $DEFINES $INCLUDES -o objvhdlsim/config.o
gcc $CFLAGS -c driver/hermite.c $DEFINES $INCLUDES -o objvhdlsim/hermite.o
gcc $CFLAGS -c driver/test_best_fit.c $DEFINES $INCLUDES -o objvhdlsim/test_best_fit.o
gcc $CFLAGS -c driver/tb_utils.c $DEFINES $INCLUDES -o objvhdlsim/tb_utils.o
# link
if [ ! -d ./bin ] 
then
  mkdir bin
fi
gcc $CFLAGS -o bin/test_best_fit_vhdlsim objvhdlsim/*.o $LIBPATH $LIBS
