/* 
ChipWhisperer Artix Target - Example of connections between example registers
and rest of system.

Copyright (c) 2016-2020, NewAE Technology Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted without restriction. Note that modules within
the project may have additional restrictions, please carefully inspect
additional licenses.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of NewAE Technology Inc.
*/

`timescale 1ns / 1ps
`default_nettype none 

module cw305_top #(
    parameter pBYTECNT_SIZE = 7,
    parameter pADDR_WIDTH = 21,
    parameter pPT_WIDTH = 128,
    parameter pCT_WIDTH = 128,
    parameter pKEY_WIDTH = 128
)(
    // USB Interface
    input wire                          usb_clk,        // Clock
`ifdef SS2_WRAPPER
    output wire                         usb_clk_buf,    // if needed by parent module
`endif

//clk checking
     output wire                          clk_debug1,//usb_clk
     output wire                          clk_debug2,//CLK50MHZ
     output wire                          clklock,
     output wire                          clk_debug3,

//Declarations for basys3_top
    input wire                          CLK100MHZ,
    input wire                          RsRx,
    output wire                         RsTx,
//End of this part

    inout wire [7:0]                    usb_data,       // Data for write/read
    input wire [pADDR_WIDTH-1:0]        usb_addr,       // Address
    input wire                          usb_rdn,        // !RD, low when addr valid for read
    input wire                          usb_wrn,        // !WR, low when data+addr valid for write
    input wire                          usb_cen,        // !CE, active low chip enable
    input wire                          usb_trigger,    // High when trigger requested

    // Buttons/LEDs on Board
    input wire                          j16_sel,        // DIP switch J16
    input wire                          k16_sel,        // DIP switch K16
    input wire                          k15_sel,        // DIP switch K15
    input wire                          l14_sel,        // DIP Switch L14
    input wire                          pushbutton,     // Pushbutton SW4, connected to R1, used here as reset
    output wire                         led1,           // red LED
    output wire                         led2,           // green LED
    output wire                         led3           // blue LED

    // PLL
    //input wire                          pll_clk1,       //PLL Clock Channel #1
    //input wire                        pll_clk2,       //PLL Clock Channel #2 (unused in this example)

    // 20-Pin Connector Stuff
//    output wire                         tio_trigger,
//    output wire                         tio_clkout,
//    input  wire                         tio_clkin

    // Block Interface to Crypto Core
//`ifdef USE_BLOCK_INTERFACE
//   ,output wire                         crypto_clk,
//    output wire                         crypto_rst,
//    output wire [pPT_WIDTH-1:0]         crypto_textout,
//    output wire [pKEY_WIDTH-1:0]        crypto_keyout,
//    input  wire [pCT_WIDTH-1:0]         crypto_cipherin,
//    output wire                         crypto_start,
//    input wire                          crypto_ready,
//    input wire                          crypto_done,
//    input wire                          crypto_busy,
//    input wire                          crypto_idle
//`endif
    );

`ifndef SS2_WRAPPER
    wire usb_clk_buf;
`endif

//    wire [pKEY_WIDTH-1:0] crypt_key;
//    wire [pPT_WIDTH-1:0] crypt_textout;
//    wire [pCT_WIDTH-1:0] crypt_cipherin;
//    wire crypt_init;
//    wire crypt_ready;
//    wire crypt_start;
//    wire crypt_done;
//    wire crypt_busy;

    wire [7:0] usb_dout;
    wire isout;
    wire [pADDR_WIDTH-pBYTECNT_SIZE-1:0] reg_address;
    wire [pBYTECNT_SIZE-1:0] reg_bytecnt;
    wire reg_addrvalid;
    wire [7:0] write_data;
    wire [7:0] read_data;
    wire reg_read;
    wire reg_write;
    wire [4:0] clk_settings;
    wire crypt_clk;
        

    wire resetn = pushbutton;
    wire reset = !resetn;
    
    //basys3_top wires 
     wire                          btnC;
     wire                          btnU;
     wire [2:0]                    leds;
     wire                          re_req_debug;
     wire                          r_ack_debug;
     wire                          w_req_debug;
     wire                          w_ack_debug;
     
     //clk_wiz_0
     wire                          CLK50MHZ;
     wire                          CWIZLOCKED;
     wire                          usb_clk_bufg;
    // USB CLK Heartbeat
//    reg [24:0] usb_timer_heartbeat;
//    always @(posedge usb_clk_buf) usb_timer_heartbeat <= usb_timer_heartbeat +  25'd1;
//    assign led1 = usb_timer_heartbeat[24];

    // CRYPT CLK Heartbeat
//    reg [22:0] crypt_clk_heartbeat;
//    always @(posedge crypt_clk) crypt_clk_heartbeat <= crypt_clk_heartbeat +  23'd1;
//    assign led2 = crypt_clk_heartbeat[22];

////Create cw305_basys3_top fot the new module basys3_top.vhdl
    basys3_top #(
        
    ) U_basys3_top (
    //name of basys3_top         name of here
        .out_CLK50MHZ           (clk_debug2),
        .CLK50MHZ               (CLK50MHZ),
        .btnC                   (btnC),
      //  .btnU                   (btnU),
        .LEDS                   (leds),
        .RsRx                   (RsRx),
        .RsTx                   (RsTx),
        .r_req_debug            (re_req_debug),
        .r_ack_debug            (r_ack_debug),
        .w_req_debug            (w_req_debug),
        .w_ack_debug            (w_ack_debug),
        .CWIZLOCKED             (CWIZLOCKED)
    );
    //End of this part
    
    cw305_usb_reg_fe #(
       .pBYTECNT_SIZE           (pBYTECNT_SIZE),
       .pADDR_WIDTH             (pADDR_WIDTH)
    ) U_usb_reg_fe (
       .rst                     (reset),
       .usb_clk                 (usb_clk_buf), 
       .usb_din                 (usb_data), 
       .usb_dout                (usb_dout), 
       .usb_rdn                 (usb_rdn), 
       .usb_wrn                 (usb_wrn),
       .usb_cen                 (usb_cen),
       .usb_alen                (1'b0),                 // unused
       .usb_addr                (usb_addr),
       .usb_isout               (isout), 
       .reg_address             (reg_address), 
       .reg_bytecnt             (reg_bytecnt), 
       .reg_datao               (write_data), 
       .reg_datai               (read_data),
       .reg_read                (reg_read), 
       .reg_write               (reg_write), 
       .reg_addrvalid           (reg_addrvalid)
    );
    
//CLK50MHZ
    clk_wiz_0 #(

    ) U_clk_wiz_0(
        .clk_out1   (CLK50MHZ),
        .reset       (btnU),
        .locked      (CWIZLOCKED),
        .clk_in1     (CLK100MHZ)
    );

//clk_gen: clk_wiz_0
//    		port map(clk_out1 => CLK50MHZ,
//            			reset => btnU,
//            			locked => CWIZLOCKED,
//            			clk_in1 => CLK100MHZ
//            		);

    assign usb_data = isout? usb_dout : 8'bZ;
    
    assign btnC = ~write_data[0];
    assign btnU = ~write_data[1];
    
    assign read_data[0] = re_req_debug;
    assign read_data[1] = r_ack_debug;
    assign read_data[2] = w_req_debug ;
    assign read_data[3] = w_ack_debug;
    
    assign led1 = reset; //leds[0];
    assign led2 = usb_rdn; //leds[1];
    assign led3 = reg_write;
    
    assign clk_debug1 = isout;
//    assign clk_debug2 = CLK100MHZ;
    assign clk_debug3 = usb_data[1];
    assign clklock = usb_data[0];
    
     IBUFG clkibuf (
        .O(usb_clk_bufg),
        .I(usb_clk) 
    );
    BUFG clkbuf(
        .O(usb_clk_buf),
        .I(usb_clk_bufg)
    );
    
//`ifdef ICE40
//    assign usb_clk_buf = usb_clk;
//    assign crypt_clk = usb_clk;
//    assign tio_clkout = usb_clk;
//`else
//    clocks U_clocks (
//       .usb_clk                 (usb_clk),
//       .usb_clk_buf             (usb_clk_buf),
//       .I_j16_sel               (j16_sel),
//       .I_k16_sel               (k16_sel),
//       .I_clock_reg             (clk_settings),
//       .I_cw_clkin              (tio_clkin),
//       .I_pll_clk1              (CLK100MHZ),
//       .O_cw_clkout             (tio_clkout),
//       .O_cryptoclk             (crypt_clk)
//    );
//`endif



 

endmodule

`default_nettype wire

