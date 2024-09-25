ghdl -i --work=ahir_ieee_proposed ../../fpga/vhdl_external//aHiR_ieee_proposed.vhdl
ghdl -i --work=ahir ../../fpga/vhdl_external/ahir.vhdl
ghdl -i --work=GhdlLink ../../fpga//vhdl_external/GhdlLink.vhdl
ghdl -i --work=work ../vhdl/ecg_core_global_package.vhdl
ghdl -i --work=work ../vhdl/ecg_core.vhdl
ghdl -i --work=work ../vhdl/ecg_core_test_bench.vhdl
ghdl -m -Wc,-O3 -Wl,-L$AHIR_RELEASE/lib -Wl,-lVhpi ecg_core_test_bench 
