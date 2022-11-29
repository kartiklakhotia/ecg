#!/bin/bash
DEFINES="-D FRONTEND -D USE_DOUBLE -D DEBUGPRINT"
INCLUDES="-I $AHIR_RELEASE/include -I include -I ./"
LIBPATH="-L $AHIR_RELEASE/lib "
AHIR_LIB="$AHIR_RELEASE/lib"
VHDL_LIB="$AHIR_RELEASE/vhdl"
LIBS=" -lpthread -lSocketLibPipeHandler -lm "
CFLAGS=" -g "
rm -rf objvhdl
mkdir objvhdl
gcc $CFLAGS -c driver/config.c $DEFINES $INCLUDES -o objvhdl/config.o
gcc $CFLAGS -c driver/hermite.c $DEFINES $INCLUDES -o objvhdl/hermite.o
gcc $CFLAGS -c driver/test_best_fit.c $DEFINES $INCLUDES -o objvhdl/test_best_fit.o
gcc $CFLAGS -c driver/tb_utils.c $DEFINES $INCLUDES -o objvhdl/tb_utils.o
gcc $CFLAGS -c vhdlCStubs.c $DEFINES $INCLUDES -o objvhdl/vhdlCStubs.o
# link
if [ ! -d ./bin ] 
then
  mkdir bin
fi
gcc $CFLAGS -o bin/test_best_fit_frontend_hw objvhdl/*.o $LIBPATH $LIBS
