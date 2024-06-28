read_vhdl -library work ../../C/vhdl/ecg_core_global_package.vhdl
read_vhdl -library work ../../C/vhdl/ecg_core.vhdl
read_vhdl -library simpleUartLib  ../vhdl_external/simpleUartLib.vhdl
read_vhdl -library ahir ../vhdl_external/ahirXilinx.vhdl
read_vhdl -library ahir_ieee_proposed ../vhdl_external/aHiR_ieee_proposed.vhdl
read_vhdl -library work ../vhdl/serial_uart.vhdl
read_vhdl -library work ../vhdl/basys3_top.vhdl

############# CONSTRAINT FILE ###########
read_xdc ../constraint/basys3.xdc

############# IP CORE FILES #############
set_property part xc7a35tcpg236-3 [current_project]
read_ip ../ip/clk_wiz_0/clk_wiz_0.xci 

############### SYNTHESIZE ##############
synth_design -fsm_extraction off -flatten_hierarchy none -top basys3_top -part xc7a35tcpg236-3
write_checkpoint -force PostSynthCheckpoint.dcp
report_timing_summary -file timing.postsynth.rpt -nworst 4

report_utilization -file utilization_post_synth.rpt
report_utilization -hierarchical -file utilization_post_synth.hierarchical.rpt
opt_design  
place_design  -directive Explore
place_design -post_place_opt 
phys_opt_design  -directive Explore
route_design  -directive Explore
write_checkpoint -force PostPlaceRouteCheckpoint.dcp
report_timing_summary -file timing.rpt -nworst 10 -verbose
report_utilization -file utilization_post_place_and_route.rpt
report_utilization -hierarchical -file utilization_post_place_and_route.hierarchical.rpt
write_bitstream -force ecg.bit
