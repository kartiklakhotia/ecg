#!/bin/bash
DEFINES="-D SW -D FRONTEND -D NUSE_DOUBLE -D NDEBUGPRINT -D NREADBACK"
INCLUDES="-I $AHIR_RELEASE/include -I include"
LIBPATH="-L $AHIR_RELEASE/lib "
LIBS=" -lpthread -lPipeHandlerDebugPthreads -lm "
CFLAGS=" -g "
rm -rf objsw
mkdir objsw
#./src:
gcc $CFLAGS -c src/best_fit.c  $DEFINES $INCLUDES -o objsw/best_fit.o
gcc $CFLAGS -c src/hermite_fitter.c $DEFINES $INCLUDES -o objsw/hermite_fitter.o
gcc $CFLAGS -c src/frontend.c $DEFINES $INCLUDES -o objsw/frontend.o
gcc $CFLAGS -c src/utils.c  $DEFINES -DDEBUGPRINT $INCLUDES -o objsw/utils.o
gcc $CFLAGS -c src/beat.c  $DEFINES $INCLUDES -o objsw/beat.o
gcc $CFLAGS -c src/fpops.c  $DEFINES $INCLUDES -o objsw/fpops.o
gcc $CFLAGS -c src/derivative.c  $DEFINES $INCLUDES -o objsw/derivative.o
gcc $CFLAGS -c src/band_pass_filter.c  $DEFINES $INCLUDES -o objsw/band_pass_filter.o
gcc $CFLAGS -c src/moving_average.c  $DEFINES $INCLUDES -o objsw/moving_average.o
gcc $CFLAGS -c src/qrs_peak_detect.c  $DEFINES $INCLUDES -o objsw/qrs_peak_detect.o
#./driver:
gcc $CFLAGS -c driver/config.c $DEFINES $INCLUDES -o objsw/config.o
gcc $CFLAGS -c driver/hermite.c $DEFINES $INCLUDES -o objsw/hermite.o
gcc $CFLAGS -c driver/test_best_fit.c $DEFINES $INCLUDES -o objsw/test_best_fit.o
gcc $CFLAGS -c driver/tb_utils.c $DEFINES -DDEBUGPRINT $INCLUDES -o objsw/tb_utils.o
# link
rm -rf bin
mkdir bin
gcc $CFLAGS -o bin/test_best_fit_frontend_sw objsw/*.o $LIBPATH $LIBS
