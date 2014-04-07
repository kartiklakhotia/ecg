# build software version of testbench (to check the "desired behaviour")
AHIR_RELEASE=/home/ahir/ahir-master/release/
SOCKETLIB_INCLUDE=$(AHIR_RELEASE)/CtestBench/include
SOCKETLIB_LIB=$(AHIR_RELEASE)/CtestBench/lib
PIPEHANDLER_INCLUDE=$(AHIR_RELEASE)/pipeHandler/include
PIPEHANDLER_LIB=$(AHIR_RELEASE)/pipeHandler/lib
PTHREADUTILS_INCLUDE=$(AHIR_RELEASE)/pthreadUtils/include
VHDL_LIB=$(AHIR_RELEASE)/vhdl
VHDL_VHPI_LIB=$(AHIR_RELEASE)/CtestBench/vhdl
FUNCTIONLIB=$(AHIR_RELEASE)/functionLibrary/
SRC=./src
all: SW HW 
TOAA:c2llvmbc llvmbc2aa  aalink
TOVC:c2llvmbc llvmbc2aa  aalink aa2vc 
VC2VHDL: vc2vhdl  vhdlsim
AA2VHDLSIM: aa2vc  vc2vhdl  vhdlsim
TOVHDL:TOVC vc2vhdl

# llvm2aa opts: pipelined case, extract-do-while.
#LLVM2AAOPTS=--storageinit=true
LLVM2AAOPTS=-extract_do_while=true --storageinit=true -pipedepths=pipedepths.txt

#program defs: no unrolling
#PROGDEFS=-DPIPELINE 
#PROGDEFS=-DPIPELINE -DALT (what does this do, used in c2llvmbc)
#PROGDEFS=-DUNROLL
PROGDEFS=

TOPMODULES=-T QRSDet 


# compile with SW defined.
# note the use of IOLIB in building the testbench.
SW: $(SRC)/filter.c $(SRC)/detect.c $(SRC)/filter.h $(SRC)/detect.h  $(FUNCTIONLIB)/src/timer.c 
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(FUNCTIONLIB)/include -I$(SRC) $(SRC)/filter.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(FUNCTIONLIB)/include -I$(SRC) $(SRC)/detect.c
	gcc -g -c -DSW $(PROGDEFS) -I$(FUNCTIONLIB)/include -I$(SRC) $(FUNCTIONLIB)/src/timer.c
	gcc -g -c -DSW $(PROGDEFS) -I$(PIPEHANDLER_INCLUDE) -I$(PTHREADUTILS_INCLUDE) -I$(SRC) $(SRC)/testDetect.c
	gcc  -o testbench_sw filter.o detect.o timer.o testDetect.o -L$(PIPEHANDLER_LIB) -lPipeHandler -lpthread -lm -lrt

# five steps from C to vhdl simulator.
HW: c2llvmbc llvmbc2aa  aalink aa2vc  vc2vhdl  vhdlsim

AA2VHDL: aa2vc vc2vhdl vhdlsim

# C to llvm byte-code.. use clang.
CLANGOPTS=-O3
c2llvmbc: $(SRC)/filter.c $(SRC)/filter.h $(SRC)/detect.c $(SRC)/detect.h
	clang $(CLANGOPTS) -I/usr/include/i386-linux-gnu -std=gnu89 $(PROGDEFS) -I$(SRC)  -I$(SOCKETLIB_INCLUDE) -I$(FUNCTIONLIB)/include -emit-llvm -c $(SRC)/detect.c
	opt --indvars --loopsimplify detect.o -o detect.opt.o
	llvm-dis detect.opt.o
	clang $(CLANGOPTS) -I/usr/include/i386-linux-gnu -std=gnu89 $(PROGDEFS) -I$(SRC)  -I$(SOCKETLIB_INCLUDE) -I$(FUNCTIONLIB)/include -emit-llvm -c $(SRC)/filter.c
	opt --indvars --loopsimplify filter.o -o filter.opt.o
	llvm-dis filter.opt.o

# llvm byte-code to Aa..
llvmbc2aa:  filter.opt.o detect.opt.o 
	llvm2aa $(LLVM2AAOPTS)  filter.opt.o | vcFormat >  filter.aa
	llvm2aa $(LLVM2AAOPTS)  detect.opt.o | vcFormat >  detect.aa

# Aa to vC
aalink:  detect.aa
	AaLinkExtMem detect.aa filter.aa | vcFormat > prog.linked.aa
	AaOpt -B prog.linked.aa | vcFormat > prog.linked.opt.aa

aa2vc: prog.linked.opt.aa
	Aa2VC -O -C prog.linked.opt.aa | vcFormat > prog.vc

# vC to VHDL
vc2vhdl: prog.vc
	vc2vhdl -O -S 4 -I 2 -v -a -C -e ahir_system -w -s ghdl $(TOPMODULES) -f prog.vc -L $(FUNCTIONLIB)/timer.list
	vhdlFormat < ahir_system_global_package.unformatted_vhdl > ahir_system_global_package.vhdl
	vhdlFormat < ahir_system.unformatted_vhdl > ahir_system.vhdl
	vhdlFormat < ahir_system_test_bench.unformatted_vhdl > ahir_system_test_bench.vhdl

# build testbench and ghdl executable
# note the use of SOCKETLIB in building the testbench.
vhdlsim: ahir_system.vhdl ahir_system_test_bench.vhdl $(SRC)/testDetect.c vhdlCStubs.h vhdlCStubs.c
	gcc -c vhdlCStubs.c  -I$(SRC) -I./ -I$(SOCKETLIB_INCLUDE)
	gcc -c $(SRC)/testDetect.c -I$(PTHREADUTILS_INCLUDE) -I$(SRC) -I./ -I$(SOCKETLIB_INCLUDE)
	gcc -o testbench_hw testDetect.o vhdlCStubs.o  -L$(SOCKETLIB_LIB) -lSocketLib -lpthread -lm
	ghdl --clean
	ghdl --remove
	ghdl -i --work=GhdlLink  $(VHDL_LIB)/GhdlLink.vhdl
	ghdl -i --work=ahir  $(VHDL_LIB)/ahir.vhdl
	ghdl -i --work=aHiR_ieee_proposed  $(VHDL_LIB)/aHiR_ieee_proposed.vhdl
	ghdl -i --work=work ahir_system_global_package.vhdl
	ghdl -i --work=work ahir_system.vhdl
	ghdl -i --work=work ahir_system_test_bench.vhdl
	ghdl -m --work=work -Wl,-L$(SOCKETLIB_LIB) -Wl,-lVhpi ahir_system_test_bench 

clean:
	rm -rf *.o* *.cf *.*vhdl vhdlCStubs.* *.vcd in_data* out_data* testbench_sw testbench_hw ahir_system_test_bench vhpi.log *.aa *.vc *.lso xst *.ngc *_xmsgs *.xrpt pipeHandler.log *.srp *.ghw *.dot

PHONY: all clean	
