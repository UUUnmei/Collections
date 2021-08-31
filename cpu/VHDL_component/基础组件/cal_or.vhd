-- 或运算
library ieee;
use ieee.std_logic_1164.all;


entity cal_or is
port(A, B: in std_logic_vector(15 downto 0);		-- A or B
	  S : out std_logic_vector(15	 downto 0));			
end cal_or;

architecture work of cal_or is

begin
	
	S <= A or B;
	
end architecture work;

