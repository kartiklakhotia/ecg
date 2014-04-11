library std;
use std.standard.all;
library ieee;
use ieee.std_logic_1164.all;
library ahir;
use ahir.memory_subsystem_package.all;
use ahir.types.all;
use ahir.subprograms.all;
use ahir.components.all;
use ahir.basecomponents.all;
use ahir.operatorpackage.all;
use ahir.utilities.all;
use ahir.functionLibraryComponents.all;
library work;
use work.ahir_system_global_package.all;
library GhdlLink;
use GhdlLink.Utility_Package.all;
use GhdlLink.Vhpi_Foreign.all;
entity ahir_system_Test_Bench is -- 
  -- 
end entity;
architecture VhpiLink of ahir_system_Test_Bench is -- 
  component ahir_system is -- 
    port (-- 
      clk : in std_logic;
      reset : in std_logic;
      det_input_pipe_pipe_write_data: in std_logic_vector(31 downto 0);
      det_input_pipe_pipe_write_req : in std_logic_vector(0 downto 0);
      det_input_pipe_pipe_write_ack : out std_logic_vector(0 downto 0);
      det_output_pipe_pipe_read_data: out std_logic_vector(31 downto 0);
      det_output_pipe_pipe_read_req : in std_logic_vector(0 downto 0);
      det_output_pipe_pipe_read_ack : out std_logic_vector(0 downto 0)); -- 
    -- 
  end component;
  signal clk: std_logic := '0';
  signal reset: std_logic := '1';
  signal qrsDet_tag_in: std_logic_vector(1 downto 0);
  signal qrsDet_tag_out: std_logic_vector(1 downto 0);
  signal qrsDet_start_req : std_logic := '0';
  signal qrsDet_start_ack : std_logic := '0';
  signal qrsDet_fin_req   : std_logic := '0';
  signal qrsDet_fin_ack   : std_logic := '0';
  -- write to pipe det_input_pipe
  signal det_input_pipe_pipe_write_data: std_logic_vector(31 downto 0);
  signal det_input_pipe_pipe_write_req : std_logic_vector(0 downto 0) := (others => '0');
  signal det_input_pipe_pipe_write_ack : std_logic_vector(0 downto 0);
  -- read from pipe det_output_pipe
  signal det_output_pipe_pipe_read_data: std_logic_vector(31 downto 0);
  signal det_output_pipe_pipe_read_req : std_logic_vector(0 downto 0) := (others => '0');
  signal det_output_pipe_pipe_read_ack : std_logic_vector(0 downto 0);
  -- 
begin --
  -- clock/reset generation 
  clk <= not clk after 5 ns;
  process
  begin --
    Vhpi_Initialize;
    wait until clk = '1';
    reset <= '0';
    while true loop --
      wait until clk = '0';
      Vhpi_Listen;
      Vhpi_Send;
      --
    end loop;
    wait;
    --
  end process;
  -- connect all the top-level modules to Vhpi
  process
  variable val_string, obj_ref: VhpiString;
  begin --
    wait until reset = '0';
    while true loop -- 
      wait until clk = '0';
      wait for 1 ns; 
      obj_ref := Pack_String_To_Vhpi_String("det_input_pipe req");
      Vhpi_Get_Port_Value(obj_ref,val_string,1);
      det_input_pipe_pipe_write_req <= Unpack_String(val_string,1);
      obj_ref := Pack_String_To_Vhpi_String("det_input_pipe 0");
      Vhpi_Get_Port_Value(obj_ref,val_string,32);
      det_input_pipe_pipe_write_data <= Unpack_String(val_string,32);
      wait until clk = '1';
      obj_ref := Pack_String_To_Vhpi_String("det_input_pipe ack");
      val_string := Pack_SLV_To_Vhpi_String(det_input_pipe_pipe_write_ack);
      Vhpi_Set_Port_Value(obj_ref,val_string,1);
      -- 
    end loop;
    --
  end process;
  process
  variable val_string, obj_ref: VhpiString;
  begin --
    wait until reset = '0';
    while true loop -- 
      wait until clk = '0';
      wait for 1 ns; 
      obj_ref := Pack_String_To_Vhpi_String("det_output_pipe req");
      Vhpi_Get_Port_Value(obj_ref,val_string,1);
      det_output_pipe_pipe_read_req <= Unpack_String(val_string,1);
      wait until clk = '1';
      obj_ref := Pack_String_To_Vhpi_String("det_output_pipe ack");
      val_string := Pack_SLV_To_Vhpi_String(det_output_pipe_pipe_read_ack);
      Vhpi_Set_Port_Value(obj_ref,val_string,1);
      obj_ref := Pack_String_To_Vhpi_String("det_output_pipe 0");
      val_string := Pack_SLV_To_Vhpi_String(det_output_pipe_pipe_read_data);
      Vhpi_Set_Port_Value(obj_ref,val_string,32);
      -- 
    end loop;
    --
  end process;
  ahir_system_instance: ahir_system -- 
    port map ( -- 
      clk => clk,
      reset => reset,
      det_input_pipe_pipe_write_data  => det_input_pipe_pipe_write_data, 
      det_input_pipe_pipe_write_req  => det_input_pipe_pipe_write_req, 
      det_input_pipe_pipe_write_ack  => det_input_pipe_pipe_write_ack,
      det_output_pipe_pipe_read_data  => det_output_pipe_pipe_read_data, 
      det_output_pipe_pipe_read_req  => det_output_pipe_pipe_read_req, 
      det_output_pipe_pipe_read_ack  => det_output_pipe_pipe_read_ack ); -- 
  -- 
end VhpiLink;
