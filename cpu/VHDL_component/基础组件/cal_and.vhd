-- 与运算
library ieee;
use ieee.std_logic_1164.all;


entity cal_and is
port(A, B: in std_logic_vector(15 downto 0);		-- A and B
	  S : out std_logic_vector(15	 downto 0));			
end cal_and;

architecture work of cal_and is

begin
	
	S <= A and B;
	
end architecture work;

