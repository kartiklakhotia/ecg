library ieee;
use ieee.std_logic_1164.all;

library unisim;
use unisim.vcomponents.all; -- for 7-series FPGA's

entity basys3_top is
	port (    
	    out_CLK50MHZ: out std_logic;
	    CLK50MHZ: in std_logic;
		-- this is the reset (btnC)
		btnC: in std_logic;
		-- this is the clocking wiz clock reset.
		--btnU: in std_logic;
		-- LEDS to be driven from AHIR system.
		LEDS: out std_logic_vector(2 downto 0);
		-- UART signals, UART configured to 115200 
		-- baud.
		RsRx: in std_logic;
		RsTx: out std_logic;
		-- Debug
		r_req_debug : out std_logic;
		r_ack_debug : out std_logic;
		w_req_debug : out std_logic;
		w_ack_debug : out std_logic;
		
		CWIZLOCKED : in std_logic
		);
end entity basys3_top;


architecture Struct of basys3_top is

	signal btnC_registered, btnC_synched: std_logic;

	component ecg_core is  -- system 
  	port ( 
    		clk : in std_logic;
    		reset : in std_logic;
    		in_data_pipe_write_data: in std_logic_vector(7 downto 0);
    		in_data_pipe_write_req : in std_logic_vector(0 downto 0);
    		in_data_pipe_write_ack : out std_logic_vector(0 downto 0);
    		out_data_pipe_read_data: out std_logic_vector(7 downto 0);
    		out_data_pipe_read_req : in std_logic_vector(0 downto 0);
    		out_data_pipe_read_ack : out std_logic_vector(0 downto 0)); -- 
	end component; 

	component serial_uart is
		generic (baud_rate, clock_frequency: integer);
		port (clk, reset: in std_logic;
			SERIAL_RX: in std_logic_vector(0 downto 0);	
			SERIAL_TX: out std_logic_vector(0 downto 0);	
	
			SERIAL_TO_CORE_pipe_read_data: out std_logic_vector(7 downto 0);
			SERIAL_TO_CORE_pipe_read_req:  in std_logic_vector(0 downto 0);
			SERIAL_TO_CORE_pipe_read_ack:  out std_logic_vector(0 downto 0);
	
			CORE_TO_SERIAL_pipe_write_data : in std_logic_vector(7 downto 0);
			CORE_TO_SERIAL_pipe_write_req :  in std_logic_vector(0 downto 0);
			CORE_TO_SERIAL_pipe_write_ack :  out std_logic_vector(0 downto 0)
		);
	end component;

	signal in_data_pipe_write_data: std_logic_vector(7 downto 0);
    	signal in_data_pipe_write_req : std_logic_vector(0 downto 0);
    	signal in_data_pipe_write_ack : std_logic_vector(0 downto 0);
    	signal out_data_pipe_read_data: std_logic_vector(7 downto 0);
    	signal out_data_pipe_read_req : std_logic_vector(0 downto 0);
    	signal out_data_pipe_read_ack : std_logic_vector(0 downto 0); -- 

begin

	
    	
	-------------------------------------------------------
	-- synch the switches.
	-------------------------------------------------------
	process(CLK50MHZ)
	begin
		if(CLK50MHZ'event and CLK50MHZ='1') then
			btnC_registered <= btnC;
			btnC_synched    <= btnC_registered;
		end if;
	end process;

	-------------------------------------------------------
	-- LEDs.. I/O activity will be displayed on the LEDs.
	-------------------------------------------------------
	process(CLK50MHZ)
	begin
		if(CLK50MHZ'event and CLK50MHZ = '1') then
			if(btnC_synched = '1') then
				LEDS <= (others => '1');
			else
				LEDS(0) <= in_data_pipe_write_data(0);
				LEDS(2 downto 1) <= out_data_pipe_read_data(1 downto 0);
			end if;
		end if;
	end process;
	

	-------------------------------------------------------
	-- the ecg core interface
	-------------------------------------------------------
	ecg_core_inst:ecg_core
  		port map( -- 
				in_data_pipe_write_data => in_data_pipe_write_data,
    				in_data_pipe_write_req  => in_data_pipe_write_req ,
    				in_data_pipe_write_ack  => in_data_pipe_write_ack ,
    				out_data_pipe_read_data => out_data_pipe_read_data,
    				out_data_pipe_read_req  => out_data_pipe_read_req ,
    				out_data_pipe_read_ack  => out_data_pipe_read_ack ,
    				clk => CLK50MHZ, 
				reset => btnC_synched
    				-- 
  			);

	-------------------------------------------------------
	-- the serial uart.
	-------------------------------------------------------
	uart_inst: serial_uart
		generic map (baud_rate => 115200, clock_frequency => 50000000)
		port map (clk => CLK50MHZ, reset => btnC_synched,
				SERIAL_TO_CORE_pipe_read_data => in_data_pipe_write_data,
    				SERIAL_TO_CORE_pipe_read_ack  => in_data_pipe_write_req ,
    				SERIAL_TO_CORE_pipe_read_req  => in_data_pipe_write_ack ,
    				CORE_TO_SERIAL_pipe_write_data => out_data_pipe_read_data,
    				CORE_TO_SERIAL_pipe_write_ack  => out_data_pipe_read_req ,
    				CORE_TO_SERIAL_pipe_write_req  => out_data_pipe_read_ack,
				SERIAL_RX(0) => RsRx,
				SERIAL_TX(0) => RsTx);
				
	-------------------------------------------------------
	-- DEBUG
	-------------------------------------------------------	
	
	r_req_debug <= in_data_pipe_write_ack(0);
    r_ack_debug <= in_data_pipe_write_req(0);
    w_req_debug <= out_data_pipe_read_ack(0);
    w_ack_debug <= out_data_pipe_read_req(0);
    -- clock checking
    out_CLK50MHZ <= CLK50MHZ;
				
end Struct;
