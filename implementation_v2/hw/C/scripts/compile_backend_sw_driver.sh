#!/bin/bash
DEFINES="-D SW -D BACKEND -D NUSE_DOUBLE -D NDEBUGPRINT -D READBACK -D NUSEALLONES"
INCLUDES="-I $AHIR_RELEASE/include -I include"
LIBPATH="-L $AHIR_RELEASE/lib "
LIBS=" -lpthread -lPipeHandlerDebugPthreads -lm "
CFLAGS=" -g "
rm -rf objswbe
mkdir objswbe
#./src:
gcc $CFLAGS -c src/best_fit.c  $DEFINES $INCLUDES -o objswbe/best_fit.o
gcc $CFLAGS -c src/hermite_fitter.c $DEFINES $INCLUDES -o objswbe/hermite_fitter.o
gcc $CFLAGS -c src/frontend.c $DEFINES $INCLUDES -o objswbe/frontend.o
gcc $CFLAGS -c src/utils.c  $DEFINES -DDEBUGPRINT $INCLUDES -o objswbe/utils.o
gcc $CFLAGS -c src/beat.c  $DEFINES $INCLUDES -o objswbe/beat.o
gcc $CFLAGS -c src/fpops.c  $DEFINES $INCLUDES -o objswbe/fpops.o
gcc $CFLAGS -c src/derivative.c  $DEFINES $INCLUDES -o objswbe/derivative.o
gcc $CFLAGS -c src/band_pass_filter.c  $DEFINES $INCLUDES -o objswbe/band_pass_filter.o
gcc $CFLAGS -c src/moving_average.c  $DEFINES $INCLUDES -o objswbe/moving_average.o
gcc $CFLAGS -c src/qrs_peak_detect.c  $DEFINES $INCLUDES -o objswbe/qrs_peak_detect.o
#./driver:
gcc $CFLAGS -c driver/config.c $DEFINES $INCLUDES -o objswbe/config.o
gcc $CFLAGS -c driver/hermite.c $DEFINES $INCLUDES -o objswbe/hermite.o
gcc $CFLAGS -c driver/test_backend.c $DEFINES $INCLUDES -o objswbe/test_backend.o
gcc $CFLAGS -c driver/tb_utils.c $DEFINES -DDEBUGPRINT $INCLUDES -o objswbe/tb_utils.o
# link
if [ ! -d ./bin ] 
then
  mkdir bin
fi
gcc $CFLAGS -o bin/test_backend_sw objswbe/*.o $LIBPATH $LIBS
