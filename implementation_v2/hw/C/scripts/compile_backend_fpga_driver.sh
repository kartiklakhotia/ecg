#!/bin/bash
UARTINCL="$AJIT_PROJECT_HOME/processor/TestEnvironments/core_environment/uart_interface/include"
UARTLIB="$AJIT_PROJECT_HOME/processor/TestEnvironments/core_environment/uart_interface/lib"
DEFINES="-D FPGA -D BACKEND -D NUSE_DOUBLE -D NDEBUGPRINT -D NREADBACK"
INCLUDES="-I $AHIR_RELEASE/include -I include -I ./ -I $UARTINCL"
LIBPATH="-L $AHIR_RELEASE/lib -L $UARTLIB"
LIBS="-lPipeHandlerDebugPthreads -lm -luart_interface -lpthread"
CFLAGS=" -g "
rm -rf objfpgabe
mkdir objfpgabe
#./src:
gcc $CFLAGS -c src/best_fit.c  $DEFINES $INCLUDES -o objfpgabe/best_fit.o
gcc $CFLAGS -c src/hermite_fitter.c $DEFINES $INCLUDES -o objfpgabe/hermite_fitter.o
gcc $CFLAGS -c src/frontend.c $DEFINES $INCLUDES -o objfpgabe/frontend.o
gcc $CFLAGS -c src/utils.c  $DEFINES -DDEBUGPRINT $INCLUDES -o objfpgabe/utils.o
gcc $CFLAGS -c src/beat.c  $DEFINES $INCLUDES -o objfpgabe/beat.o
gcc $CFLAGS -c src/fpops.c  $DEFINES $INCLUDES -o objfpgabe/fpops.o
gcc $CFLAGS -c src/derivative.c  $DEFINES $INCLUDES -o objfpgabe/derivative.o
gcc $CFLAGS -c src/band_pass_filter.c  $DEFINES $INCLUDES -o objfpgabe/band_pass_filter.o
gcc $CFLAGS -c src/moving_average.c  $DEFINES $INCLUDES -o objfpgabe/moving_average.o
gcc $CFLAGS -c src/qrs_peak_detect.c  $DEFINES $INCLUDES -o objfpgabe/qrs_peak_detect.o
#./driver:
gcc $CFLAGS -c driver/config.c $DEFINES $INCLUDES -o objfpgabe/config.o
gcc $CFLAGS -c driver/hermite.c $DEFINES $INCLUDES -o objfpgabe/hermite.o
gcc $CFLAGS -c driver/test_backend.c $DEFINES $INCLUDES -o objfpgabe/test_backend.o
gcc $CFLAGS -c driver/tb_utils.c $DEFINES -DDEBUGPRINT $INCLUDES -o objfpgabe/tb_utils.o
# link
rm -rf bin
mkdir bin
gcc $CFLAGS -o bin/test_backend_fpga objfpgabe/*.o $LIBPATH $LIBS
