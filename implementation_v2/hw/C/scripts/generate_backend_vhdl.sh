#!/bin/bash
DEFINES="-D FPGA -D BACKEND -D NUSE_DOUBLE -D NMONITORBACKEND -D NMONITORFRONTEND -D NREADBACK "
INCLUDES="-I $AHIR_RELEASE/include -I include"
LIBPATH="-L $AHIR_RELEASE/lib "
LIBS=" -lpthread -lPipeHandler -lm "
CFLAGS=" -m32 -O3 -std=gnu89 -emit-llvm  "
#LLVM2AAOPTS="-extract_do_while=true --storageinit=true -pipedepths=pipedepths.txt"
LLVM2AAOPTS=" -extract_do_while=true "
TOPMODULES=" -T hermiteFitterDaemon "
rm -rf objvhdlbe
mkdir objvhdlbe
rm -rf .Aa
mkdir .Aa
rm -rf .vC
mkdir .vC
#./src:
clang $CFLAGS -c src/best_fit.c  $DEFINES $INCLUDES -o objvhdlbe/best_fit.o
opt --indvars --loopsimplify objvhdlbe/best_fit.o -o objvhdlbe/best_fit.opt.o
llvm-dis objvhdlbe/best_fit.opt.o -o objvhdlbe/best_fit.opt.o.ll
llvm2aa $LLVM2AAOPTS  objvhdlbe/best_fit.opt.o | vcFormat > .Aa/best_fit.aa

clang $CFLAGS -c src/hermite_fitter.c $DEFINES $INCLUDES -o objvhdlbe/hermite_fitter.o
opt --indvars --loopsimplify objvhdlbe/hermite_fitter.o -o objvhdlbe/hermite_fitter.opt.o
llvm-dis objvhdlbe/hermite_fitter.opt.o -o objvhdlbe/hermite_fitter.opt.o.ll
llvm2aa $LLVM2AAOPTS  objvhdlbe/hermite_fitter.opt.o | vcFormat > .Aa/hermite_fitter.aa

clang $CFLAGS -c src/frontend.c $DEFINES $INCLUDES -o objvhdlbe/frontend.o
opt --indvars --loopsimplify objvhdlbe/frontend.o -o objvhdlbe/frontend.opt.o
llvm-dis objvhdlbe/frontend.opt.o -o objvhdlbe/frontend.opt.o.ll
llvm2aa $LLVM2AAOPTS  objvhdlbe/frontend.opt.o | vcFormat > .Aa/frontend.aa

clang $CFLAGS -c src/beat.c  $DEFINES $INCLUDES -o objvhdlbe/beat.o
opt --indvars --loopsimplify objvhdlbe/beat.o -o objvhdlbe/beat.opt.o
llvm-dis objvhdlbe/beat.opt.o -o objvhdlbe/beat.opt.o.ll
llvm2aa $LLVM2AAOPTS  objvhdlbe/beat.opt.o | vcFormat > .Aa/beat.aa


clang $CFLAGS -c src/derivative.c  $DEFINES $INCLUDES -o objvhdlbe/derivative.o
opt --indvars --loopsimplify objvhdlbe/derivative.o -o objvhdlbe/derivative.opt.o
llvm-dis objvhdlbe/derivative.opt.o -o objvhdlbe/derivative.opt.o.ll
llvm2aa $LLVM2AAOPTS  objvhdlbe/derivative.opt.o | vcFormat > .Aa/derivative.aa

clang $CFLAGS -c src/band_pass_filter.c  $DEFINES $INCLUDES -o objvhdlbe/band_pass_filter.o
opt --indvars --loopsimplify objvhdlbe/band_pass_filter.o -o objvhdlbe/band_pass_filter.opt.o
llvm-dis objvhdlbe/band_pass_filter.opt.o -o objvhdlbe/band_pass_filter.opt.o.ll
llvm2aa $LLVM2AAOPTS  objvhdlbe/band_pass_filter.opt.o | vcFormat > .Aa/band_pass_filter.aa

clang $CFLAGS -c src/moving_average.c  $DEFINES $INCLUDES -o objvhdlbe/moving_average.o
opt --indvars --loopsimplify objvhdlbe/moving_average.o -o objvhdlbe/moving_average.opt.o
llvm-dis objvhdlbe/moving_average.opt.o -o objvhdlbe/moving_average.opt.o.ll
llvm2aa $LLVM2AAOPTS  objvhdlbe/moving_average.opt.o | vcFormat > .Aa/moving_average.aa

clang $CFLAGS -c src/qrs_peak_detect.c  $DEFINES $INCLUDES -o objvhdlbe/qrs_peak_detect.o
opt --indvars --loopsimplify objvhdlbe/qrs_peak_detect.o -o objvhdlbe/qrs_peak_detect.opt.o
llvm-dis objvhdlbe/qrs_peak_detect.opt.o -o objvhdlbe/qrs_peak_detect.opt.o.ll
llvm2aa $LLVM2AAOPTS  objvhdlbe/qrs_peak_detect.opt.o | vcFormat > .Aa/qrs_peak_detect.aa

AaLinkExtMem .Aa/band_pass_filter.aa .Aa/best_fit.aa .Aa/beat.aa .Aa/derivative.aa .Aa/frontend.aa .Aa/hermite_fitter.aa .Aa/moving_average.aa .Aa/qrs_peak_detect.aa Aa/utils.aa Aa/ajit_fpu.aa  | vcFormat > .Aa/ecg.linked.aa
AaOpt -C .Aa/ecg.linked.aa | vcFormat > .Aa/ecg.linked.optC.aa
AaOpt -B .Aa/ecg.linked.optC.aa | vcFormat > .Aa/ecg.linked.opt.aa
Aa2VC -O -C -r controllerDaemon -r hermiteFitterDaemon -r beatDaemon .Aa/ecg.linked.opt.aa | vcFormat > .vC/ecg.vc

	
# generate vhdl from vc
rm -rf backend_vhdl
mkdir backend_vhdl
vc2vhdl  -O -I 2  -a -C -e ecg_core -w -s ghdl $TOPMODULES -f .vC/ecg.vc 
vhdlFormat < ecg_core_global_package.unformatted_vhdl > backend_vhdl/ecg_core_global_package.vhdl
vhdlFormat < ecg_core.unformatted_vhdl > backend_vhdl/ecg_core.vhdl
vhdlFormat < ecg_core_test_bench.unformatted_vhdl > backend_vhdl/ecg_core_test_bench.vhdl
rm -f *.unformatted_vhdl

