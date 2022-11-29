#!/bin/bash
VHDL_LIB="$AHIR_RELEASE/vhdl"
echo "building ghdl synth simulation model"
ghdl --clean
ghdl --remove
ghdl -i --ieee=synopsys --work=GhdlLink  $VHDL_LIB/GhdlLink.vhdl
ghdl -i --ieee=synopsys --work=aHiR_ieee_proposed  $VHDL_LIB/aHiR_ieee_proposed.vhdl
ghdl -i --ieee=synopsys --work=ahir  $VHDL_LIB/ahir.vhdl
ghdl -i --ieee=synopsys --no-vital-checks --work=unisim -fexplicit $AHIR_RELEASE/vhdl/unisims/unisim_VCOMP.vhd
ghdl -i --ieee=synopsys --no-vital-checks --work=unisim -fexplicit $AHIR_RELEASE/vhdl/unisims/unisim_VPKG.vhd
ghdl -i --ieee=synopsys --no-vital-checks --work=unisim -fexplicit $AHIR_RELEASE/vhdl/unisims/primitive/*.vhd
ghdl -i --ieee=synopsys --work=work ../basys3/vivado_synth/ecg_coreS.vhdl
ghdl -i --ieee=synopsys --work=work vhdl/ecg_core_global_package.vhdl
ghdl -i --ieee=synopsys --work=work vhdl/ecg_core_test_bench.vhdl
ghdl -m --ieee=synopsys --work=work -Wl,-L$AHIR_LIB -Wl,-lVhpi ecg_core_test_bench 

