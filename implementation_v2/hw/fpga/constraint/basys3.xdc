## This file is a general .xdc for the Basys3 rev B board
## To use it in a project:
## - uncomment the lines corresponding to used pins
## - rename the used ports (in each line, after get_ports) according to the top level signal names in the project

# Clock signal
set_property PACKAGE_PIN W5 [get_ports CLK100MHZ]							
set_property IOSTANDARD LVCMOS33 [get_ports CLK100MHZ]
create_clock -add -name sys_clk_pin -period 10.00 -waveform {0 5}   [get_ports CLK100MHZ]
 

# LEDS
set_property PACKAGE_PIN U16 [get_ports {LEDS[0]}]					
	set_property IOSTANDARD LVCMOS33 [get_ports {LEDS[0]}]
set_property PACKAGE_PIN E19 [get_ports {LEDS[1]}]					
	set_property IOSTANDARD LVCMOS33 [get_ports {LEDS[1]}]
set_property PACKAGE_PIN U19 [get_ports {LEDS[2]}]					
	set_property IOSTANDARD LVCMOS33 [get_ports {LEDS[2]}]
set_property PACKAGE_PIN V19 [get_ports {LEDS[3]}]					
	set_property IOSTANDARD LVCMOS33 [get_ports {LEDS[3]}]
set_property PACKAGE_PIN W18 [get_ports {LEDS[4]}]					
	set_property IOSTANDARD LVCMOS33 [get_ports {LEDS[4]}]
set_property PACKAGE_PIN U15 [get_ports {LEDS[5]}]					
	set_property IOSTANDARD LVCMOS33 [get_ports {LEDS[5]}]
set_property PACKAGE_PIN U14 [get_ports {LEDS[6]}]					
	set_property IOSTANDARD LVCMOS33 [get_ports {LEDS[6]}]
set_property PACKAGE_PIN V14 [get_ports {LEDS[7]}]					
	set_property IOSTANDARD LVCMOS33 [get_ports {LEDS[7]}]
	
	


##Buttons
set_property PACKAGE_PIN U18 [get_ports btnC]						
	set_property IOSTANDARD LVCMOS33 [get_ports btnC]
set_property PACKAGE_PIN T18 [get_ports btnU]						
	set_property IOSTANDARD LVCMOS33 [get_ports btnU]
#set_property PACKAGE_PIN W19 [get_ports btnL]						
	#set_property IOSTANDARD LVCMOS33 [get_ports btnL]
#set_property PACKAGE_PIN T17 [get_ports btnR]						
	#set_property IOSTANDARD LVCMOS33 [get_ports btnR]
#set_property PACKAGE_PIN U17 [get_ports btnD]						
	#set_property IOSTANDARD LVCMOS33 [get_ports btnD]

# allow suboptimal placement for BUFG-BUFG cascade pair.
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets clk_gen/inst/clk_out1] 


##Pmod Header JA
##Sch name = JA1
#set_property PACKAGE_PIN J1 [get_ports {JA[0]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JA[0]}]
##Sch name = JA2
#set_property PACKAGE_PIN L2 [get_ports {JA[1]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JA[1]}]
##Sch name = JA3
#set_property PACKAGE_PIN J2 [get_ports {JA[2]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JA[2]}]
##Sch name = JA4
#set_property PACKAGE_PIN G2 [get_ports {JA[3]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JA[3]}]
##Sch name = JA7
#set_property PACKAGE_PIN H1 [get_ports {JA[4]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JA[4]}]
##Sch name = JA8
#set_property PACKAGE_PIN K2 [get_ports {JA[5]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JA[5]}]
##Sch name = JA9
#set_property PACKAGE_PIN H2 [get_ports {JA[6]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JA[6]}]
##Sch name = JA10
#set_property PACKAGE_PIN G3 [get_ports {JA[7]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JA[7]}]



##Pmod Header JB
##Sch name = JB1
#set_property PACKAGE_PIN A14 [get_ports {adc_data_0}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {adc_data_0}]
##Sch name = JB2
#set_property PACKAGE_PIN A16 [get_ports {adc_clk}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {adc_clk}]
##Sch name = JB3
#set_property PACKAGE_PIN B15 [get_ports {adc_data_1}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {adc_data_1}]
##Sch name = JB4
#set_property PACKAGE_PIN B16 [get_ports {JB[3]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JB[3]}]

#
# 5 is ground
# 6 is power (3.3V?)
#

##Sch name = JB7
#set_property PACKAGE_PIN A15 [get_ports {JB[4]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JB[4]}]
##Sch name = JB8
#set_property PACKAGE_PIN A17 [get_ports {JB[5]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JB[5]}]
##Sch name = JB9
#set_property PACKAGE_PIN C15 [get_ports {JB[6]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JB[6]}]
##Sch name = JB10 
#set_property PACKAGE_PIN C16 [get_ports {JB[7]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JB[7]}]

# 11 is ground
# 12 is power (3.3V?)
 


##Pmod Header JC
##Sch name = JC1
#set_property PACKAGE_PIN K17 [get_ports {JC[0]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JC[0]}]
##Sch name = JC2
#set_property PACKAGE_PIN M18 [get_ports {JC[1]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JC[1]}]
##Sch name = JC3
#set_property PACKAGE_PIN N17 [get_ports {JC[2]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JC[2]}]
##Sch name = JC4
#set_property PACKAGE_PIN P18 [get_ports {JC[3]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JC[3]}]
##Sch name = JC7
#set_property PACKAGE_PIN L17 [get_ports {JC[4]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JC[4]}]
##Sch name = JC8
#set_property PACKAGE_PIN M19 [get_ports {JC[5]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JC[5]}]
##Sch name = JC9
#set_property PACKAGE_PIN P17 [get_ports {JC[6]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JC[6]}]
##Sch name = JC10
#set_property PACKAGE_PIN R18 [get_ports {JC[7]}]					
	#set_property IOSTANDARD LVCMOS33 [get_ports {JC[7]}]


#Pmod Header JXADC
#Sch name = XA1_P
#set_property PACKAGE_PIN J3 [get_ports {vauxp6}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vauxp6}]
#Sch name = XA2_P
#set_property PACKAGE_PIN L3 [get_ports {vauxp14}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vauxp14}]
#Sch name = XA3_P
#set_property PACKAGE_PIN M2 [get_ports {vauxp7}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vauxp7}]
#Sch name = XA4_P
#set_property PACKAGE_PIN N2 [get_ports {vauxp15}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vauxp15}]
#Sch name = XA1_N
#set_property PACKAGE_PIN K3 [get_ports {vauxn6}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vauxn6}]
#Sch name = XA2_N
#set_property PACKAGE_PIN M3 [get_ports {vauxn14}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vauxn14}]
#Sch name = XA3_N
#set_property PACKAGE_PIN M1 [get_ports {vauxn7}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vauxn7}]
#Sch name = XA4_N
#set_property PACKAGE_PIN N1 [get_ports {vauxn15}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vauxn15}]



##VGA Connector
#set_property PACKAGE_PIN G19 [get_ports {vgaRed[0]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vgaRed[0]}]
#set_property PACKAGE_PIN H19 [get_ports {vgaRed[1]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vgaRed[1]}]
#set_property PACKAGE_PIN J19 [get_ports {vgaRed[2]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vgaRed[2]}]
#set_property PACKAGE_PIN N19 [get_ports {vgaRed[3]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vgaRed[3]}]
#set_property PACKAGE_PIN N18 [get_ports {vgaBlue[0]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vgaBlue[0]}]
#set_property PACKAGE_PIN L18 [get_ports {vgaBlue[1]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vgaBlue[1]}]
#set_property PACKAGE_PIN K18 [get_ports {vgaBlue[2]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vgaBlue[2]}]
#set_property PACKAGE_PIN J18 [get_ports {vgaBlue[3]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vgaBlue[3]}]
#set_property PACKAGE_PIN J17 [get_ports {vgaGreen[0]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vgaGreen[0]}]
#set_property PACKAGE_PIN H17 [get_ports {vgaGreen[1]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vgaGreen[1]}]
#set_property PACKAGE_PIN G17 [get_ports {vgaGreen[2]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vgaGreen[2]}]
#set_property PACKAGE_PIN D17 [get_ports {vgaGreen[3]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {vgaGreen[3]}]
#set_property PACKAGE_PIN P19 [get_ports Hsync]						
	#set_property IOSTANDARD LVCMOS33 [get_ports Hsync]
#set_property PACKAGE_PIN R19 [get_ports Vsync]						
	#set_property IOSTANDARD LVCMOS33 [get_ports Vsync]


##USB-RS232 Interface
set_property PACKAGE_PIN B18 [get_ports RsRx]						
	set_property IOSTANDARD LVCMOS33 [get_ports RsRx]
set_property PACKAGE_PIN A18 [get_ports RsTx]						
	set_property IOSTANDARD LVCMOS33 [get_ports RsTx]


##USB HID (PS/2)
#set_property PACKAGE_PIN C17 [get_ports PS2Clk]						
	#set_property IOSTANDARD LVCMOS33 [get_ports PS2Clk]
	#set_property PULLUP true [get_ports PS2Clk]
#set_property PACKAGE_PIN B17 [get_ports PS2Data]					
	#set_property IOSTANDARD LVCMOS33 [get_ports PS2Data]	
	#set_property PULLUP true [get_ports PS2Data]


##Quad SPI Flash
##Note that CCLK_0 cannot be placed in 7 series devices. You can access it using the
##STARTUPE2 primitive.
#set_property PACKAGE_PIN D18 [get_ports {QspiDB[0]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {QspiDB[0]}]
#set_property PACKAGE_PIN D19 [get_ports {QspiDB[1]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {QspiDB[1]}]
#set_property PACKAGE_PIN G18 [get_ports {QspiDB[2]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {QspiDB[2]}]
#set_property PACKAGE_PIN F18 [get_ports {QspiDB[3]}]				
	#set_property IOSTANDARD LVCMOS33 [get_ports {QspiDB[3]}]
#set_property PACKAGE_PIN K19 [get_ports QspiCSn]					
	#set_property IOSTANDARD LVCMOS33 [get_ports QspiCSn]

