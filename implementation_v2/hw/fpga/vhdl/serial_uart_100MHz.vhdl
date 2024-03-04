library ieee;
use ieee.std_logic_1164.all;

library simpleUartLib;
use simpleUartLib.uartPackage.all;

entity serial_uart is
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
end entity serial_uart;

architecture Struct of serial_uart is
begin
	uart_inst: uartTopGenericConfigurable
		generic map (baudFreq => uart_baudFreq_115200,
					baudLimit => uart_baudLimit_115200)
		port map (
				reset => reset,
				clk => clk,
				serIn => SERIAL_RX(0),	
				serOut => SERIAL_TX(0),	
	 			uart_rx_pipe_read_data => SERIAL_TO_CORE_pipe_read_data,
	 			uart_rx_pipe_read_req => SERIAL_TO_CORE_pipe_read_req,
	 			uart_rx_pipe_read_ack => SERIAL_TO_CORE_pipe_read_ack,
	 			uart_tx_pipe_write_data => CORE_TO_SERIAL_pipe_write_data,
	 			uart_tx_pipe_write_req =>  CORE_TO_SERIAL_pipe_write_req,
	 			uart_tx_pipe_write_ack =>  CORE_TO_SERIAL_pipe_write_ack
		);

end Struct;
	
