-- VHDL global package produced by vc2vhdl from virtual circuit (vc) description 
library ieee;
use ieee.std_logic_1164.all;
package ahir_system_global_package is -- 
  constant DDCALCbuff_base_address : std_logic_vector(1 downto 0) := "00";
  constant DDCALCbuff_ptr_base_address : std_logic_vector(0 downto 0) := "0";
  constant DDbuff_base_address : std_logic_vector(6 downto 0) := "0000000";
  constant DDbuff_ptr_base_address : std_logic_vector(0 downto 0) := "0";
  constant DERIVbuff_base_address : std_logic_vector(1 downto 0) := "00";
  constant DERIVbuff_ptr_base_address : std_logic_vector(0 downto 0) := "0";
  constant HPbuff_base_address : std_logic_vector(4 downto 0) := "00000";
  constant HPbuff_ptr_base_address : std_logic_vector(0 downto 0) := "0";
  constant HPy1_base_address : std_logic_vector(0 downto 0) := "0";
  constant LPbuff_base_address : std_logic_vector(3 downto 0) := "0000";
  constant LPbuff_ptr_base_address : std_logic_vector(0 downto 0) := "0";
  constant LPy1_base_address : std_logic_vector(0 downto 0) := "0";
  constant LPy2_base_address : std_logic_vector(0 downto 0) := "0";
  constant NOISEbuff_base_address : std_logic_vector(4 downto 0) := "00000";
  constant NOISEbuff_ptr_base_address : std_logic_vector(0 downto 0) := "0";
  constant QRSbuff_base_address : std_logic_vector(4 downto 0) := "01000";
  constant QRSbuff_ptr_base_address : std_logic_vector(0 downto 0) := "0";
  constant RRbuff_base_address : std_logic_vector(4 downto 0) := "10000";
  constant RRbuff_ptr_base_address : std_logic_vector(0 downto 0) := "0";
  constant WINbuff_base_address : std_logic_vector(4 downto 0) := "00000";
  constant WINbuff_ptr_base_address : std_logic_vector(0 downto 0) := "0";
  constant WINsum_base_address : std_logic_vector(0 downto 0) := "0";
  constant lastDatum_base_address : std_logic_vector(0 downto 0) := "0";
  constant maxDer_base_address : std_logic_vector(0 downto 0) := "0";
  constant maxPeak_base_address : std_logic_vector(0 downto 0) := "0";
  constant mempool_base_address : std_logic_vector(0 downto 0) := "0";
  constant threshold_base_address : std_logic_vector(0 downto 0) := "0";
  constant timeSinceMaxPeak_base_address : std_logic_vector(0 downto 0) := "0";
  -- 
end package ahir_system_global_package;
