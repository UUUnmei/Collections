-- 异或运算
library ieee;
use ieee.std_logic_1164.all;


entity cal_xor is
port(A, B: in std_logic_vector(15 downto 0);		-- A xor B
	  S : out std_logic_vector(15	 downto 0));			
end cal_xor;

architecture work of cal_xor is

begin
	
	S <= A xor B;
	
end architecture work;

