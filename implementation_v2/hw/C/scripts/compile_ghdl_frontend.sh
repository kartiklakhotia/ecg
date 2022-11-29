#!/bin/bash
DEFINES="-D HW -D USE_DOUBLE"
INCLUDES="-I $AHIR_RELEASE/include -I include -I ./"
LIBPATH="-L $AHIR_RELEASE/lib "
AHIR_LIB="$AHIR_RELEASE/lib"
VHDL_LIB="$AHIR_RELEASE/vhdl"
LIBS=" -lpthread -lSocketLibPipeHandler -lm "
CFLAGS=" -g "
rm -rf objfrontend_vhdl
mkdir objfrontend_vhdl
gcc $CFLAGS -c driver/config.c $DEFINES $INCLUDES -o objfrontend_vhdl/config.o
gcc $CFLAGS -c driver/hermite.c $DEFINES $INCLUDES -o objfrontend_vhdl/hermite.o
gcc $CFLAGS -c driver/test_best_fit.c $DEFINES $INCLUDES -o objfrontend_vhdl/test_best_fit.o
gcc $CFLAGS -c driver/tb_utils.c $DEFINES $INCLUDES -o objfrontend_vhdl/tb_utils.o
gcc $CFLAGS -c vhdlCStubs.c $DEFINES $INCLUDES -o objfrontend_vhdl/vhdlCStubs.o
# link
if [ ! -d ./bin ] 
then
  mkdir bin
fi
gcc $CFLAGS -o bin/test_best_fit_hw objfrontend_vhdl/*.o $LIBPATH $LIBS
echo "building ghdl simulation model"
ghdl --clean
ghdl --remove
ghdl -i --work=GhdlLink  $VHDL_LIB/GhdlLink.vhdl
ghdl -i --work=aHiR_ieee_proposed  $VHDL_LIB/aHiR_ieee_proposed.vhdl
ghdl -i --work=ahir  $VHDL_LIB/ahir.vhdl
ghdl -i --work=work frontend_vhdl/ecg_frontend_global_package.vhdl
ghdl -i --work=work frontend_vhdl/ecg_frontend.vhdl
ghdl -i --work=work frontend_vhdl/ecg_frontend_test_bench.vhdl
ghdl -m --work=work -Wl,-L$AHIR_LIB -Wl,-lVhpi ecg_frontend_test_bench 

