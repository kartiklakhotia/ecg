#!/bin/bash
DEFINES="-D HW -D NUSE_DOUBLE -D NMONITORBACKEND -D NMONITORFRONTEND -D DEBUGPRINT -D NREADBACK"
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
gcc $CFLAGS $DEFINES -o bin/test_best_fit_hw objvhdl/*.o $LIBPATH $LIBS
echo "building ghdl simulation model"
ghdl --clean
ghdl --remove
ghdl -i --work=GhdlLink  $VHDL_LIB/GhdlLink.vhdl
ghdl -i --work=aHiR_ieee_proposed  $VHDL_LIB/aHiR_ieee_proposed.vhdl
ghdl -i --work=ahir  $VHDL_LIB/ahir.vhdl
ghdl -i --work=work vhdl/ecg_core_global_package.vhdl
ghdl -i --work=work vhdl/ecg_core.vhdl
ghdl -i --work=work vhdl/ecg_core_test_bench.vhdl
ghdl -m --work=work -Wl,-L$AHIR_LIB -Wl,-lVhpi ecg_core_test_bench 

